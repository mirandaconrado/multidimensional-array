#include "size.hpp"

#include <gtest/gtest.h>

using namespace MultidimensionalArray;

static void check_sizes(Size::SizeType const& sizes1,
    Size::SizeType const& sizes2) {
  EXPECT_EQ(sizes1.size(), sizes2.size());
  for (size_t i = 0; i < sizes1.size(); i++)
    EXPECT_EQ(sizes1[i], sizes2[i]);
}

TEST(SizeTest, Accessors) {
  Size::SizeType sizes({2, 3, 4, 5});
  Size size(sizes);

  EXPECT_TRUE(size.same(sizes));
  EXPECT_FALSE(size.same(Size::SizeType(
          {2, 3, 4, 5, 6})));
  EXPECT_FALSE(size.same(Size::SizeType(
          {2, 3, 4, 6})));

  EXPECT_EQ(2*3*4*5, size.get_total_size());
}

TEST(SizeTest, Iterator) {
  Size::SizeType sizes({2, 3, 4, 5});
  Size size(sizes);

  auto it1 = size.cbegin();
  auto it2 = size.cend();

  for (Size::SizeType::value_type i1 = 0; i1 < 2; i1++)
    for (Size::SizeType::value_type i2 = 0; i2 < 3; i2++)
      for (Size::SizeType::value_type i3 = 0; i3 < 4; i3++)
        for (Size::SizeType::value_type i4 = 0; i4 < 5; i4++) {
          check_sizes(Size::SizeType({i1, i2, i3, i4}),
              *it1);
          EXPECT_TRUE(size.check_index(*it1));
          Size::SizeType temp(*it1);
          temp[0] += 2;
          EXPECT_FALSE(size.check_index(temp));
          ++it1;
        }

  EXPECT_EQ(it2, it1);
}
