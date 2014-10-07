#include "size.hpp"

#include <gtest/gtest.h>

void check_sizes(MultidimensionalArray::Size::SizeType const& sizes1,
    MultidimensionalArray::Size::SizeType const& sizes2) {
  EXPECT_EQ(sizes1.size(), sizes2.size());
  for (size_t i = 0; i < sizes1.size(); i++)
    EXPECT_EQ(sizes1[i], sizes2[i]);
}

TEST(SizeTest, Iterator) {
  MultidimensionalArray::Size::SizeType sizes({2, 3, 4, 5});
  MultidimensionalArray::Size size(sizes);

  EXPECT_TRUE(size.same(sizes));
  EXPECT_FALSE(size.same(MultidimensionalArray::Size::SizeType(
          {2, 3, 4, 5, 6})));
  EXPECT_FALSE(size.same(MultidimensionalArray::Size::SizeType(
          {2, 3, 4, 6})));

  auto it1 = size.cbegin();
  auto it2 = size.cend();

  for (MultidimensionalArray::Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
    for (MultidimensionalArray::Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
      for (MultidimensionalArray::Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
        for (MultidimensionalArray::Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
          check_sizes(MultidimensionalArray::Size::SizeType({i1, i2, i3, i4}),
              *it1);
          EXPECT_TRUE(size.check_index(*it1));
          MultidimensionalArray::Size::SizeType temp(*it1);
          temp[0] += 2;
          EXPECT_FALSE(size.check_index(temp));
          ++it1;
        }

  EXPECT_EQ(it2, it1);
}
