#include "view.hpp"

#include <gtest/gtest.h>

#include <vector>

class ViewTest: public ::testing::Test {
  protected:
    std::vector<unsigned int> sizes;
    int *values;

    virtual void SetUp() {
      sizes = std::vector<unsigned int>({2, 3, 4, 5, 6, 7});
      values = new int[2*3*4*5*6*7];
      unsigned int index = 0;
      for (int i1 = 0; i1 < 2; i1++)
        for (int i2 = 0; i2 < 3; i2++)
          for (int i3 = 0; i3 < 4; i3++)
            for (int i4 = 0; i4 < 5; i4++)
              for (int i5 = 0; i5 < 6; i5++)
                for (int i6 = 0; i6 < 7; i6++) {
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

TEST_F(ViewTest, AssignmentOperator) {
  MultidimensionalArray::Array<float> array(sizes, values);
  MultidimensionalArray::Array<int> array2(sizes);

  array2 = array.view();
  unsigned int index = 0;
  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 0; i3 < 4; i3++)
        for (int i4 = 0; i4 < 5; i4++)
          for (int i5 = 0; i5 < 6; i5++)
            for (int i6 = 0; i6 < 7; i6++) {
              EXPECT_EQ(index, array2(i1, i2, i3, i4, i5, i6));
              index++;
            }
}

TEST_F(ViewTest, Basic) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::View<int> view(array.view());

  unsigned int index = 0;
  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 0; i3 < 4; i3++)
        for (int i4 = 0; i4 < 5; i4++)
          for (int i5 = 0; i5 < 6; i5++)
            for (int i6 = 0; i6 < 7; i6++) {
              EXPECT_EQ(index, view(i1, i2, i3, i4, i5, i6));
              index++;
            }
}

TEST_F(ViewTest, Begin) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::View<int> view(array.view().set_range_begin(2, 3).
      set_range_begin(3, 2).set_range_begin(4, 1));

  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 3; i3 < 4; i3++)
        for (int i4 = 2; i4 < 5; i4++)
          for (int i5 = 1; i5 < 6; i5++)
            for (int i6 = 0; i6 < 7; i6++)
              EXPECT_EQ(array(i1, i2, i3, i4, i5, i6),
                  view(i1, i2, i3-3, i4-2, i5-1, i6));
}

TEST_F(ViewTest, End) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::View<int> view(array.view().set_range_end(2, 3).
      set_range_end(3, 2).set_range_end(4, 1));

  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 0; i3 < 3; i3++)
        for (int i4 = 0; i4 < 2; i4++)
          for (int i5 = 0; i5 < 1; i5++)
            for (int i6 = 0; i6 < 7; i6++)
              EXPECT_EQ(array(i1, i2, i3, i4, i5, i6),
                  view(i1, i2, i3, i4, i5, i6));
}

TEST_F(ViewTest, Fix) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::View<int> view(array.view().fix_dimension(3, 2));

  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 0; i3 < 4; i3++)
        for (int i4 = 2; i4 < 3; i4++)
          for (int i5 = 0; i5 < 6; i5++)
            for (int i6 = 0; i6 < 7; i6++)
              EXPECT_EQ(array(i1, i2, i3, i4, i5, i6),
                  view(i1, i2, i3, i5, i6));
}

TEST_F(ViewTest, Mixed) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::View<int> view(array.view().set_range_begin(2, 3).
      set_range_end(3, 2).set_range_stride(4, 2));

  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 3; i3 < 4; i3++)
        for (int i4 = 0; i4 < 2; i4++)
          for (int i5 = 0; i5 < 6; i5+=2)
            for (int i6 = 0; i6 < 7; i6++)
              EXPECT_EQ(array(i1, i2, i3, i4, i5, i6),
                  view(i1, i2, i3-3, i4, i5/2, i6));
}

TEST_F(ViewTest, FullMixed) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::View<int> view(array.view().set_range_begin(5, 1).
      set_range_stride(5, 2).set_range_end(5, 2));

  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 0; i3 < 4; i3++)
        for (int i4 = 0; i4 < 5; i4++)
          for (int i5 = 0; i5 < 6; i5++)
            for (int i6 = 1; i6 < 5; i6+=2)
              EXPECT_EQ(array(i1, i2, i3, i4, i5, i6),
                  view(i1, i2, i3, i4, i5, (i6-1)/2));
}

TEST_F(ViewTest, Stride) {
  MultidimensionalArray::Array<int> array(sizes, values);
  MultidimensionalArray::View<int> view(array.view().set_range_stride(2, 3).
      set_range_stride(3, 2));

  for (int i1 = 0; i1 < 2; i1++)
    for (int i2 = 0; i2 < 3; i2++)
      for (int i3 = 0; i3 < 4; i3 += 3)
        for (int i4 = 0; i4 < 5; i4 += 2)
          for (int i5 = 0; i5 < 6; i5++)
            for (int i6 = 0; i6 < 7; i6++)
              ASSERT_EQ(array(i1, i2, i3, i4, i5, i6),
                  view(i1, i2, i3/3, i4/2, i5, i6));
}