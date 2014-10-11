#include "const_array.hpp"
#include "const_slice.hpp"

#include <gtest/gtest.h>

using namespace MultidimensionalArray;

class ConstSliceTest: public ::testing::Test {
  protected:
    Size::SizeType sizes;
    int *values;

    virtual void SetUp() {
      sizes = Size::SizeType({2, 3, 4, 5});
      values = new int[2*3*4*5*6];
      size_t index = 0;
      for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
        for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
          for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
            for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
              values[index] = index;
              index++;
            }
    }

    virtual void TearDown() {
      delete[] values;
    }

    void check_sizes(Size::SizeType const& sizes1,
        Size::SizeType const& sizes2) {
      EXPECT_EQ(sizes1.size(), sizes2.size());
      for (size_t i = 0; i < sizes1.size(); i++)
        EXPECT_EQ(sizes1[i], sizes2[i]);
    }
};

TEST_F(ConstSliceTest, Elements) {
  ConstArray<int> array(sizes, values);
  ConstSlice<int> slice(array, 1);

  size_t index = 0, element_index = 0, inner_index = 0;
  for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
    for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++) {
      ConstArray<int> temp(
          slice.get_element(element_index));
      int const* values = temp.get_pointer();
      inner_index = 0;
      for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
        for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
          EXPECT_EQ(index, values[inner_index]);
          EXPECT_EQ(index, temp(i3, i4));
          index++;
          inner_index++;
        }
      element_index++;
    }
}

TEST_F(ConstSliceTest, Sizes) {
  ConstArray<int> array(sizes, values);

  {
    ConstSlice<int> slice(array, 0);
    check_sizes(slice.left_size(), {2});
    check_sizes(slice.right_size(), {3, 4, 5});
  }

  {
    ConstSlice<int> slice(array, 1);
    check_sizes(slice.left_size(), {2, 3});
    check_sizes(slice.right_size(), {4, 5});
  }

  {
    ConstSlice<int> slice(array, 2);
    check_sizes(slice.left_size(), {2, 3, 4});
    check_sizes(slice.right_size(), {5});
  }
}
