#include <vector>
#include <numeric>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid.h>

using fl::math::size2d;
using fl::math::default_unit;
using fl::math::scale;
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

TEST(EuclidSize2D, FromTuple)
{
  auto const p = size2d<default_unit, f64>::from_tuple(std::tuple<double, double>{1.5, 2.0});

  EXPECT_EQ(p.x(), 1.5);
  EXPECT_EQ(p.y(), 2.0);
  EXPECT_EQ(p.area(), 3.0);
}

TEST(EuclidSize2D, Negation)
{
  EXPECT_EQ(-size2d(1.0, 2.0), size2d(-1.0, -2.0));
  EXPECT_EQ(-size2d(0.0, 0.0), size2d(-0.0, -0.0));
  EXPECT_EQ(-size2d(-1.0, -2.0), size2d(1.0, 2.0));
}

TEST(EuclidSize2D, Add)
{
  EXPECT_EQ(size2d(1.0, 2.0) + size2d(3.0, 4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) + size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) + size2d(-3.0, -4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(0.0, 0.0) + size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, AddAssign)
{
  EXPECT_EQ(size2d(1.0, 2.0) += size2d(3.0, 4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) += size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) += size2d(-3.0, -4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(0.0, 0.0) += size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, Sum)
{
  auto vec = std::vector<size2d<default_unit, f32>>{size2d(1.0F, 2.0F), size2d(3.0F, 4.0F), size2d(5.0F, 6.0F)};
  auto acc = std::accumulate(vec.begin(), vec.end(), size2d());

  EXPECT_EQ(acc, size2d(9.0F, 12.0F));
}

TEST(EuclidSize2D, Subtract)
{
  EXPECT_EQ(size2d(1.0, 2.0) - size2d(3.0, 4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(1.0, 2.0) - size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) - size2d(-3.0, -4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(0.0, 0.0) - size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, SubtractAssign)
{
  EXPECT_EQ(size2d(1.0, 2.0) -= size2d(3.0, 4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(1.0, 2.0) -= size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) -= size2d(-3.0, -4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(0.0, 0.0) -= size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, MultiplyByScalar)
{
  EXPECT_EQ(size2d(1.0, 2.0) * 3.0, size2d(3.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) * 0.0, size2d(0.0, 0.0));
  EXPECT_EQ(size2d(1.0, 2.0) * -3.0, size2d(-3.0, -6.0));
  EXPECT_EQ(size2d(0.0, 0.0) * 0.0, size2d(0.0, 0.0));
}

TEST(EuclidSize2D, MultiplyAssignByScalar)
{
  EXPECT_EQ(size2d(1.0, 2.0) *= 3.0, size2d(3.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) *= 0.0, size2d(0.0, 0.0));
  EXPECT_EQ(size2d(1.0, 2.0) *= -3.0, size2d(-3.0, -6.0));
  EXPECT_EQ(size2d(0.0, 0.0) *= 0.0, size2d(0.0, 0.0));
}

TEST(EuclidSize2D, MultiplyByScale)
{
  auto const sz = size2d<Mm>(1.0, 2.0);
  auto const cm_per_mm = scale<Mm, Cm>(0.1);
  EXPECT_EQ(sz * cm_per_mm, size2d<Cm>(0.1, 0.2));
}
/*
 * #[test]
pub fn test_div_scalar() {
    let s1: Size2D<f32> = Size2D::new(15.0, 25.0);

let result = s1 / 5.0;

assert_eq!(result, Size2D::new(3.0, 5.0));
}

#[test]
pub fn test_div_assign_scalar() {
let mut s1: Size2D<f32> = Size2D::new(15.0, 25.0);

s1 /= 5.0;

assert_eq!(s1, Size2D::new(3.0, 5.0));
}

#[test]
pub fn test_div_scale() {
let s1 = Size2DCm::new(0.1, 0.2);
let cm_per_mm: Scale<f32, Mm, Cm> = Scale::new(0.1);

let result = s1 / cm_per_mm;

assert_eq!(result, Size2DMm::new(1.0, 2.0));
}

#[test]
pub fn test_div_assign_scale() {
let mut s1 = Size2DMm::new(0.1, 0.2);
let scale: Scale<f32, Mm, Mm> = Scale::new(0.1);

s1 /= scale;

assert_eq!(s1, Size2DMm::new(1.0, 2.0));
}

#[test]
pub fn test_nan_empty() {
use std::f32::NAN;
assert!(Size2D::new(NAN, 2.0).is_empty());
assert!(Size2D::new(0.0, NAN).is_empty());
assert!(Size2D::new(NAN, -2.0).is_empty());
}
 */