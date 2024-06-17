#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid/angle.h>

TEST(EuclidAngle, WrapAngles)
{
  using fl::math::angle;
  using namespace fl::types;

  auto constexpr PI = std::numbers::pi_v<f32>;
  auto constexpr FRAC_PI_2 = PI / 2.0f;

  EXPECT_EQ(angle<f32>::from_radians(0.0).positive(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(FRAC_PI_2).positive(), angle<f32>::half_pi());
  EXPECT_EQ(angle<f32>::from_radians(-FRAC_PI_2).positive(), angle<f32>::from_radians(3.0F * FRAC_PI_2));
  EXPECT_EQ(angle<f32>::from_radians(3.0F * FRAC_PI_2).positive(), angle<f32>::from_radians(3.0F * FRAC_PI_2));
  EXPECT_EQ(angle<f32>::from_radians(5.0F * FRAC_PI_2).positive(), angle<f32>::half_pi());
  EXPECT_EQ(angle<f32>::from_radians(2.0 * PI).positive(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(-2.0 * PI).positive(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(PI).positive(), angle<f32>::pi());
  EXPECT_EQ(angle<f32>::from_radians(-PI).positive(), angle<f32>::pi());
  EXPECT_EQ(angle<f32>::from_radians(FRAC_PI_2).signed_(), angle<f32>::half_pi());
  EXPECT_TRUE(std::abs(*angle<f32>::from_radians(5.0F * FRAC_PI_2).signed_() - *angle<f32>::half_pi()) < 0.0001F);
  EXPECT_TRUE(std::abs(*angle<f32>::from_radians(3.0F * FRAC_PI_2).signed_() + *angle<f32>::half_pi()) < 0.0001F);
  EXPECT_EQ(angle<f32>::from_radians(2.0 * PI).signed_(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(-2.0 * PI).signed_(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(PI).signed_(), angle<f32>::pi());
  EXPECT_EQ(angle<f32>::from_radians(-PI).signed_(), angle<f32>::pi());
}