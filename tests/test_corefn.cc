#include <gtest/gtest.h>
#include <floppy/floppy.h>

using namespace fl;

TEST(CoreFn, Panic)
{
  GTEST_PRINT("testing panic\n");
  ASSERT_DEATH(panic("test"), "");
}

TEST(CoreFn, NarrowCast)
{
  auto a = 1_u8;
  auto const& b = a;
  auto&& c = 23;
  auto const&& d = 256;

  EXPECT_EQ(narrow_cast<u16>(a), 1);
  EXPECT_EQ(narrow_cast<u16>(b), 1);
  EXPECT_EQ(narrow_cast<u16>(c), 23);
  EXPECT_EQ(narrow_cast<u16>(d), 256);
  EXPECT_EQ(narrow_cast<u8>(d), 0);
}

TEST(CoreFn, Rtti)
{
  EXPECT_EQ(rtti::type_name<int>(), "int");
  EXPECT_EQ(rtti::type_name<float>(), "float");
  EXPECT_EQ(rtti::type_name<double>(), "double");
  EXPECT_EQ(rtti::type_name<char>(), "char");
  EXPECT_EQ(rtti::type_name<std::vector<int>>(), "std::vector<int, std::allocator<int> >");
}