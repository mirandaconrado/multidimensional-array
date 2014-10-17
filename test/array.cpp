#include "array.hpp"

#include <gtest/gtest.h>

using namespace MultidimensionalArray;

class ArrayTest: public ::testing::Test {
  protected:
    Size::SizeType sizes;
    int* values;

    virtual void SetUp() {
      sizes = Size::SizeType({2, 3, 4, 5});
      values = new int[2*3*4*5];
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

TEST_F(ArrayTest, AssignmentOperator) {
  Array<int> array(sizes, values), array2({1});

  array2.resize(array.size());
  array2 = array;

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.size());
  check_sizes(array2.size());
  EXPECT_NE(array.get_pointer(), array2.get_pointer());
}

TEST_F(ArrayTest, AssignmentOperatorWithDifferentType) {
  Array<int> array(sizes, values);
  Array<float> array2({1});

  array2.resize(array.size());
  array2 = array;

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.size());
  check_sizes(array2.size());
}

TEST_F(ArrayTest, AssignmentOperatorWithMove) {
  Array<int> array(sizes, values), array2({1});

  array2.resize(array.size());
  array2 = std::move(array);

  check_values(array2.get_pointer());
  check_sizes(array2.size());
}

TEST_F(ArrayTest, CopyConstructor) {
  Array<int> array(sizes, values), array2(array);

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.size());
  check_sizes(array2.size());
  EXPECT_NE(array.get_pointer(), array2.get_pointer());
}

TEST_F(ArrayTest, CopyConstructorWithDifferentType) {
  Array<int> array(sizes, values);
  Array<float> array2(array);

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.size());
  check_sizes(array2.size());
}

TEST_F(ArrayTest, MoveConstructor) {
  Array<int> array(sizes, values),
    array2(std::move(array));

  check_values(array2.get_pointer());
  check_sizes(array2.size());
}

TEST_F(ArrayTest, Resize) {
  Array<int> array(sizes, values);

  EXPECT_TRUE(array.resize({5, 4, 3, 2}));

  check_values(array.get_pointer());
  EXPECT_EQ(sizes.size(), array.size().size());
  for (size_t i = 0; i < sizes.size(); i++)
    EXPECT_NE(sizes[i], array.size()[i]);

  EXPECT_TRUE(array.resize({2, 3, 4, 5}));
  check_sizes(array.size());

  EXPECT_TRUE(array.resize({4, 3, 2}));
}

TEST_F(ArrayTest, SetPointer) {
  {
    Array<int> array(sizes);
    array.set_pointer(values, false);
  }
  {
    Array<int> array(sizes);
    array.set_pointer(new int[1], true);
  }
}

TEST_F(ArrayTest, Swap) {
  Array<int> array(sizes, values), array2;

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

TEST_F(ArrayTest, ValueAccess) {
  Array<int> array(sizes, values);

  size_t index = 0;
  for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
    for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
      for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
        for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
          EXPECT_EQ(index, array(i1, i2, i3, i4));
          EXPECT_EQ(index, array.get({i1, i2, i3, i4}));
          index++;
        }
}

TEST_F(ArrayTest, VectorConstructor) {
  Array<int> array(sizes, values);

  check_values(array.get_pointer());
  check_sizes(array.size());
}

TEST_F(ArrayTest, VectorConstructorWithDifferentType) {
  Array<float> array(sizes, values);

  check_values(array.get_pointer());
  check_sizes(array.size());
}
