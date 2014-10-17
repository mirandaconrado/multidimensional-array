#include "array.hpp"
#include "const_array.hpp"

#include <gtest/gtest.h>

using namespace MultidimensionalArray;

class ConstArrayTest: public ::testing::Test {
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

    template <class T>
    void check_values(T* values) {
      size_t index = 0;
      for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
        for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
          for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
            for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
              EXPECT_EQ(index, values[index]);
              index++;
            }
    }

    template <class T>
    void check_zero(T* values) {
      size_t index = 0;
      for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
        for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
          for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
            for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
              EXPECT_EQ(0, values[index]);
              index++;
            }
    }

    void check_sizes(Size::SizeType const& other_sizes) {
      EXPECT_EQ(sizes.size(), other_sizes.size());
      for (size_t i = 0; i < sizes.size(); i++)
        EXPECT_EQ(sizes[i], other_sizes[i]);
    }
};

TEST_F(ConstArrayTest, CopyConstructor) {
  ConstArray<int> array(sizes, values), array2(array);

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.size());
  check_sizes(array2.size());
  EXPECT_EQ(array.get_pointer(), array2.get_pointer());
}

TEST_F(ConstArrayTest, CopyConstructorFromArray) {
  Array<int> array(sizes, values);
  ConstArray<int> array2(array);

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.size());
  check_sizes(array2.size());
  EXPECT_EQ(array.get_pointer(), array2.get_pointer());
}

TEST_F(ConstArrayTest, MoveConstructor) {
  ConstArray<int> array(sizes, values),
    array2(std::move(array));

  check_values(array2.get_pointer());
  check_sizes(array2.size());
}

TEST_F(ConstArrayTest, MoveConstructorFromArray) {
  Array<int> array(sizes, values);
  ConstArray<int> array2(std::move(array));

  check_values(array2.get_pointer());
  check_sizes(array2.size());
}

TEST_F(ConstArrayTest, Resize) {
  ConstArray<int> array(sizes, values);

  EXPECT_TRUE(array.resize({5, 4, 3, 2}));

  check_values(array.get_pointer());
  EXPECT_EQ(sizes.size(), array.size().size());
  for (size_t i = 0; i < sizes.size(); i++)
    EXPECT_NE(sizes[i], array.size()[i]);

  EXPECT_TRUE(array.resize({2, 3, 4, 5}));
  check_sizes(array.size());

  EXPECT_FALSE(array.resize({4, 3, 2}));
}

TEST_F(ConstArrayTest, SetPointer) {
  {
    ConstArray<int> array(sizes);
    array.set_pointer(values, false);
  }
  {
    ConstArray<int> array(sizes);
    array.set_pointer(new int[1], true);
  }
}

TEST_F(ConstArrayTest, Swap) {
  ConstArray<int> array(sizes, values), array2;

  array2.swap(array);

  size_t index = 0;
  for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
    for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
      for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
        for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
          EXPECT_EQ(index, array2(i1, i2, i3, i4));
          index++;
        }

  EXPECT_EQ(0, array.size().size());
}

TEST_F(ConstArrayTest, ValueAccess) {
  ConstArray<int> array(sizes, values);

  size_t index = 0;
  for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
    for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
      for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
        for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
          EXPECT_EQ(index, array(i1, i2, i3, i4));
          index++;
        }
}

TEST_F(ConstArrayTest, VectorConstructor) {
  ConstArray<int> array(sizes, values);

  check_values(array.get_pointer());
  check_sizes(array.size());

  ConstArray<int> array2(sizes, new int[1], true);
}
