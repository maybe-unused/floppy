/// \file floppy/euclid/point2d.h
/// \brief A two-dimensional point tagged with a unit.
/// \author whs31

#pragma once

#include <array>
#include <tuple>
#include <functional>
#include <floppy/euclid/length.h>

// todo: to qpoint
// todo: to qpointf

namespace floppy::math
{
  /// \brief A two-dimensional point tagged with a unit.
  /// \headerfile floppy/euclid/point2d.h
  /// \tparam U Associated unit of measurement. Default is \ref default_unit.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  /// \see floppy::math::length
  /// \see floppy::math::size2d
  /// \see floppy::math::vector2d
  template <typename U = default_unit, concepts::num T = f32>
  class point2d : public traits::formattable<point2d<U, T>, char>
  {
   public:
    /// \brief Associated unit type.
    using unit = U;

    /// \brief Underlying number type.
    using underlying_type = T;

    /// \brief Constructs new point with zero coordinates.
    constexpr point2d()
      : x_(static_cast<underlying_type>(0))
      , y_(static_cast<underlying_type>(0))
    {}

    /// \brief Constructs new point from given scalar values.
    /// \param x The x-coordinate.
    /// \param y The y-coordinate.
    constexpr point2d(underlying_type x, underlying_type y)
      : x_(x)
      , y_(y)
    {}

    /// \brief Constructs new point from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    constexpr point2d(length<unit> x, length<unit> y)
      : x_(x.value())
      , y_(y.value())
    {}

    /// \brief Constructs new point, setting all components to the same value.
    /// \param value The value to set all components to.
    constexpr explicit point2d(underlying_type value)
      : x_(value)
      , y_(value)
    {}

    /// \brief Tags a unitless value with units.
    /// \param p Unitless point2d
    constexpr explicit point2d(point2d<default_unit, T> const& p)
      : x_(p.x_)
      , y_(p.y_)
    {}

    /// \brief Default copy constructor.
    constexpr point2d(point2d const&) = default;

    /// \brief Default move constructor.
    constexpr point2d(point2d&&) = default;

    /// \brief Default destructor.
    ~point2d() = default;

    /// \brief Returns the x-coordinate of the point as scalar value.
    /// \return The x-coordinate of the point.
    [[nodiscard]] constexpr auto x() const -> underlying_type { return this->x_; }

    /// \brief Returns the y-coordinate of the point as scalar value.
    /// \return The y-coordinate of the point.
    [[nodiscard]] constexpr auto y() const -> underlying_type { return this->y_; }

    /// \brief Returns the x-coordinate of the point as length value with units.
    /// \return The x-coordinate of the point.
    [[nodiscard]] constexpr auto lx() const -> length<unit> { return length<unit>(this->x_); }

    /// \brief Returns the y-coordinate of the point as length value with units.
    /// \return The y-coordinate of the point.
    [[nodiscard]] constexpr auto ly() const -> length<unit> { return length<unit>(this->y_); }

    /// \brief Returns string representation of the point2d.
    /// \details Point is represented as it's numeric values. If the underlying number type is floating
    /// point, it is rounded to two decimal places.
    /// \note Due to limitations of the language, units are not displayed.
    /// \return String representation of the object.
    [[nodiscard]] virtual auto to_string() const -> std::string override {
      if constexpr(std::is_floating_point_v<T>)
        return fmt::format("[{:.2f}, {:.2f}]", this->x_, this->y_);
      else
        return fmt::format("[{}, {}]", this->x_, this->y_);
    }

    /// \brief Applies the function <b>fn</b> to each component of the point.
    /// \tparam U2 The new point2d's numeric scalar type.
    /// \param fn The function to apply.
    template <concepts::num U2>
    constexpr auto map(std::function<U2(U)> fn) const { return point2d<unit, U2>(fn(x_), fn(y_)); }

    /// \brief Constructs new point with zero coordinates.
    [[nodiscard]] static constexpr auto origin() -> point2d { return point2d(); }

    /// \brief The same as <i>origin</i>.
    [[nodiscard]] static constexpr auto zero() -> point2d { return point2d(); }

    // todo: zip https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.zip
    // todo: extend https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.extend
    // todo: to_vector https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.to_vector

    /// \brief Drops the units from the point, returning just the numeric scalar values.
    [[nodiscard]] constexpr auto to_untyped() const -> point2d<default_unit, underlying_type> {
      return point2d<default_unit, underlying_type>(this->x_, this->y_);
    }

    /// \brief Casts the unit of measurement.
    /// \tparam U2 New unit of measurement.
    /// \return The point2d with the new unit of measurement and the same value.
    template <typename U2>
    [[nodiscard]] constexpr auto cast_unit() const -> point2d<U2, underlying_type> {
      return point2d<U2, underlying_type>(this->x_, this->y_);
    }

    /// \brief Cast from one numeric representation to another, preserving the units.
    /// \tparam T2 New number type.
    /// \return The point2d with the new number type and the same value.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto cast() const -> point2d<unit, T2> {
      return point2d<unit, T2>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>f32</tt> point2d.
    [[nodiscard]] constexpr auto to_f32() const -> point2d<unit, f32> {
      return point2d<unit, f32>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>f64</tt> point2d.
    [[nodiscard]] constexpr auto to_f64() const -> point2d<unit, f64> {
      return point2d<unit, f64>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>u32</tt> point2d.
    [[nodiscard]] constexpr auto to_u32() const -> point2d<unit, u32> {
      return point2d<unit, u32>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>u64</tt> point2d.
    [[nodiscard]] constexpr auto to_u64() const -> point2d<unit, u64> {
      return point2d<unit, u64>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>usize</tt> point2d.
    [[nodiscard]] constexpr auto to_usize() const -> point2d<unit, usize> {
      return point2d<unit, usize>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>i32</tt> point2d.
    [[nodiscard]] constexpr auto to_i32() const -> point2d<unit, i32> {
      return point2d<unit, i32>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>i64</tt> point2d.
    [[nodiscard]] constexpr auto to_i64() const -> point2d<unit, i64> {
      return point2d<unit, i64>(this->x_, this->y_);
    }

    /// \brief Casts into <tt>isize</tt> point2d.
    [[nodiscard]] constexpr auto to_isize() const -> point2d<unit, isize> {
      return point2d<unit, isize>(this->x_, this->y_);
    }

    /// \brief Casts into an array of two values.
    [[nodiscard]] constexpr auto to_array() const -> std::array<underlying_type, 2> {
      return { this->x_, this->y_ };
    }

    /// \brief Casts into a tuple of two values.
    [[nodiscard]] constexpr auto to_tuple() const -> std::tuple<underlying_type, underlying_type> {
      return { this->x_, this->y_ };
    }

    // todo: to_3d https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.to_3d

    /// \brief Rounds each component to the nearest integer value.
    /// \details This behavior is preserved for negative values (unlike the basic cast).
    [[nodiscard]] constexpr auto round() const -> point2d<unit, underlying_type> {
      return point2d<unit, underlying_type>(std::round(this->x_), std::round(this->y_));
    }

    /// \brief Rounds each component to the smallest integer equal or greater than the original value.
    /// \details This behavior is preserved for negative values (unlike the basic cast).
    [[nodiscard]] constexpr auto ceil() const -> point2d<unit, underlying_type> {
      return point2d<unit, underlying_type>(std::ceil(this->x_), std::ceil(this->y_));
    }

    /// \brief Rounds each component to the largest integer equal or less than the original value.
    /// \details This behavior is preserved for negative values (unlike the basic cast).
    [[nodiscard]] constexpr auto floor() const -> point2d<unit, underlying_type> {
      return point2d<unit, underlying_type>(std::floor(this->x_), std::floor(this->y_));
    }

    /// \brief Linearly interpolate between this point and another point.
    /// \details Example:
    /// \code {.cpp}
    /// using floppy::math::point2d;
    /// auto const from = point2d(0.0F, 10.0F);
    /// auto const to = point2d(8.0F, -4.0F);
    /// fmt::println("{}", from.lerp(to, -1.0F));
    /// fmt::println("{}", from.lerp(to, 0.0F));
    /// fmt::println("{}", from.lerp(to, 0.5F));
    /// fmt::println("{}", from.lerp(to, 1.0F));
    /// fmt::println("{}", from.lerp(to, 2.0F));
    /// \endcode
    ///
    /// Output:
    /// \code {.sh}
    /// [-8.0, 24.0]
    /// [0.0, 10.0]
    /// [4.0, 3.0]
    /// [8.0, -4.0]
    /// [16.0, -18.0]
    /// \endcode
    /// \param other The other point.
    /// \param t Interpolation factor.
    /// \return The interpolated point.
    [[nodiscard]] constexpr auto lerp(point2d<U, T> const& other, T t) const -> point2d<U, T> {
      return point2d<U, T>(this->x_ + (other.x_ - this->x_) * t, this->y_ + (other.y_ - this->y_) * t);
    }

    /// \brief Returns the largest x and y values of this and another point.
    /// \param other The other point.
    /// \return Point with the largest x and y values.
    [[nodiscard]] constexpr auto max(point2d<U, T> const& other) const -> point2d<U, T> {
      return point2d<U, T>(std::max(this->x_, other.x_), std::max(this->y_, other.y_));
    }
    /// \brief Returns the smallest x and y values of this and another point.
    /// \param other The other point.
    /// \return Point with the smallest x and y values.
    [[nodiscard]] constexpr auto min(point2d<U, T> const& other) const -> point2d<U, T> {
      return point2d<U, T>(std::min(this->x_, other.x_), std::min(this->y_, other.y_));
    }

    /// \brief Returns the point each component of which clamped by corresponding components of start and end.
    /// \param start The start point.
    /// \param end The end point.
    /// \return The clamped point.
    [[nodiscard]] constexpr auto clamp(point2d<U, T> const& start, point2d<U, T> const& end) const -> point2d<U, T> {
      return point2d<U, T>(std::clamp(this->x_, start.x_, end.x_), std::clamp(this->y_, start.y_, end.y_));
    }

    /// \brief Returns true if all members are finite.
    [[nodiscard]] constexpr auto is_finite() const -> bool { return std::isfinite(this->x_) && std::isfinite(this->y_); }

    // todo add_size https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.add_size

    /// \brief Returns distance between this and another point.
    /// \param other The other point.
    /// \return The distance.
    [[nodiscard]] constexpr auto distance_to(point2d<U, T> const& other) const -> length<unit> {
      return length<unit>(std::hypot(this->x_ - other.x_, this->y_ - other.y_));
    }

    /// \brief Calculates the least nonnegative remainder of this (mod other)
    /// \param other The divisor.
    /// \return The remainder.
    /// \see div_euclid
    [[nodiscard]] constexpr auto rem_euclid(point2d<U, T> const& other) const -> point2d<U, T> {
      if constexpr(std::is_integral_v<T>)
        return point2d<U, T>(this->x_ % other.x_, this->y_ % other.y_);
      else
        return point2d<U, T>(std::fmod(this->x_, other.x_), std::fmod(this->y_, other.y_));
    }

    /// \brief Calculates Euclidean division, the matching method for rem_euclid.
    /// \param other The divisor.
    /// \return The quotient.
    /// \see rem_euclid
    [[nodiscard]] constexpr auto div_euclid(point2d<U, T> const& other) const -> point2d<U, T> {
      return point2d<U, T>(this->x_ / other.x_, this->y_ / other.y_);
    }
    // todo: same two methods for size2d

    // todo: operators

    constexpr auto operator=(point2d const&) -> point2d& = default;
    constexpr auto operator=(point2d&&) -> point2d& = default;

    /// \brief Constructs new point from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    [[nodiscard]] static constexpr auto from_lengths(length<unit> x, length<unit> y) -> point2d { return point2d(x, y); }

    /// \brief Constructs new point, setting all components to the same value.
    /// \param value The value to set all components to.
    [[nodiscard]] static constexpr auto splat(underlying_type value) -> point2d { return point2d(value); }

   private:
    T x_;
    T y_;
  };
} // namespace floppy::math