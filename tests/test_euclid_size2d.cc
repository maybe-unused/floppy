#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid.h>

using fl::math::size2d;
using namespace fl::types;

struct Cm {};
struct Mm {};

TEST(EuclidSize2D, Area)
{
  auto const p = size2d(1.5, 2.0);
  EXPECT_EQ(p.x(), 1.5);
  EXPECT_EQ(p.y(), 2.0);
  EXPECT_EQ(p.area(), 3.0);
}