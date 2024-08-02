/// \file floppy/euclid/scale.h
/// \brief A scaling factor between two different units of measurement.
/// \author whs31

#pragma once

#include <floppy/detail/math.h>
#include <floppy/traits.h>
#include <floppy/euclid/detail/nt_traits.h>

namespace floppy::math
{
  /// \brief Default unit of measurement.
  struct [[maybe_unused]] default_unit {};

  /// \brief A scaling factor between two different units of measurement.
  /// \headerfile floppy/euclid.h
  /// \ingroup geometry
  /// \details This is effectively a type-safe float, intended to be used in combination with other types
  /// like \ref floppy::math::length to enforce conversion between systems of measurement at compile time.
  /// <b>S</b> and <b>D</b> are types representing the units before and after multiplying a value by a scale.
  /// They may be types without values, such as empty enums. For example:
  /// \code {.cpp}
  /// using fl::math::scale;
  /// struct Inch {};
  /// struct Mm {};
  ///
  /// auto const mm_per_inch = scale<Inch, Mm>(25.4F);
  /// auto const one_foot = length<Inch>(12.0F);
  /// auto const one_foot_in_mm = mm_per_inch * one_foot;
  /// \endcode
  /// \tparam S Source type.
  /// \tparam D Destination type.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  /// \see floppy::math::length
  template <typename S, typename D, concepts::num T = f32>
  struct scale : public formattable<scale<S, D, T>, char>,
                 public detail::basic_numeric_newtype<scale<S, D, T>, T>
  {
    /// \brief Underlying source type.
    using source_type = S;

    /// \brief Underlying destination type.
    using destination_type = D;

    /// \brief Constructs an identity scale.
    /// \see scale::identity
    constexpr scale() : detail::basic_numeric_newtype<scale<S, D, T>, T>()
    {}

    /// \brief Constructs a scale from a number.
    /// \param s Scale factor.
    /// \see scale::identity
    constexpr explicit scale(T s) : detail::basic_numeric_newtype<scale<S, D, T>, T>(s)
    {}

    /// \brief Returns string representation of the scale.
    /// \details Scale is represented as the reciprocal of the underlying number, e.g. if scale value
    /// is <i>0.1F</i>, the string representation is <i>1/10</i>. Floating point values are rounded to
    /// nearest integer.
    /// \return String representation of the object.
    [[nodiscard]] virtual auto to_string() const -> std::string override {
      return fmt::format("1/{}", std::round(1.0F / this->m_));
    }

    /// \brief Returns <tt>true</tt> if this scale has no effect.
    /// \details Example:
    /// \code {.cpp}
    /// struct mm {};
    /// struct cm {};
    /// using fl::math::scale;
    ///
    /// auto const cm_per_mm = scale<mm, cm>(0.1F);
    /// auto const mm_per_mm = scale<mm, mm>(1.0F);
    /// assert(cm_per_mm.is_identity() == false);
    /// assert(mm_per_mm.is_identity() == true);
    /// assert(mm_per_mm == scale<mm, mm>::identity());
    /// \endcode
    /// \return <tt>true</tt> if this scale has no effect.
    [[nodiscard]] constexpr auto is_identity() const -> bool { return eq(this->m_, T(1.0)); }

    /// \brief Returns the inverse scale (1.0 / scale).
    /// \details Example:
    /// \code {.cpp}
    /// struct mm {};
    /// struct cm {};
    /// using fl::math::scale;
    ///
    /// auto const cm_per_mm = scale<mm, cm>(0.1F);
    /// assert(cm_per_mm.inverse() == scale<mm, cm>(10.0F));
    /// \endcode
    /// \return The inverse scale.
    [[nodiscard]] constexpr auto inverse() const -> scale<destination_type, source_type, T> {
      return scale<destination_type, source_type, T>(T(1.0) / this->m_);
    }

    /// \brief Creates an identity scale (1.0).
    [[nodiscard]] static constexpr auto identity() -> scale { return scale(1.0); }

    /// \brief Returns the underlying scalar scale factor.
    /// \return This scale as number.
    [[nodiscard]] constexpr auto operator*() const -> T { return this->m_; }

    /// \brief Sums two scales of same source and destination types.
    /// \param other Scale to add
    /// \return The result of addition
    [[nodiscard]] constexpr auto operator+(scale const& other) const -> scale { return scale(this->m_ + other.m_); }

    /// \brief Subtracts two scales of same source and destination types.
    /// \param other Scale to subtract
    /// \return The result of subtraction
    [[nodiscard]] constexpr auto operator-(scale const& other) const -> scale { return scale(this->m_ - other.m_); }

    /// \brief Multiplies this scale with another scale of different source and destination types.
    /// \tparam Sn Other scale source type
    /// \tparam Dn Other scale destination type
    /// \tparam Tn Other scale underlying number type
    /// \param other Scale to multiply with
    /// \return The result of multiplication
    template <typename Sn, typename Dn, concepts::num Tn>
    [[nodiscard]] constexpr auto operator*(scale<Sn, Dn, Tn> const& other) const -> scale {
      return scale(this->value() * other.value());
    }

    /// \brief Divides this scale with another scale of different source and destination types.
    /// \tparam Sn Other scale source type
    /// \tparam Dn Other scale destination type
    /// \tparam Tn Other scale underlying number type
    /// \param other Scale to divide with
    /// \return The result of division
    template <typename Sn, typename Dn, concepts::num Tn>
    [[nodiscard]] constexpr auto operator/(scale<Sn, Dn, Tn> const& other) const -> scale {
      return scale(this->value() / other.value());
    }
  };
} // namespace floppy::math