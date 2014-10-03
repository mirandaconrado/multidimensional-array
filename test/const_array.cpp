#include "const_array.hpp"
#include "array.hpp"

#include <gtest/gtest.h>

#include <vector>

class ConstArrayTest: public ::testing::Test {
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

    template <class T>
    void check_values(T* values) {
      unsigned int index = 0;
      for (int i1 = 0; i1 < 2; i1++)
        for (int i2 = 0; i2 < 3; i2++)
          for (int i3 = 0; i3 < 4; i3++)
            for (int i4 = 0; i4 < 5; i4++) {
              EXPECT_EQ(index, values[index]);
              index++;
            }
    }

    template <class T>
    void check_zero(T* values) {
      unsigned int index = 0;
      for (int i1 = 0; i1 < 2; i1++)
        for (int i2 = 0; i2 < 3; i2++)
          for (int i3 = 0; i3 < 4; i3++)
            for (int i4 = 0; i4 < 5; i4++) {
              EXPECT_EQ(0, values[index]);
              index++;
            }
    }

    void check_sizes(std::vector<unsigned int> const& other_sizes) {
      EXPECT_EQ(sizes.size(), other_sizes.size());
      for (unsigned int i = 0; i < sizes.size(); i++)
        EXPECT_EQ(sizes[i], other_sizes[i]);
    }
};

TEST_F(ConstArrayTest, CopyConstructor) {
  MultidimensionalArray::ConstArray<int> array(sizes, values), array2(array);

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.get_size());
  check_sizes(array2.get_size());
  EXPECT_EQ(array.get_pointer(), array2.get_pointer());
}

TEST_F(ConstArrayTest, CopyConstructorFromArray) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::ConstArray<int> array2(array);

  check_values(array.get_pointer());
  check_values(array2.get_pointer());
  check_sizes(array.get_size());
  check_sizes(array2.get_size());
  EXPECT_EQ(array.get_pointer(), array2.get_pointer());
}

TEST_F(ConstArrayTest, MoveConstructor) {
  MultidimensionalArray::ConstArray<int> array(sizes, values),
    array2(std::move(array));

  check_values(array2.get_pointer());
  check_sizes(array2.get_size());
}

TEST_F(ConstArrayTest, MoveConstructorFromArray) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::ConstArray<int> array2(std::move(array));

  check_values(array2.get_pointer());
  check_sizes(array2.get_size());
}

TEST_F(ConstArrayTest, Resize) {
  MultidimensionalArray::ConstArray<int> array(sizes, values);

  EXPECT_TRUE(array.resize({5, 4, 3, 2}));

  check_values(array.get_pointer());
  EXPECT_EQ(sizes.size(), array.get_size().size());
  for (unsigned int i = 0; i < sizes.size(); i++)
    EXPECT_NE(sizes[i], array.get_size()[i]);

  EXPECT_TRUE(array.resize({2, 3, 4, 5}));
  check_sizes(array.get_size());

  EXPECT_FALSE(array.resize({4, 3, 2}));
}

TEST_F(ConstArrayTest, SetPointer) {
  {
    MultidimensionalArray::ConstArray<int> array(sizes);
    array.set_pointer(values, false);
  }
  {
    MultidimensionalArray::ConstArray<int> array(sizes);
    array.set_pointer(new int[1], true);
  }
}

TEST_F(ConstArrayTest, ValueAccess) {
  MultidimensionalArray::ConstArray<int> array(sizes, values);

  unsigned int index = 0;
  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 0; i3 < 4; i3++)
        for (int i4 = 0; i4 < 5; i4++) {
          EXPECT_EQ(index, array(i1, i2, i3, i4));
          index++;
        }
}

TEST_F(ConstArrayTest, VectorConstructor) {
  MultidimensionalArray::ConstArray<int> array(sizes, values);

  check_values(array.get_pointer());
  check_sizes(array.get_size());

  MultidimensionalArray::ConstArray<int> array2(sizes, new int[1], true);
}