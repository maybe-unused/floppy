/// \file floppy/euclid/length.h
/// \brief A one-dimensional distance with associated unit of measurement.
/// \author whs31

#pragma once

#include <floppy/euclid/scale.h>

namespace floppy::math
{
  /// \brief A one-dimensional distance with associated unit of measurement.
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
  class length : traits::formattable<length<U, T>, char>
  {
   public:
    /// \brief Constructs an empty length.
    constexpr length()
      : m_(static_cast<T>(0.0F))
    {}

    /// \brief Constructs a length from a given numeric value.
    explicit constexpr length(T value)
      : m_(value)
    {}

    /// \brief Returns the underlying value.
    [[nodiscard]] constexpr auto value() const -> T { return this->m_; }

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

   private:
    T m_;
  };
} // namespace floppy::math