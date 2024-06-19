#include <vector>
#include <numeric>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid.h>

using fl::math::size2d;
using fl::math::point2d;
using fl::math::default_unit;
using fl::math::scale;
using namespace fl::types;

struct Mm {};
struct Cm {};

TEST(EuclidPoint2D, Negation)
{
  EXPECT_EQ(-point2d(1.0, 2.0), point2d(-1.0, -2.0));
  EXPECT_EQ(-point2d(0.0, 0.0), point2d(-0.0, -0.0));
  EXPECT_EQ(-point2d(-1.0, -2.0), point2d(1.0, 2.0));
}

TEST(EuclidPoint2D, AddSize)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 + p2, point2d(4.0, 6.0));
}

TEST(EuclidPoint2D, AddAssignSize)
{
  auto p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 += p2, point2d(4.0, 6.0));
}

TEST(EuclidPoint2D, Subtract)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const p2 = point2d(3.0, 4.0);

  EXPECT_EQ(p1 - p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, SubtractAssign)
{
  auto p1 = point2d(1.0, 2.0);
  auto const p2 = point2d(3.0, 4.0);

  EXPECT_EQ(p1 -= p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, SubtractSize)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 - p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, SubtractAssignSize)
{
  auto p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 -= p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, MulScalar)
{
  auto const p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 * 2.0, point2d(6.0, 10.0));
  EXPECT_EQ(p1 * 5.0, point2d(15.0, 25.0));
  EXPECT_EQ(p1 * -1.0, point2d(-3.0, -5.0));
}

TEST(EuclidPoint2D, MulAssignScalar)
{
  auto p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 *= 2.0, point2d(6.0, 10.0));
  EXPECT_EQ(p1 *= 2.0, point2d(12.0, 20.0));
  EXPECT_EQ(p1 *= -0.5, point2d(-6.0, -10.0));
}

TEST(EuclidPoint2D, MulScale)
{
  auto const p1 = point2d<Mm>(1.0, 2.0);
  auto const cm_per_mm = scale<Mm, Cm>(0.1);

  EXPECT_EQ(p1 * cm_per_mm, point2d<Cm>(0.1, 0.2));
}

TEST(EuclidPoint2D, DivScalar)
{
  auto const p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 / 2.0, point2d(1.5, 2.5));
  EXPECT_EQ(p1 / 5.0, point2d(0.6, 1.0));
  EXPECT_EQ(p1 / -1.0, point2d(-3.0, -5.0));
}

TEST(EuclidPoint2D, DivAssignScalar)
{
  auto p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 /= 2.0, point2d(1.5, 2.5));
  EXPECT_EQ(p1 /= 2.0, point2d(0.75, 1.25));
}

TEST(EuclidPoint2D, DivScale)
{
  auto const p1 = point2d<Cm>(0.1, 0.2);
  auto const cm_per_mm = scale<Mm, Cm>(0.1);

  EXPECT_EQ(p1 / cm_per_mm, point2d<Mm>(1.0, 2.0));
}

TEST(EuclidPoint2D, Format)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const res = fmt::format("{}", p1);

  EXPECT_TRUE(res == "[1, 2]" or res == "[1.0, 2.0]");
}

/*
#[test] pub fn test_add_vec() { assert_eq!(Point2DMm::new(1.0, 2.0) + vec2(3.0, 4.0), Point2DMm::new(4.0, 6.0)); }
#[test] pub fn test_add_assign_vec() { assert_eq!(Point2DMm::new(1.0, 2.0) += vec2(3.0, 4.0), Point2DMm::new(4.0, 6.0)); }
#[test] pub fn test_sub_vec() { assert_eq!(Point2DMm::new(1.0, 2.0) - vec2(3.0, 4.0), Point2DMm::new(-2.0, -2.0)); }
#[test] pub fn test_sub_assign_vec() { assert_eq!(Point2DMm::new(1.0, 2.0) -= vec2(3.0, 4.0), Point2DMm::new(-2.0, -2.0)); }


#[test]
pub fn test_point_debug_formatting() {
let n = 1.23456789;
let p1 = Point2D::new(n, -n);
let should_be = format!("({:.4}, {:.4})", n, -n);

let got = format!("{:.4?}", p1);

assert_eq!(got, should_be);
}
}

mod euclid {
use crate::default::{Point2D, Size2D};
use crate::point2;

#[test]
pub fn test_rem_euclid() {
let p = Point2D::new(7.0, -7.0);
let s = Size2D::new(4.0, -4.0);

assert_eq!(p.rem_euclid(&s), point2(3.0, 1.0));
assert_eq!((-p).rem_euclid(&s), point2(1.0, 3.0));
assert_eq!(p.rem_euclid(&-s), point2(3.0, 1.0));
}

#[test]
pub fn test_div_euclid() {
let p = Point2D::new(7.0, -7.0);
let s = Size2D::new(4.0, -4.0);

assert_eq!(p.div_euclid(&s), point2(1.0, 2.0));
assert_eq!((-p).div_euclid(&s), point2(-2.0, -1.0));
assert_eq!(p.div_euclid(&-s), point2(-1.0, -2.0));
}
}
}

#[cfg(test)]
mod point3d {
use crate::default;
use crate::default::Point3D;
use crate::{point2, point3};
#[cfg(feature = "mint")]
use mint;

#[test]
pub fn test_min() {
let p1 = Point3D::new(1.0, 3.0, 5.0);
let p2 = Point3D::new(2.0, 2.0, -1.0);

let result = p1.min(p2);

assert_eq!(result, Point3D::new(1.0, 2.0, -1.0));
}

#[test]
pub fn test_max() {
let p1 = Point3D::new(1.0, 3.0, 5.0);
let p2 = Point3D::new(2.0, 2.0, -1.0);

let result = p1.max(p2);

assert_eq!(result, Point3D::new(2.0, 3.0, 5.0));
}

#[test]
pub fn test_conv_vector() {
use crate::point3;
for i in 0..100 {
    // We don't care about these values as long as they are not the same.
    let x = i as f32 * 0.012345;
    let y = i as f32 * 0.987654;
    let z = x * y;
    let p: Point3D<f32> = point3(x, y, z);
    assert_eq!(p.to_vector().to_point(), p);
  }
}

#[test]
pub fn test_swizzling() {
let p: default::Point3D<i32> = point3(1, 2, 3);
assert_eq!(p.xy(), point2(1, 2));
assert_eq!(p.xz(), point2(1, 3));
assert_eq!(p.yz(), point2(2, 3));
}

#[test]
pub fn test_distance_to() {
let p1 = Point3D::new(1.0, 2.0, 3.0);
let p2 = Point3D::new(2.0, 2.0, 3.0);

assert_eq!(p1.distance_to(p2), 1.0);

let p1 = Point3D::new(1.0, 2.0, 3.0);
let p2 = Point3D::new(1.0, 4.0, 3.0);

assert_eq!(p1.distance_to(p2), 2.0);

let p1 = Point3D::new(1.0, 2.0, 3.0);
let p2 = Point3D::new(1.0, 2.0, 6.0);

assert_eq!(p1.distance_to(p2), 3.0);
}

#[cfg(feature = "mint")]
#[test]
pub fn test_mint() {
let p1 = Point3D::new(1.0, 3.0, 5.0);
let pm: mint::Point3<_> = p1.into();
let p2 = Point3D::from(pm);

assert_eq!(p1, p2);
}

mod ops {
use crate::default::Point3D;
use crate::scale::Scale;
use crate::{size3, vec3, Vector3D};

pub enum Mm {}
pub enum Cm {}

pub type Point3DMm<T> = crate::Point3D<T, Mm>;
pub type Point3DCm<T> = crate::Point3D<T, Cm>;

#[test]
pub fn test_neg() {
assert_eq!(-Point3D::new(1.0, 2.0, 3.0), Point3D::new(-1.0, -2.0, -3.0));
assert_eq!(-Point3D::new(0.0, 0.0, 0.0), Point3D::new(-0.0, -0.0, -0.0));
assert_eq!(-Point3D::new(-1.0, -2.0, -3.0), Point3D::new(1.0, 2.0, 3.0));
}

#[test]
pub fn test_add_size() {
let p1 = Point3DMm::new(1.0, 2.0, 3.0);
let p2 = size3(4.0, 5.0, 6.0);

let result = p1 + p2;

assert_eq!(result, Point3DMm::new(5.0, 7.0, 9.0));
}

#[test]
pub fn test_add_assign_size() {
let mut p1 = Point3DMm::new(1.0, 2.0, 3.0);

p1 += size3(4.0, 5.0, 6.0);

assert_eq!(p1, Point3DMm::new(5.0, 7.0, 9.0));
}

#[test]
pub fn test_add_vec() {
let p1 = Point3DMm::new(1.0, 2.0, 3.0);
let p2 = vec3(4.0, 5.0, 6.0);

let result = p1 + p2;

assert_eq!(result, Point3DMm::new(5.0, 7.0, 9.0));
}

#[test]
pub fn test_add_assign_vec() {
let mut p1 = Point3DMm::new(1.0, 2.0, 3.0);

p1 += vec3(4.0, 5.0, 6.0);

assert_eq!(p1, Point3DMm::new(5.0, 7.0, 9.0));
}

#[test]
pub fn test_sub() {
let p1 = Point3DMm::new(1.0, 2.0, 3.0);
let p2 = Point3DMm::new(4.0, 5.0, 6.0);

let result = p1 - p2;

assert_eq!(result, Vector3D::<_, Mm>::new(-3.0, -3.0, -3.0));
}

#[test]
pub fn test_sub_size() {
let p1 = Point3DMm::new(1.0, 2.0, 3.0);
let p2 = size3(4.0, 5.0, 6.0);

let result = p1 - p2;

assert_eq!(result, Point3DMm::new(-3.0, -3.0, -3.0));
}

#[test]
pub fn test_sub_assign_size() {
let mut p1 = Point3DMm::new(1.0, 2.0, 3.0);

p1 -= size3(4.0, 5.0, 6.0);

assert_eq!(p1, Point3DMm::new(-3.0, -3.0, -3.0));
}

#[test]
pub fn test_sub_vec() {
let p1 = Point3DMm::new(1.0, 2.0, 3.0);
let p2 = vec3(4.0, 5.0, 6.0);

let result = p1 - p2;

assert_eq!(result, Point3DMm::new(-3.0, -3.0, -3.0));
}

#[test]
pub fn test_sub_assign_vec() {
let mut p1 = Point3DMm::new(1.0, 2.0, 3.0);

p1 -= vec3(4.0, 5.0, 6.0);

assert_eq!(p1, Point3DMm::new(-3.0, -3.0, -3.0));
}

#[test]
pub fn test_mul_scalar() {
let p1: Point3D<f32> = Point3D::new(3.0, 5.0, 7.0);

let result = p1 * 5.0;

assert_eq!(result, Point3D::new(15.0, 25.0, 35.0));
}

#[test]
pub fn test_mul_assign_scalar() {
let mut p1: Point3D<f32> = Point3D::new(3.0, 5.0, 7.0);

p1 *= 5.0;

assert_eq!(p1, Point3D::new(15.0, 25.0, 35.0));
}

#[test]
pub fn test_mul_scale() {
let p1 = Point3DMm::new(1.0, 2.0, 3.0);
let cm_per_mm: Scale<f32, Mm, Cm> = Scale::new(0.1);

let result = p1 * cm_per_mm;

assert_eq!(result, Point3DCm::new(0.1, 0.2, 0.3));
}

#[test]
pub fn test_mul_assign_scale() {
let mut p1 = Point3DMm::new(1.0, 2.0, 3.0);
let scale: Scale<f32, Mm, Mm> = Scale::new(0.1);

p1 *= scale;

assert_eq!(p1, Point3DMm::new(0.1, 0.2, 0.3));
}

#[test]
pub fn test_div_scalar() {
let p1: Point3D<f32> = Point3D::new(15.0, 25.0, 35.0);

let result = p1 / 5.0;

assert_eq!(result, Point3D::new(3.0, 5.0, 7.0));
}

#[test]
pub fn test_div_assign_scalar() {
let mut p1: Point3D<f32> = Point3D::new(15.0, 25.0, 35.0);

p1 /= 5.0;

assert_eq!(p1, Point3D::new(3.0, 5.0, 7.0));
}

#[test]
pub fn test_div_scale() {
let p1 = Point3DCm::new(0.1, 0.2, 0.3);
let cm_per_mm: Scale<f32, Mm, Cm> = Scale::new(0.1);

let result = p1 / cm_per_mm;

assert_eq!(result, Point3DMm::new(1.0, 2.0, 3.0));
}

#[test]
pub fn test_div_assign_scale() {
let mut p1 = Point3DMm::new(0.1, 0.2, 0.3);
let scale: Scale<f32, Mm, Mm> = Scale::new(0.1);

p1 /= scale;

assert_eq!(p1, Point3DMm::new(1.0, 2.0, 3.0));
}
}

mod euclid {
use crate::default::{Point3D, Size3D};
use crate::point3;

#[test]
pub fn test_rem_euclid() {
let p = Point3D::new(7.0, -7.0, 0.0);
let s = Size3D::new(4.0, -4.0, 12.0);

assert_eq!(p.rem_euclid(&s), point3(3.0, 1.0, 0.0));
assert_eq!((-p).rem_euclid(&s), point3(1.0, 3.0, 0.0));
assert_eq!(p.rem_euclid(&-s), point3(3.0, 1.0, 0.0));
}

#[test]
pub fn test_div_euclid() {
let p = Point3D::new(7.0, -7.0, 0.0);
let s = Size3D::new(4.0, -4.0, 12.0);

assert_eq!(p.div_euclid(&s), point3(1.0, 2.0, 0.0));
assert_eq!((-p).div_euclid(&s), point3(-2.0, -1.0, 0.0));
assert_eq!(p.div_euclid(&-s), point3(-1.0, -2.0, 0.0));
}
}
 */