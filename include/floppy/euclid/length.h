/// \file floppy/euclid/length.h
/// \brief A one-dimensional distance with associated unit of measurement.
/// \author whs31

#pragma once

#include <floppy/euclid/scale.h>
#include <floppy/euclid/detail/nt_traits.h>

namespace floppy::math
{
  /// \brief A one-dimensional distance with associated unit of measurement.
  /// \headerfile floppy/euclid/length.h
  /// \details Unit is not used in the representation of a length value.
  /// It is used only at compile time to ensure that a Length stored with one unit is converted
  /// explicitly before being used in an expression that requires a different unit.
  /// It may be a type without values, such as an empty struct.
  /// You can multiply a length with a scale to convert it to another unit of measurement. See the \ref scale
  /// docs for detailed example.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  /// \tparam U Associated unit of measurement. Default is \ref default_unit.
  /// \see floppy::math::scale
  /// \see floppy::math::angle
  template <typename U = default_unit, concepts::num T = f32>
  class length : public traits::formattable<length<U, T>, char>,
                 public detail::basic_numeric_newtype<length<U, T>, T>
  {
   public:
    /// \brief Associated unit of measurement.
    using unit = U;

    /// \brief Constructs an empty length.
    constexpr length()
      : detail::basic_numeric_newtype<length<U, T>, T>()
    {}

    /// \brief Constructs a length from a given numeric value.
    explicit constexpr length(T value)
      : detail::basic_numeric_newtype<length<U, T>, T>(value)
    {}

    /// \brief Returns string representation of the length.
    /// \details Length is represented as it's numeric value. If the underlying number type is floating
    /// point, it is rounded to three decimal places.
    /// \note Due to limitations of the language, units are not displayed.
    /// \return String representation of the object.
    [[nodiscard]] virtual auto to_string() const -> std::string override {
      if constexpr(std::is_floating_point_v<T>)
        return fmt::format("{:.3f}", this->m_);
      else
        return fmt::format("{}", this->m_);
    }

    /// \brief Casts the unit of measurement.
    /// \tparam U2 New unit of measurement.
    /// \return The length with the new unit of measurement and the same value.
    template <typename U2>
    [[nodiscard]] constexpr auto cast_unit() const -> length<U2, T> { return length<U2, T>(this->m_); }

    /// \brief Cast from one numeric representation to another, preserving the units.
    /// \tparam T2 New number type.
    /// \return The length with the new number type and the same value.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto cast() const -> length<U, T2> { return length<U, T2>(this->m_); }

    /// \brief Linearly interpolate between this length and another length.
    /// \details Example:
    /// \code {.cpp}
    /// auto const from = length(0.0F);
    /// auto const to = length(8.0F);
    /// fmt::println("{}", from.lerp(to, -1.0F));
    /// fmt::println("{}", from.lerp(to, 0.0F));
    /// fmt::println("{}", from.lerp(to, 0.5F));
    /// fmt::println("{}", from.lerp(to, 1.0F));
    /// fmt::println("{}", from.lerp(to, 2.0F));
    /// \endcode
    ///
    /// Output:
    /// \code {.sh}
    /// -8.0
    /// 0.0
    /// 4.0
    /// 8.0
    /// 16.0
    /// \endcode
    /// \param other The other length.
    /// \param t Interpolation factor.
    /// \return The interpolated length.
    [[nodiscard]] constexpr auto lerp(length<U, T> const& other, T t) const -> length<U, T> {
      return length<U, T>(this->m_ + (other.m_ - this->m_) * t);
    }

    /// \brief Returns the underlying value.
    [[nodiscard]] constexpr auto operator*() const -> T { return this->m_; }

    constexpr auto operator=(T value) -> length& {
      this->m_ = value;
      return *this;
    }

    constexpr auto operator+=(length const& other) -> length& { return *this = *this + other; }
    constexpr auto operator-=(length const& other) -> length& { return *this = *this - other; }
    constexpr auto operator*=(length const& other) -> length& { return *this = *this * other; }
    constexpr auto operator/=(length const& other) -> length& { return *this = *this / other; }

    template <concepts::num T2>
    constexpr auto operator+=(T2 value) -> length& { return *this = *this + static_cast<T>(value); }

    template <concepts::num T2>
    constexpr auto operator-=(T2 value) -> length& { return *this = *this - static_cast<T>(value); }

    template <concepts::num T2>
    constexpr auto operator*=(T2 value) -> length& { return *this = *this * static_cast<T>(value); }

    template <concepts::num T2>
    constexpr auto operator/=(T2 value) -> length& { return *this = *this / static_cast<T>(value); }

    constexpr auto operator+(length const& other) const -> length { return length(this->m_ + other.m_); }
    constexpr auto operator-(length const& other) const -> length { return length(this->m_ - other.m_); }
    constexpr auto operator*(length const& other) const -> length { return length(this->m_ * other.m_); }

    template <concepts::num T2>
    constexpr auto operator+(T2 value) const -> length { return length(this->m_ + static_cast<T>(value)); }

    template <concepts::num T2>
    constexpr auto operator-(T2 value) const -> length { return length(this->m_ - static_cast<T>(value)); }

    template <concepts::num T2>
    constexpr auto operator*(T2 value) const -> length { return length(this->m_ * static_cast<T>(value)); }

    template <concepts::num T2>
    constexpr auto operator/(T2 value) const -> length { return length(this->m_ / static_cast<T>(value)); }

    /// \brief Divides this length by length of another unit and return the result as a scale.
    /// \tparam U2 Unit of the other length.
    /// \tparam T2 Type of the other length.
    /// \param other The other length.
    /// \return The result as a scale ratio <i>U2/U</i> of type T of this length.
    template <typename U2, concepts::num T2>
    constexpr auto operator/(length<U2, T2> const& other) const -> scale<U2, U, T> {
      return scale<U2, U, T>(this->value() / static_cast<T>(other.value()));
    }

    /// \brief Multiplies this length by scale factor.
    /// \tparam U2 Unit of the scale factor.
    /// \tparam T2 Type of the scale factor.
    /// \param other The scale factor.
    /// \return The result as a length of type T of this length.
    template <typename U2, concepts::num T2>
    constexpr auto operator*(scale<U, U2, T2> const& s) const -> length<U2, T> {
      return length<U2, T>(this->value() * static_cast<T>(s.value()));
    }

    /// \brief Divides this length by scale factor.
    /// \tparam U2 Unit of the scale factor.
    /// \tparam T2 Type of the scale factor.
    /// \param other The scale factor.
    /// \return The result as a length of type T of this length.
    template <typename U2, concepts::num T2>
    constexpr auto operator/(scale<U2, U, T2> const& s) const -> length<U2, T> {
      return length<U2, T>(this->value() / static_cast<T>(s.value()));
    }

    constexpr auto operator+() const -> length { return length(this->m_); }
    constexpr auto operator-() const -> length { return length(-this->m_); }
  };
} // namespace floppy::math