/// \file floppy/euclid/point2d.h
/// \brief A two-dimensional point tagged with a unit.
/// \author whs31

#pragma once

#include <array>
#include <tuple>
#include <functional>
#include <floppy/euclid/length.h>
#include <floppy/euclid/detail/nt_traits2d.h>

// todo: to qpoint
// todo: to qpointf

namespace floppy::math
{
  /// \brief A two-dimensional point tagged with a unit.
  /// \headerfile floppy/euclid.h
  /// \tparam U Associated unit of measurement. Default is \ref default_unit.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  /// \see floppy::math::length
  /// \see floppy::math::size2d
  /// \see floppy::math::vector2d
  template <typename U = default_unit, concepts::num T = f32>
  class point2d : public detail::basic_two_dimensional_type<point2d<U, T>, U, T>
  {
   public:
    /// \brief Associated unit type.
    using unit = U;

    /// \brief Underlying number type.
    using underlying_type = T;

    /// \brief Constructs new point with zero coordinates.
    constexpr point2d() : detail::basic_two_dimensional_type<point2d<U, T>, U, T>() {}

    /// \brief Constructs new point from given scalar values.
    /// \param x The x-coordinate.
    /// \param y The y-coordinate.
    constexpr point2d(underlying_type x, underlying_type y) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new point from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    constexpr point2d(length<unit> x, length<unit> y) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new point, setting all components to the same value.
    /// \param value The value to set all components to.
    constexpr explicit point2d(underlying_type value) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(value) {}

    /// \brief Tags a unitless value with units.
    /// \param p Unitless point2d
    constexpr explicit point2d(point2d<default_unit, T> const& p) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(p.x(), p.y()){}

    /// \brief Applies the function <b>fn</b> to each component of the point.
    /// \tparam U2 The new point2d's numeric scalar type.
    /// \param fn The function to apply.
    template <concepts::num U2>
    constexpr auto map(std::function<U2(U)> fn) const { return point2d<unit, U2>(fn(this->x()), fn(this->y())); }

    // todo: zip https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.zip
    // todo: extend https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.extend
    // todo: to_vector https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.to_vector

    /// \brief Drops the units from the point, returning just the numeric scalar values.
    [[nodiscard]] constexpr auto to_untyped() const -> point2d<default_unit, underlying_type> {
      return point2d<default_unit, underlying_type>(this->x(), this->y());
    }

    /// \brief Casts the unit of measurement.
    /// \tparam U2 New unit of measurement.
    /// \return The point2d with the new unit of measurement and the same value.
    template <typename U2>
    [[nodiscard]] constexpr auto cast_unit() const -> point2d<U2, underlying_type> {
      return point2d<U2, underlying_type>(this->x(), this->y());
    }

    /// \brief Cast from one numeric representation to another, preserving the units.
    /// \tparam T2 New number type.
    /// \return The point2d with the new number type and the same value.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto cast() const -> point2d<unit, T2> {
      return point2d<unit, T2>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f32</tt> point2d.
    [[nodiscard]] constexpr auto to_f32() const -> point2d<unit, f32> {
      return point2d<unit, f32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f64</tt> point2d.
    [[nodiscard]] constexpr auto to_f64() const -> point2d<unit, f64> {
      return point2d<unit, f64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u32</tt> point2d.
    [[nodiscard]] constexpr auto to_u32() const -> point2d<unit, u32> {
      return point2d<unit, u32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u64</tt> point2d.
    [[nodiscard]] constexpr auto to_u64() const -> point2d<unit, u64> {
      return point2d<unit, u64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>usize</tt> point2d.
    [[nodiscard]] constexpr auto to_usize() const -> point2d<unit, usize> {
      return point2d<unit, usize>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i32</tt> point2d.
    [[nodiscard]] constexpr auto to_i32() const -> point2d<unit, i32> {
      return point2d<unit, i32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i64</tt> point2d.
    [[nodiscard]] constexpr auto to_i64() const -> point2d<unit, i64> {
      return point2d<unit, i64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>isize</tt> point2d.
    [[nodiscard]] constexpr auto to_isize() const -> point2d<unit, isize> {
      return point2d<unit, isize>(this->x(), this->y());
    }

    // todo: to_3d https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.to_3d
    // todo add_size https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.add_size

    /// \brief Returns distance between this and another point.
    /// \param other The other point.
    /// \return The distance.
    [[nodiscard]] constexpr auto distance_to(point2d<U, T> const& other) const -> length<unit> {
      return length<unit>(std::hypot(this->x() - other.x(), this->y() - other.y()));
    }

    /// \brief Calculates the least nonnegative remainder of this (mod other)
    /// \param other The divisor.
    /// \return The remainder.
    /// \see div_euclid
    [[nodiscard]] constexpr auto rem_euclid(point2d<U, T> const& other) const -> point2d<U, T> {
      if constexpr(std::is_integral_v<T>)
        return point2d<U, T>(this->x() % other.x(), this->y() % other.y());
      else
        return point2d<U, T>(std::fmod(this->x(), other.x()), std::fmod(this->y(), other.y()));
    }

    /// \brief Calculates Euclidean division, the matching method for rem_euclid.
    /// \param other The divisor.
    /// \return The quotient.
    /// \see rem_euclid
    [[nodiscard]] constexpr auto div_euclid(point2d<U, T> const& other) const -> point2d<U, T> {
      return point2d<U, T>(this->x() / other.x(), this->y() / other.y());
    }
    // todo: same two methods for size2d
    // todo: operators

    /// \brief Constructs new point with zero coordinates.
    [[nodiscard]] static constexpr auto origin() -> point2d { return point2d(); }
  };
} // namespace floppy::math