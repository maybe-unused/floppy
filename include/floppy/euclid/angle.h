/// \file floppy/euclid/angle.h
/// \brief Newtype describing an angle.
/// \author whs31

#pragma once

#include <floppy/detail/math.h>
#include <floppy/traits.h>
#include <floppy/euclid/detail/nt_traits.h>

namespace floppy::math
{
  /// \brief Newtype describing an angle.
  /// \headerfile floppy/euclid.h
  /// \ingroup geometry
  /// \details Stores an angle in radians.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  template <concepts::num T = f32>
  struct angle : public formattable<angle<T>, char>,
                 public detail::advanced_numeric_type<angle<T>, T>
  {
    /// \brief Constructs an empty angle.
    constexpr angle()
      : detail::advanced_numeric_type<angle<T>, T>()
    {}

    /// \brief Constructs an angle from radians.
    /// \param radians Angle in radians
    /// \see from_degrees, from_radians
    constexpr explicit angle(T radians)
      : detail::advanced_numeric_type<angle<T>, T>(radians)
    {}

    /// \brief Returns string representation of the object.
    /// \details Angle is represented in degrees.
    /// If the underlying number type is floating point, it is rounded to two decimal places.
    /// \return String representation of the object.
    [[nodiscard]] virtual auto to_string() const -> std::string override {
      if constexpr(std::is_floating_point_v<T>)
        return fmt::format("{:.2f}°", this->degrees());
      else
        return fmt::format("{}°", this->degrees());
    }

    /// \brief Returns this angle as number in radians.
    [[nodiscard]] constexpr auto radians() const -> T { return this->m_; }

    /// \brief Returns this angle as number in degrees.
    [[nodiscard]] constexpr auto degrees() const -> T { return to_degrees(this->m_); }

    /// \brief Returns this angle as number in range <code>[0, 2\pi)</code>.
    [[nodiscard]] constexpr auto positive() const -> angle {
      auto const tp = std::numbers::pi * T(2);
      auto a = math::fmod(this->m_, tp);
      if(a < T(0.0))
        a += tp;
      if(approx_eq(a, static_cast<T>(tp), T(3)))
        a = T(0.0);
      return angle::from_radians(a);
    }

    /// \brief Returns this angle as number in range <code>[-\pi, \pi)</code>.
    [[nodiscard]] constexpr auto signed_() const -> angle {
      auto const ret = angle::pi() - (angle::pi() - *this).positive();
      if(std::abs(ret.m_) <= 2.0F * std::numeric_limits<T>::epsilon())
        return angle::zero();
      return ret;
    }

    /// \brief Returns the shortest signed angle between two angles.
    /// \param other Other angle
    /// \note Takes wrapping and signs into account.
    [[nodiscard]] constexpr auto angle_to(angle const& other) const -> angle {
      auto const max = std::numbers::pi * T(2);
      auto const d = std::fmod(other.radians() - this->radians(), max);
      return angle::from_radians(std::fmod(T(2) * d, max) - d);
    }

    /// \brief Linear interpolation between two angles, using the shortest path.
    /// \param other Other angle
    /// \param t Interpolation factor
    [[nodiscard]] constexpr auto lerp(angle const& other, T t) const -> angle {
      auto const res = *this + this->angle_to(other) * t;
      // round to 3 decimal places
      if constexpr(std::is_floating_point_v<T>)
        return angle::from_radians(std::round(res.radians() * 1000.0) / 1000.0);
      else
        return res;
    }

    /// \brief Return <code>(sin(x), cos(x))</code>.
    [[nodiscard]] constexpr auto sin_cos() const -> std::pair<T, T> {
      return std::make_pair(std::sin(this->m_), std::cos(this->m_));
    }

    /// \brief Cast to a new underlying number type.
    /// \tparam U New number type
    /// \return Casted angle
    template <concepts::num U>
    [[nodiscard]] constexpr auto cast() const -> angle<U> { return angle<U>(static_cast<U>(this->m_)); }

    /// \brief Constructs an angle from radians.
    /// \param radians Angle in radians
    /// \see from_degrees
    [[nodiscard]] static constexpr auto from_radians(T radians) -> angle { return angle(radians); }

    /// \brief Constructs an angle from degrees.
    /// \param degrees Angle in degrees
    /// \see from_radians
    [[nodiscard]] static constexpr auto from_degrees(T degrees) -> angle { return angle(to_radians(degrees)); }

    /// \brief Constructs an zero angle.
    [[nodiscard]] static constexpr auto zero() -> angle { return angle(static_cast<T>(0.0)); }

    /// \brief Constructs an angle with value <code>pi</code>.
    [[nodiscard]] static constexpr auto pi() -> angle { return angle(std::numbers::pi); }

    /// \brief Constructs an angle with value <code>2pi</code>.
    [[nodiscard]] static constexpr auto two_pi() -> angle { return angle(T(2) * std::numbers::pi); }

    /// \brief Constructs an angle with value <code>1/2pi</code>.
    [[nodiscard]] static constexpr auto half_pi() -> angle { return angle(T(0.5) * std::numbers::pi); }

    /// \brief Constructs an angle with value <code>1/3pi</code>.
    [[nodiscard]] static constexpr auto third_pi() -> angle { return angle(std::numbers::pi / T(3)); }

    /// \brief Constructs an angle with value <code>1/4pi</code>.
    [[nodiscard]] static constexpr auto quarter_pi() -> angle { return angle(T(0.25) * std::numbers::pi); }
  };
} // namespace floppy::math