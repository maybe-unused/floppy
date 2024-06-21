#include <sstream>
#include <gtest/gtest.h>
#include <floppy/floppy.h>

TEST(PrintHelpers, GoogleTestPrint)
{
  GTEST_PRINT("example print");
  GTEST_PRINT("example print: {}", 1);
  GTEST_SUCCEED();
}

TEST(PrintHelpers, PrintVersion)
{
  auto ss = std::stringstream{};
  ss << floppy::meta::version { 1, 2, 3 };

  EXPECT_EQ(ss.str(), "1.2.3");
}

TEST(PrintHelpers, PrintMeta)
{
  auto ss = std::stringstream{};
  ss << floppy::meta::project_meta({ 1, 2, 3 }, "abbb", "c.io", "b corp");

  EXPECT_EQ(ss.str(), "abbb v. 1.2.3 (c) b corp <c.io>");
}