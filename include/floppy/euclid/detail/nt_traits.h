/// \file floppy/euclid/detail/nt_traits.h
/// \author whs31

#pragma once

#include <floppy/detail/math.h>
#include <floppy/detail/concepts.h>

namespace floppy::math::detail
{
  /// \brief CRTP base for trivially comparable numeric types.
  /// \headerfile floppy/euclid.h
  /// \ingroup geometry_details
  /// \tparam T The type itself.
  template <typename T>
  struct default_comparable
  {
    /// \brief Compares this and <i>other</i> with \ref strong_compare for equality.
    /// \param other The other value to compare with.
    [[nodiscard]] constexpr auto operator==(T const& other) const -> bool { return (strong_compare(**static_cast<T const*>(this), *other) == std::strong_ordering::equal); }

    /// \brief Compares this and <i>other</i> with \ref strong_compare for inequality.
    /// \param other The other value to compare with.
    [[nodiscard]] constexpr auto operator!=(T const& other) const -> bool { return (strong_compare(**static_cast<T const*>(this), *other) != std::strong_ordering::equal); }

    /// \brief Compares this and <i>other</i> with \ref strong_compare for less than.
    /// \param other The other value to compare with.
    [[nodiscard]] constexpr auto operator<(T const& other) const -> bool { return (strong_compare(**static_cast<T const*>(this), *other) == std::strong_ordering::less); }

    /// \brief Compares this and <i>other</i> with \ref strong_compare for greater than.
    /// \param other The other value to compare with.
    [[nodiscard]] constexpr auto operator>(T const& other) const -> bool { return (strong_compare(**static_cast<T const*>(this), *other) == std::strong_ordering::greater); }

    /// \brief Compares this and <i>other</i> with \ref strong_compare for less than or equal to.
    /// \param other The other value to compare with.
    [[nodiscard]] constexpr auto operator<=(T const& other) const -> bool {
      return (strong_compare(**static_cast<T const*>(this), *other) == std::strong_ordering::less
        or strong_compare(**static_cast<T const*>(this), *other) == std::strong_ordering::equal);
    }

    /// \brief Compares this and <i>other</i> with \ref strong_compare for greater than or equal to.
    /// \param other The other value to compare with.
    [[nodiscard]] constexpr auto operator>=(T const& other) const -> bool {
      return (strong_compare(**static_cast<T const*>(this), *other) == std::strong_ordering::greater
        or strong_compare(**static_cast<T const*>(this), *other) == std::strong_ordering::equal);
    }

    /// \brief Compares this and <i>other numeric value</i> with \ref strong_compare.
    /// \tparam T2 Right hand side numeric type.
    /// \param other The other value to compare with.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto operator<=>(T2 const& other) const { return strong_compare(**static_cast<T const*>(this), static_cast<T::underlying_type>(other)); }

    /// \brief Compares this and <i>other numeric value</i> with \ref strong_compare for equality.
    /// \tparam T2 Right hand side numeric type.
    /// \param other The other value to compare with.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto operator==(T2 const& other) const -> bool { return strong_compare(**static_cast<T const*>(this), static_cast<T::underlying_type>(other)) == std::strong_ordering::equal; }

    /// \brief Compares this and <i>other numeric value</i> with \ref strong_compare for inequality.
    /// \tparam T2 Right hand side numeric type.
    /// \param other The other value to compare with.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto operator!=(T2 const& other) const -> bool { return strong_compare(**static_cast<T const*>(this), static_cast<T::underlying_type>(other)) != std::strong_ordering::equal; }
  };

  /// \brief CRTP base for numeric newtypes that can be compared.
  /// \ingroup geometry_details
  /// \headerfile floppy/euclid.h
  /// \tparam T The type itself.
  /// \tparam U The underlying number type.
  /// \see floppy::math::angle
  /// \see floppy::math::length
  /// \see floppy::math::scale
  template <typename T, concepts::num U>
  struct basic_numeric_newtype : public default_comparable<T>
  {
    /// \brief Underlying number type.
    using underlying_type = U;

    /// \brief Constructs a new basic_numeric_newtype with the value of <i>T(0.0)</i>.
    constexpr basic_numeric_newtype()
      : m_(underlying_type(0.0))
    {}

    /// \brief Constructs a new basic_numeric_newtype with the value of <i>value</i>.
    /// \param value The value of the new basic_numeric_newtype.
    constexpr explicit basic_numeric_newtype(underlying_type value)
      : m_(value)
    {}

    /// \brief Default copy constructor.
    constexpr basic_numeric_newtype(basic_numeric_newtype const&) = default;

    /// \brief Default move constructor.
    constexpr basic_numeric_newtype(basic_numeric_newtype&&) = default;

    /// \brief Default destructor.
    ~basic_numeric_newtype() = default;

    /// \brief Default copy assignment.
    constexpr auto operator=(basic_numeric_newtype const&) -> basic_numeric_newtype& = default;

    /// \brief Default move assignment.
    constexpr auto operator=(basic_numeric_newtype&&) -> basic_numeric_newtype& = default;

    /// \brief Returns the underlying numeric scalar.
    /// \return The underlying numeric scalar.
    [[nodiscard]] constexpr auto value() const -> underlying_type { return this->m_; }

    /// \brief Returns this numeric newtype as number in another numeric representation.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto as() const -> U2 { return static_cast<U2>(this->m_); }

    /// \brief Returns this numeric newtype as 32-bit floating point number.
    [[maybe_unused]] [[nodiscard]] constexpr auto as_f32() const -> underlying_type { return this->as<f32>(); }

    /// \brief Returns this numeric newtype as 64-bit floating point number.
    [[maybe_unused]] [[nodiscard]] constexpr auto as_f64() const -> underlying_type { return this->as<f64>(); }

    /// \brief Returns true if this is a finite number.
    [[nodiscard]] constexpr auto is_finite() const -> bool { return ::std::isfinite(this->m_); }

    /// \brief Returns <b>sin</b>(this).
    [[nodiscard]] constexpr auto sin() const -> T { return std::sin(this->m_); }

    /// \brief Returns <b>cos</b>(this).
    [[nodiscard]] constexpr auto cos() const -> T { return std::cos(this->m_); }

    /// \brief Returns <b>tan</b>(this).
    [[nodiscard]] constexpr auto tan() const -> T { return std::tan(this->m_); }

    /// \brief Returns <b>asin</b>(this).
    [[nodiscard]] constexpr auto asin() const -> T { return std::asin(this->m_); }

    /// \brief Returns <b>acos</b>(this).
    [[nodiscard]] constexpr auto acos() const -> T { return std::acos(this->m_); }

    /// \brief Returns <b>atan</b>(this).
    [[nodiscard]] constexpr auto atan() const -> T { return std::atan(this->m_); }

    /// \brief Increments the underlying value by <tt>1</tt>.
    /// \return The incremented value.
    [[nodiscard]] constexpr auto operator++() -> T& { ++this->m_; return *this; }

    /// \brief Increments the underlying value by <tt>1</tt>.
    /// \return The incremented value.
    [[nodiscard]] constexpr auto operator++(int) -> T { auto ret = *this; ++this->m_; return ret; }

    /// \brief Decrements the underlying value by <tt>1</tt>.
    /// \return The decremented value.
    [[nodiscard]] constexpr auto operator--() -> T& { --this->m_; return *this; }

    /// \brief Decrements the underlying value by <tt>1</tt>.
    /// \return The decremented value.
    [[nodiscard]] constexpr auto operator--(int) -> T { auto ret = *this; --this->m_; return ret; }

    /// \brief Returns <tt>true</tt> if underlying value is not <tt>0</tt>.
    [[nodiscard]] constexpr explicit operator bool() const { return not is_null(this->m_); }

    /// \brief Returns <tt>true</tt> if underlying value is <tt>0</tt>.
    [[nodiscard]] constexpr auto operator!() const -> bool { return not this->m_; }

    /// \brief Returns the underlying value as <tt>underlying_type</tt>.
    [[nodiscard]] constexpr explicit operator underlying_type() const { return this->m_; }

   protected:
    underlying_type m_; ///< Underlying value.
  };

  /// \brief CRTP base for numeric newtypes containing math operations implementation.
  /// \ingroup geometry_details
  /// \headerfile floppy/euclid.h
  /// \tparam T The type itself.
  /// \tparam U The underlying number type.
  /// \see floppy::math::angle
  template <typename T, concepts::num U>
  struct advanced_numeric_type : public basic_numeric_newtype<T, U>
  {
    using basic_numeric_newtype<T, U>::basic_numeric_newtype;

    /// \brief Returns the underlying numeric scalar.
    /// \return The underlying numeric scalar.
    [[nodiscard]] constexpr auto operator*() const -> U { return this->m_; }

    /// \brief Returns the value.
    [[nodiscard]] constexpr auto operator+() const -> T { return *this; }

    /// \brief Returns the negated value.
    [[nodiscard]] constexpr auto operator-() const -> T { return T(-this->m_); }

    /// \brief Returns <tt>this + other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator+(T const& other) const -> T { return T(this->m_ + other.m_); }

    /// \brief Returns <tt>this - other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator-(T const& other) const -> T { return T(this->m_ - other.m_); }

    /// \brief Returns <tt>this * other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator*(T const& other) const -> T { return T(this->m_ * other.m_); }

    /// \brief Returns <tt>this / other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator/(T const& other) const -> T { return T(this->m_ / other.m_); }

    /// \brief Returns <tt>this += other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator+=(T const& other) -> T& { this->m_ += other.m_; return *this; }

    /// \brief Returns <tt>this -= other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator-=(T const& other) -> T& { this->m_ -= other.m_; return *this; }

    /// \brief Returns <tt>this *= other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator*=(T const& other) -> T& { this->m_ *= other.m_; return *this; }

    /// \brief Returns <tt>this /= other</tt>.
    /// \param other The other value.
    [[nodiscard]] constexpr auto operator/=(T const& other) -> T& { this->m_ /= other.m_; return *this; }

    /// \brief Returns <tt>this * other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator*(U2 const& other) const -> T { return T(this->m_ * other); }

    /// \brief Returns <tt>this / other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator/(U2 const& other) const -> T { return T(this->m_ / other); }

    /// \brief Returns <tt>this + other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator+(U2 const& other) const -> T { return T(this->m_ + other); }

    /// \brief Returns <tt>this - other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator-(U2 const& other) const -> T { return T(this->m_ - other); }

    /// \brief Returns <tt>this += other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator+=(U2 const& other) -> T& { this->m_ += other; return *this; }

    /// \brief Returns <tt>this -= other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator-=(U2 const& other) -> T& { this->m_ -= other; return *this; }

    /// \brief Returns <tt>this *= other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator*=(U2& other) -> T& { this->m_ *= other; return *this; }

    /// \brief Returns <tt>this /= other</tt>.
    /// \tparam U2 Right hand side numeric type.
    /// \param other The other value.
    template <concepts::num U2>
    [[nodiscard]] constexpr auto operator/=(U2 const& other) -> T& { this->m_ /= other; return *this; }
  };
} // namespace floppy::math::detail