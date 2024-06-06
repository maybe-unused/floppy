#include <gtest/gtest.h>
#include <floppy/detail/print_helpers.h>

TEST(PrintHelpers, GoogleTestPrint)
{
  GTEST_PRINT("example print");
  GTEST_PRINT("example print: {}", 1);
}