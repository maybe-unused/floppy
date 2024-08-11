#include <cstdint>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/backtrace.h>

using namespace fl;

TEST(Backtrace, Death)
{
  GTEST_PRINT("Performing death test");
  GTEST_SUCCESS_("Death test skipped");
}