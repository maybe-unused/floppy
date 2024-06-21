#include <vector>
#include <numeric>
#include <gtest/gtest.h>
#include <floppy/graphics.h>

using fl::gfx::color;

// static_assert(color(50, 168, 82).rgb() == color(0x32A852FF).rgb());
//   static_assert(color("#32a852").rgb() == color(0x32A852FF).rgb());
//   static_assert(color("32a852").rgb() == color(0x32A852FF).rgb());
//   static_assert(color(50, 168, 82, 128).rgba() == color(0x32A85280).rgba());
//   static_assert(color("#32a85280").rgba() == color(0x32A85280).rgba());
//   static_assert(color("32a85280").rgba() == color(0x32A85280).rgba());
//   static_assert(color("32a85280").hsl() == color::hsl_t(136, 54.0F, 43.0F));
//   static_assert(color::from_floats(0.1966F, 0.659F, 0.322F, 0.505F).rgba() == color("32a85280").rgba());

auto ref = color(0x32A85280);

TEST(GraphicsColor, ConstructorU8)
{
  auto const c = color(50, 168, 82);

  EXPECT_EQ(c.rgb(), ref.rgb());
  EXPECT_NE(c.rgba(), ref.rgba());
}

TEST(GraphicsColor, ConstructorRgba)
{
  auto const c = color(0x32A852);
  auto const c2 = color(0x32A85280);

  EXPECT_EQ(color(0x32A852), color(0x32A852FF));
  EXPECT_EQ(c.rgb(), ref.rgb());
  EXPECT_EQ(c.red(), ref.red());
  EXPECT_EQ(c.green(), ref.green());
  EXPECT_EQ(c.blue(), ref.blue());
  EXPECT_EQ(c2.rgba(), ref.rgba());
  EXPECT_NE(c, c2);
  EXPECT_EQ(c2, ref);
}

TEST(GraphicsColor, String)
{
  EXPECT_EQ(color("32a85280"), ref);
  EXPECT_EQ(color("#32a85280"), ref);
  EXPECT_EQ(color("#32a852").rgb(), ref.rgb());
  EXPECT_EQ(color("32a852").rgb(), ref.rgb());
}

TEST(GraphicsColor, Hsl)
{
  EXPECT_EQ(color("32a85280").hsl(), color::hsl_t(136, 0.5412844F, 0.42745098F));
}

TEST(GraphicsColor, Hsla)
{
  EXPECT_EQ(color("32a85280").hsla(), color::hsla_t(136, 0.5412844F, 0.42745098F, 0.5019608F));
}

TEST(GraphicsColor, Hsv)
{
  EXPECT_EQ(color("32a85280").hsv(), color::hsv_t(136, 0.70238096F, 0.65882355F));
}

TEST(GraphicsColor, Hsva)
{
  EXPECT_EQ(color("32a85280").hsva(), color::hsva_t(136, 0.70238096, 0.65882355, 0.5019608F));
}

TEST(GraphicsColor, FromHsv)
{
  EXPECT_EQ(color::from_hsv({136, 0.70238096, 0.65882355}), color("32a851FF"));
}