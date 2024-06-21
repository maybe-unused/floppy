/// \file floppy/euclid/detail/nt_traits2d.h
/// \author whs31

#pragma once

#include <array>
#include <tuple>
#include <utility>
#include <algorithm>
#include <floppy/euclid/length.h>
#include <floppy/euclid/detail/nt_traits.h>

namespace floppy::math::detail
{
  /// \brief CRTP base class for two dimensional numerical types.
  /// \headerfile floppy/euclid.h
  /// \ingroup geometry_details
  /// \tparam T The base class.
  /// \tparam U The unit type.
  /// \tparam N The number type. Must satisfy <i>floppy::concepts::num</i>.
  /// \see floppy::math::point2d
  /// \see floppy::math::vector2d
  /// \see floppy::math::size2d
  template <typename T, typename U, concepts::num N>
  struct basic_two_dimensional_type : public traits::formattable<T, char>
  {
    /// \brief Associated unit type.
    using unit = U;

    /// \brief Underlying number type.
    using underlying_type = N;

    /// \brief Constructs new basic_two_dimensional_type with zero coordinates.
    constexpr basic_two_dimensional_type()
      : x_(static_cast<underlying_type>(0))
      , y_(static_cast<underlying_type>(0))
    {}

    /// \brief Constructs new basic_two_dimensional_type from given scalar values.
    /// \param x The x-coordinate.
    /// \param y The y-coordinate.
    constexpr basic_two_dimensional_type(underlying_type x, underlying_type y)
      : x_(x)
      , y_(y)
    {}

    /// \brief Constructs new basic_two_dimensional_type from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    constexpr basic_two_dimensional_type(length<unit> x, length<unit> y)
      : x_(x.value())
      , y_(y.value())
    {}

    /// \brief Constructs new basic_two_dimensional_type, setting all components to the same value.
    /// \param value The value to set all components to.
    constexpr explicit basic_two_dimensional_type(underlying_type value)
      : x_(value)
      , y_(value)
    {}

    /// \brief Returns inverted basic_two_dimensional_type.
    /// \return Inverted basic_two_dimensional_type.
    /// \see yx
    [[nodiscard]] constexpr auto inverted() const -> T { return T(this->y_, this->x_); }

    /// \brief Same as <i>inverted</i>.
    /// \return Inverted basic_two_dimensional_type.
    /// \see inverted
    [[nodiscard]] constexpr auto yx() const -> T { return this->inverted(); }

    /// \brief Returns the x-coordinate of the basic_two_dimensional_type as scalar value.
    /// \return The x-coordinate of the basic_two_dimensional_type.
    [[nodiscard]] constexpr auto x() const -> underlying_type { return this->x_; }

    /// \brief Returns the y-coordinate of the basic_two_dimensional_type as scalar value.
    /// \return The y-coordinate of the basic_two_dimensional_type.
    [[nodiscard]] constexpr auto y() const -> underlying_type { return this->y_; }

    /// \brief Returns mutable reference to the x-coordinate of the basic_two_dimensional_type as scalar value.
    /// \return Mutable reference to the x-coordinate of the basic_two_dimensional_type.
    [[nodiscard]] constexpr auto x_mut() -> underlying_type& { return this->x_; }

    /// \brief Returns mutable reference to the y-coordinate of the basic_two_dimensional_type as scalar value.
    /// \return Mutable reference to the y-coordinate of the basic_two_dimensional_type.
    [[nodiscard]] constexpr auto y_mut() -> underlying_type& { return this->y_; }

    /// \brief Returns the x-coordinate of the basic_two_dimensional_type as length value with units.
    /// \return The x-coordinate of the basic_two_dimensional_type.
    [[nodiscard]] constexpr auto lx() const -> length<unit> { return length<unit>(this->x_); }

    /// \brief Returns the y-coordinate of the basic_two_dimensional_type as length value with units.
    /// \return The y-coordinate of the basic_two_dimensional_type.
    [[nodiscard]] constexpr auto ly() const -> length<unit> { return length<unit>(this->y_); }

    /// \brief Returns string representation of the basic_two_dimensional_type.
    /// \details Size is represented as it's numeric values. If the underlying number type is floating
    /// basic_two_dimensional_type, it is rounded to two decimal places.
    /// \note Due to limitations of the language, units are not displayed.
    /// \return String representation of the object.
    [[nodiscard]] virtual auto to_string() const -> std::string override {
      if constexpr(std::is_floating_point_v<T>)
        return fmt::format("[{:.2f}, {:.2f}]", this->x_, this->y_);
      else
        return fmt::format("[{}, {}]", this->x_, this->y_);
    }

    /// \brief Casts into an array of two values.
    [[nodiscard]] constexpr auto to_array() const -> std::array<underlying_type, 2> {
      return { this->x_, this->y_ };
    }

    /// \brief Casts into a tuple of two values.
    [[nodiscard]] constexpr auto to_tuple() const -> std::tuple<underlying_type, underlying_type> {
      return { this->x_, this->y_ };
    }

    /// \brief Casts into a pair of two values.
    [[nodiscard]] constexpr auto to_pair() const -> std::pair<underlying_type, underlying_type> {
      return { this->x_, this->y_ };
    }

    /// \brief Rounds each component to the nearest integer value.
    /// \details This behavior is preserved for negative values (unlike the basic cast).
    [[nodiscard]] constexpr auto round() const -> T {
      return T(std::round(this->x_), std::round(this->y_));
    }

    /// \brief Rounds each component to the smallest integer equal or greater than the original value.
    /// \details This behavior is preserved for negative values (unlike the basic cast).
    [[nodiscard]] constexpr auto ceil() const -> T {
      return T(std::ceil(this->x_), std::ceil(this->y_));
    }

    /// \brief Rounds each component to the largest integer equal or less than the original value.
    /// \details This behavior is preserved for negative values (unlike the basic cast).
    [[nodiscard]] constexpr auto floor() const -> T {
      return T(std::floor(this->x_), std::floor(this->y_));
    }

    /// \brief Linearly interpolate between this basic_two_dimensional_type and another basic_two_dimensional_type.
    /// \details Example:
    /// \code {.cpp}
    /// using floppy::math::basic_two_dimensional_type;
    /// auto const from = T(0.0F, 10.0F);
    /// auto const to = T(8.0F, -4.0F);
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
    /// \param other The other basic_two_dimensional_type.
    /// \param t Interpolation factor.
    /// \return The interpolated basic_two_dimensional_type.
    [[nodiscard]] constexpr auto lerp(T const& other, T t) const -> T {
      return T(this->x_ + (other.x_ - this->x_) * t, this->y_ + (other.y_ - this->y_) * t);
    }

    /// \brief Returns the largest x and y values of this and another basic_two_dimensional_type.
    /// \param other The other basic_two_dimensional_type.
    /// \return basic_two_dimensional_type with the largest x and y values.
    [[nodiscard]] constexpr auto max(T const& other) const -> T {
      return T(std::max(this->x_, other.x_), std::max(this->y_, other.y_));
    }
    /// \brief Returns the smallest x and y values of this and another basic_two_dimensional_type.
    /// \param other The other basic_two_dimensional_type.
    /// \return basic_two_dimensional_type with the smallest x and y values.
    [[nodiscard]] constexpr  auto min(T const& other) const -> T {
      return T(std::min(this->x_, other.x_), std::min(this->y_, other.y_));
    }

    /// \brief Returns the basic_two_dimensional_type each component of which clamped by corresponding components of start and end.
    /// \param start The start basic_two_dimensional_type.
    /// \param end The end basic_two_dimensional_type.
    /// \return The clamped basic_two_dimensional_type.
    [[nodiscard]] constexpr auto clamp(T const& start, T const& end) const -> T {
      return T(std::clamp(this->x_, start.x_, end.x_), std::clamp(this->y_, start.y_, end.y_));
    }

    /// \brief Returns the absolute value of each component.
    /// \return The absolute value of each component.
    [[nodiscard]] constexpr auto abs() const -> T { return T(std::abs(this->x_), std::abs(this->y_)); }

    /// \brief Returns dot product of this and another basic_two_dimensional_type.
    /// \param other The other basic_two_dimensional_type.
    /// \return The dot product of this and another basic_two_dimensional_type.
    [[nodiscard]] constexpr auto dot(T const& other) const -> underlying_type {
      return this->x_ * other.x_ + this->y_ * other.y_;
    }

    /// \brief Returns the norm of the cross product of this and another basic_two_dimensional_type.
    /// \details Cross product is defined as <code>[x1, y1] x [x2, y2] = x1 * y2 - y1 * x2</code>.
    /// \param other The other basic_two_dimensional_type.
    /// \return The norm of the cross product of this and another basic_two_dimensional_type.
    [[nodiscard]] constexpr auto cross(T const& other) const -> underlying_type {
      return this->x_ * other.y_ - this->y_ * other.x_;
    }

    /// \brief Returns true if all members are finite.
    [[nodiscard]] constexpr auto is_finite() const -> bool { return std::isfinite(this->x_) && std::isfinite(this->y_); }

    /// \brief Returns true if all members are positive.
    [[nodiscard]] constexpr auto is_positive() const -> bool { return this->x_ > 0.0F and this->y_ > 0.0F; }

    /// \brief The same as <i>origin</i>.
    [[nodiscard]] static constexpr auto zero() -> T { return T(); }

    /// \brief Constructs new basic_two_dimensional_type from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    [[nodiscard]] static constexpr auto from_lengths(length<unit> x, length<unit> y) -> T { return T(x, y); }

    /// \brief Constructs new basic_two_dimensional_type, setting all components to the same value.
    /// \param value The value to set all components to.
    [[nodiscard]] static constexpr auto splat(underlying_type value) -> T { return T(value); }

    /// \brief Constructs new basic_two_dimensional_type from <tt>std::tuple</tt>.
    /// \param other The other <tt>std::tuple</tt>.
    template <typename... Args>
    requires (std::tuple_size_v<std::tuple<Args...>> == 2)
    [[nodiscard]] static constexpr auto from_tuple(std::tuple<Args...> const& other) -> T {
      return T(std::get<0>(other), std::get<1>(other));
    }

    /// \brief Constructs new basic_two_dimensional_type from <tt>std::array</tt>.
    /// \param other The other <tt>std::array</tt>.
    template <std::size_t N2>
    requires (N2 == 2)
    [[nodiscard]] static constexpr auto from_array(std::array<underlying_type, N2> const& other) -> T {
      return T(other[0], other[1]);
    }

    /// \brief Constructs new basic_two_dimensional_type from <tt>std::pair</tt>.
    /// \param other The other <tt>std::pair</tt>.
    [[nodiscard]] static constexpr auto from_pair(std::pair<underlying_type, underlying_type> const& other) -> T {
      return T(other.first, other.second);
    }

    /// \brief Returns <tt>true</tt> if underlying values is both not <tt>0</tt> and not <tt>NaN</tt> or <tt>Infinity</tt> in case of floating point.
    /// \note Also returns <tt>false
    [[nodiscard]] constexpr explicit operator bool() const {
      if constexpr(std::is_floating_point_v<underlying_type>) {
        if(not this->is_finite())
          return false;
        if(std::isnan(this->x_) or std::isnan(this->y_))
          return false;
      }
      return not is_null(this->x_) and not is_null(this->y_);
    }

    /// \brief Returns <tt>false</tt> if underlying values is both not <tt>0</tt>.
    [[nodiscard]] constexpr auto operator!() const -> bool { return not this->operator bool(); }

   protected:
    underlying_type x_;
    underlying_type y_;
  };
} // namespace floppy::math::detail