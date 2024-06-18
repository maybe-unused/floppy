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
  /// \headerfile floppy/euclid/angle.h
  /// \details Stores an angle in radians.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  template <concepts::num T = f32>
  struct angle : public traits::formattable<angle<T>, char>,
                 public detail::default_equality<angle<T>>
  {
    /// \brief Underlying number type.
    using type = T;

    /// \brief Constructs an angle from radians.
    /// \param radians Angle in radians
    /// \see from_degrees, from_radians
    constexpr angle(T radians)
      : m_(radians)
    {}

    /// \brief Default copy constructor.
    constexpr angle(angle const&) = default;

    /// \brief Default move constructor.
    constexpr angle(angle&&) = default;

    /// \brief Default destructor.
    ~angle() = default;

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

    /// \brief Returns this angle as number in range \f$[0, 2\pi)\f$.
    [[nodiscard]] constexpr auto positive() const -> angle {
      auto const tp = ::fl::math::pi<T>::value * T(2);
      auto a = std::fmod(this->m_, tp);
      if(a < T(0.0))
        a += tp;
      return angle::from_radians(a);
    }

    /// \brief Returns this angle as number in range \f$[-\pi, \pi)\f$.
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
      auto const max = ::fl::math::pi<T>::value * T(2);
      auto const d = std::fmod(other.radians() - this->radians(), max);
      return angle::from_radians(std::fmod(T(2) * d, max) - d);
    }

    /// \brief Linear interpolation between two angles, using the shortest path.
    /// \param other Other angle
    /// \param t Interpolation factor
    [[nodiscard]] constexpr auto lerp(angle const& other, T t) const -> angle {
      return *this + this->angle_to(other) * t;
    }

    /// \brief Returns true if the angle is a finite number.
    [[nodiscard]] constexpr auto is_finite() const -> bool { return ::std::isfinite(this->m_); }

    /// \brief Return \f$(sin(x), cos(x))\f$.
    [[nodiscard]] constexpr auto sin_cos() const -> std::pair<T, T> {
      return std::make_pair(std::sin(this->m_), std::cos(this->m_));
    }

    /// \brief Casts angle from one numeric representation to another.
    template <concepts::num U>
    [[nodiscard]] constexpr auto cast() const -> angle<U> { return { static_cast<U>(this->m_) }; }

    /// \brief Returns this angle as number in another numeric representation.
    template <concepts::num U>
    [[nodiscard]] constexpr auto as() const -> U { return static_cast<U>(this->m_); }

    /// \brief Returns this angle as 32-bit floating point number.
    [[maybe_unused]] [[nodiscard]] constexpr auto as_f32() const -> T { return this->as<f32>(); }

    /// \brief Returns this angle as 64-bit floating point number.
    [[maybe_unused]] [[nodiscard]] constexpr auto as_f64() const -> T { return this->as<f64>(); }

    /// \brief Constructs an angle from radians.
    /// \param radians Angle in radians
    /// \see from_degrees
    [[nodiscard]] static constexpr auto from_radians(T radians) -> angle { return { radians }; }

    /// \brief Constructs an angle from degrees.
    /// \param degrees Angle in degrees
    /// \see from_radians
    [[nodiscard]] static constexpr auto from_degrees(T degrees) -> angle { return { to_radians(degrees) }; }

    /// \brief Constructs an zero angle.
    [[nodiscard]] static constexpr auto zero() -> angle { return { static_cast<T>(0.0) }; }

    /// \brief Constructs an angle with value \f$\pi\f$.
    [[nodiscard]] static constexpr auto pi() -> angle { return { ::fl::math::pi<T>::value }; }

    /// \brief Constructs an angle with value \f$2\pi\f$.
    [[nodiscard]] static constexpr auto two_pi() -> angle { return { T(2) * ::fl::math::pi<T>::value }; }

    /// \brief Constructs an angle with value \f$\frac{\pi}{2}\f$.
    [[nodiscard]] static constexpr auto half_pi() -> angle { return { T(0.5) * ::fl::math::pi<T>::value }; }

    /// \brief Constructs an angle with value \f$\frac{\pi}{3}\f$.
    [[nodiscard]] static constexpr auto third_pi() -> angle { return { ::fl::math::pi<T>::value / T(3) } ; }

    /// \brief Constructs an angle with value \f$\frac{\pi}{4}\f$.
    [[nodiscard]] static constexpr auto quarter_pi() -> angle { return { T(0.25) * ::fl::math::pi<T>::value }; }

    [[nodiscard]] constexpr auto operator+() const -> angle { return *this; }
    [[nodiscard]] constexpr auto operator-() const -> angle { return { -this->m_ }; }
    [[nodiscard]] constexpr auto operator+(angle const& other) const -> angle { return { this->m_ + other.m_ }; }
    [[nodiscard]] constexpr auto operator-(angle const& other) const -> angle { return { this->m_ - other.m_ }; }
    [[nodiscard]] constexpr auto operator*(angle const& other) const -> angle { return { this->m_ * other.m_ }; }
    [[nodiscard]] constexpr auto operator/(angle const& other) const -> angle { return { this->m_ / other.m_ }; }
    [[nodiscard]] constexpr auto operator+=(angle const& other) -> angle& { this->m_ += other.m_; return *this; }
    [[nodiscard]] constexpr auto operator-=(angle const& other) -> angle& { this->m_ -= other.m_; return *this; }
    [[nodiscard]] constexpr auto operator*=(angle const& other) -> angle& { this->m_ *= other.m_; return *this; }
    [[nodiscard]] constexpr auto operator/=(angle const& other) -> angle& { this->m_ /= other.m_; return *this; }

    [[nodiscard]] constexpr auto operator*() const -> T const& { return this->m_; }
    [[nodiscard]] constexpr auto operator*() -> T& { return this->m_; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator*(U const& other) const -> angle { return { this->m_ * other }; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator/(U const& other) const -> angle { return { this->m_ / other }; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator+(U const& other) const -> angle { return { this->m_ + other }; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator-(U const& other) const -> angle { return { this->m_ - other }; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator+=(U const& other) -> angle& { this->m_ += other; return *this; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator-=(U const& other) -> angle& { this->m_ -= other; return *this; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator*=(U const& other) -> angle& { this->m_ *= other; return *this; }

    template <concepts::num U>
    [[nodiscard]] constexpr auto operator/=(U const& other) -> angle& { this->m_ /= other; return *this; }

    [[nodiscard]] constexpr auto operator++() -> angle& { ++this->m_; return *this; }
    [[nodiscard]] constexpr auto operator++(int) -> angle { auto ret = *this; ++this->m_; return ret; }
    [[nodiscard]] constexpr auto operator--() -> angle& { --this->m_; return *this; }
    [[nodiscard]] constexpr auto operator--(int) -> angle { auto ret = *this; --this->m_; return ret; }

    constexpr auto operator=(angle const&) -> angle& = default;
    constexpr auto operator=(angle&&) -> angle& = default;

    /// \brief Returns true if the angle is not a zero-angle.
    [[nodiscard]] constexpr explicit operator bool() const { return not is_null(this->m_); }
    [[nodiscard]] constexpr auto operator!() const -> bool { return not this->m_; }
    [[nodiscard]] constexpr explicit operator T() const { return this->m_; }

   private:
    T m_;
  };
} // namespace floppy::math