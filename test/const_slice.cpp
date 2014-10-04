#include "const_slice.hpp"

#include <gtest/gtest.h>

#include <vector>

class ConstSliceTest: public ::testing::Test {
  protected:
    std::vector<unsigned int> sizes;
    int *values;

    virtual void SetUp() {
      sizes = std::vector<unsigned int>({2, 3, 4, 5});
      values = new int[2*3*4*5*6];
      unsigned int index = 0;
      for (int i1 = 0; i1 < 2; i1++)
        for (int i2 = 0; i2 < 3; i2++)
          for (int i3 = 0; i3 < 4; i3++)
            for (int i4 = 0; i4 < 5; i4++) {
              values[index] = index;
              index++;
            }
    }

    virtual void TearDown() {
      delete[] values;
    }

    void check_sizes(std::vector<unsigned int> const& sizes1,
        std::vector<unsigned int> const& sizes2) {
      EXPECT_EQ(sizes1.size(), sizes2.size());
      for (unsigned int i = 0; i < sizes1.size(); i++)
        EXPECT_EQ(sizes1[i], sizes2[i]);
    }
};

TEST_F(ConstSliceTest, Elements) {
  MultidimensionalArray::ConstArray<int> array(sizes, values);
  MultidimensionalArray::ConstSlice<int> slice(array, 1);

  unsigned int index = 0, element_index = 0, inner_index = 0;
  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++) {
      MultidimensionalArray::ConstArray<int> temp(
          slice.get_element(element_index));
      int const* values = temp.get_pointer();
      inner_index = 0;
      for (int i3 = 0; i3 < 4; i3++)
        for (int i4 = 0; i4 < 5; i4++) {
          EXPECT_EQ(index, values[inner_index]);
          EXPECT_EQ(index, temp(i3, i4));
          index++;
          inner_index++;
        }
      element_index++;
    }
}

TEST_F(ConstSliceTest, Sizes) {
  MultidimensionalArray::ConstArray<int> array(sizes, values);

  {
    MultidimensionalArray::ConstSlice<int> slice(array, 0);
    check_sizes(slice.get_left_size(), {2});
    check_sizes(slice.get_right_size(), {3, 4, 5});
  }

  {
    MultidimensionalArray::ConstSlice<int> slice(array, 1);
    check_sizes(slice.get_left_size(), {2, 3});
    check_sizes(slice.get_right_size(), {4, 5});
  }

  {
    MultidimensionalArray::ConstSlice<int> slice(array, 2);
    check_sizes(slice.get_left_size(), {2, 3, 4});
    check_sizes(slice.get_right_size(), {5});
  }
}
