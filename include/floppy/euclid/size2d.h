/// \file floppy/euclid/size2d.h
/// \brief A two-dimensional size tagged with a unit.
/// \author whs31

#pragma once

#include <floppy/detail/math.h>
#include <floppy/detail/concepts.h>
#include <floppy/euclid/length.h>
#include <floppy/euclid/detail/nt_traits2d.h>

#if defined(FL_QT_GUI)
# include <qsize.h>
#endif

namespace floppy::math
{
  template <typename U, concepts::num T>
  class point2d;

  /// \brief A two-dimensional size2d tagged with a unit.
  /// \headerfile floppy/euclid.h
  /// \tparam U Associated unit of measurement. Default is \ref default_unit.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  /// \see floppy::math::length
  /// \see floppy::math::point2d
  /// \see floppy::math::vector2d
  template <typename U = default_unit, concepts::num T = f32>
  class size2d : public detail::basic_two_dimensional_type<size2d<U, T>, U, T>
  {
   public:
    /// \brief Associated unit type.
    using unit = U;

    /// \brief Underlying number type.
    using underlying_type = T;

    constexpr size2d(size2d const&) = default;
    constexpr size2d& operator=(size2d const&) = default;
    constexpr size2d(size2d&&) = default;
    constexpr size2d& operator=(size2d&&) = default;

    /// \brief Constructs new size2d with zero coordinates.
    constexpr size2d() : detail::basic_two_dimensional_type<size2d<U, T>, U, T>() {}

    /// \brief Constructs new size2d from given scalar values.
    /// \param x The x-coordinate.
    /// \param y The y-coordinate.
    constexpr size2d(underlying_type x, underlying_type y) : detail::basic_two_dimensional_type<size2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new size2d from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    constexpr size2d(length<unit> x, length<unit> y) : detail::basic_two_dimensional_type<size2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new size2d, setting all components to the same value.
    /// \param value The value to set all components to.
    constexpr explicit size2d(underlying_type value) : detail::basic_two_dimensional_type<size2d<U, T>, U, T>(value) {}

    /// \brief Tags a unitless value with units.
    /// \param p Unitless size2d
    template <typename U2 = default_unit>
    requires (not std::is_same_v<U, U2>)
    constexpr explicit size2d(size2d<default_unit, underlying_type> const& p) : detail::basic_two_dimensional_type<size2d<U, T>, U, T>(p.x(), p.y()){}

    /// \brief Alias to <tt>x</tt> member function.
    [[nodiscard]] constexpr auto width() const -> underlying_type { return this->x(); }

    /// \brief Alias to <tt>y</tt> member function.
    [[nodiscard]] constexpr auto height() const -> underlying_type { return this->y(); }

    /// \brief Alias to <tt>x_mut</tt> member function.
    [[nodiscard]] constexpr auto width_mut() -> underlying_type& { return this->x_mut(); }

    /// \brief Alias to <tt>y_mut</tt> member function.
    [[nodiscard]] constexpr auto height_mut() -> underlying_type& { return this->y_mut(); }

    /// \brief Alias to <tt>lx</tt> member function.
    [[nodiscard]] constexpr auto width_typed() const -> length<unit> { return this->lx(); }

    /// \brief Alias to <tt>ly</tt> member function.
    [[nodiscard]] constexpr auto height_typed() const -> length<unit> { return this->ly(); }

    /// \brief Drops the units from the size2d, returning just the numeric scalar values.
    [[nodiscard]] constexpr auto to_untyped() const -> size2d<default_unit, underlying_type> {
      return size2d<default_unit, underlying_type>(this->x(), this->y());
    }

    /// \brief Converts this size2d into <tt>point2d</tt>.
    /// \return The resulting point2d.
    [[nodiscard]] constexpr auto to_point2d() const -> point2d<unit, underlying_type> {
      return point2d<unit, underlying_type>(this->x(), this->y());
    }

    #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Casts this size2d into <tt>QSize</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] constexpr auto to_qsize() const -> QSize {
      auto const i = this->to_i32();
      return QSize(i.x(), i.y());
    }

    /// \brief Casts this size2d into <tt>QSizeF</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] constexpr auto to_qsizef() const -> QSizeF { return QSizeF(this->x(), this->y()); }
    #endif

    /// \brief Casts the unit of measurement.
    /// \tparam U2 New unit of measurement.
    /// \return The size2d with the new unit of measurement and the same value.
    template <typename U2>
    [[nodiscard]] constexpr auto cast_unit() const -> size2d<U2, underlying_type> {
      return size2d<U2, underlying_type>(this->x(), this->y());
    }

    /// \brief Cast from one numeric representation to another, preserving the units.
    /// \tparam T2 New number type.
    /// \return The size2d with the new number type and the same value.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto cast() const -> size2d<unit, T2> {
      return size2d<unit, T2>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f32</tt> size2d.
    [[nodiscard]] constexpr auto to_f32() const -> size2d<unit, f32> {
      return size2d<unit, f32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f64</tt> size2d.
    [[nodiscard]] constexpr auto to_f64() const -> size2d<unit, f64> {
      return size2d<unit, f64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u32</tt> size2d.
    [[nodiscard]] constexpr auto to_u32() const -> size2d<unit, u32> {
      return size2d<unit, u32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u64</tt> size2d.
    [[nodiscard]] constexpr auto to_u64() const -> size2d<unit, u64> {
      return size2d<unit, u64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>usize</tt> size2d.
    [[nodiscard]] constexpr auto to_usize() const -> size2d<unit, usize> {
      return size2d<unit, usize>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i32</tt> size2d.
    [[nodiscard]] constexpr auto to_i32() const -> size2d<unit, i32> {
      return size2d<unit, i32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i64</tt> size2d.
    [[nodiscard]] constexpr auto to_i64() const -> size2d<unit, i64> {
      return size2d<unit, i64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>isize</tt> size2d.
    [[nodiscard]] constexpr auto to_isize() const -> size2d<unit, isize> {
      return size2d<unit, isize>(this->x(), this->y());
    }

    /// \brief Returns result of multiplication of both scalars.
    /// \return Area of the size2d.
    [[nodiscard]] constexpr auto area() const -> underlying_type { return this->x() * this->y(); }

    // todo: to_vector()

    [[nodiscard]] constexpr auto operator+() const -> size2d { return *this; }
    [[nodiscard]] constexpr auto operator-() const -> size2d { return size2d(-this->x(), -this->y()); }

    [[nodiscard]] constexpr auto operator==(const size2d& other) const -> bool {
      return eq(this->x(), other.x()) and eq(this->y(), other.y());
    }

    [[nodiscard]] constexpr auto operator!=(const size2d& other) const -> bool {
      return not eq(this->x(), other.x()) or not eq(this->y(), other.y());
    }

    [[nodiscard]] constexpr auto operator+(const size2d& other) const -> size2d {
      return size2d(this->x() + other.x(), this->y() + other.y());
    }

    [[nodiscard]] constexpr auto operator-(const size2d& other) const -> size2d {
      return size2d(this->x() - other.x(), this->y() - other.y());
    }

    [[nodiscard]] constexpr auto operator*(const underlying_type& other) const -> size2d {
      return size2d(this->x() * other, this->y() * other);
    }

    [[nodiscard]] constexpr auto operator/(const underlying_type& other) const -> size2d {
      return size2d(this->x() / other, this->y() / other);
    }

    [[nodiscard]] constexpr auto operator+=(const size2d& other) -> size2d& {
      this->x_mut() += other.x();
      this->y_mut() += other.y();
      return *this;
    }

    [[nodiscard]] constexpr auto operator-=(const size2d& other) -> size2d& {
      this->x_mut() -= other.x();
      this->y_mut() -= other.y();
      return *this;
    }

    [[nodiscard]] constexpr auto operator*=(const underlying_type& other) -> size2d& {
      this->x_mut() *= other;
      this->y_mut() *= other;
      return *this;
    }

    [[nodiscard]] constexpr auto operator/=(const underlying_type& other) -> size2d& {
      this->x_mut() /= other;
      this->y_mut() /= other;
      return *this;
    }

    template <typename U2>
    [[nodiscard]] constexpr auto operator*(scale<unit, U2> const& s) const -> size2d<U2, underlying_type> {
      return size2d<U2, underlying_type>(this->x() * s.value(), this->y() * s.value());
    }

    template <typename U2>
    [[nodiscard]] constexpr auto operator/(scale<U2, unit> const& s) const -> size2d<U2, underlying_type> {
      return size2d<U2, underlying_type>(this->x() / s.value(), this->y() / s.value());
    }

#if defined(FL_QT_GUI) || defined(FL_DOC)
    // and constructors
    /// \brief Constructs new size2d from <tt>QSize</tt>.
    /// \param other The other <tt>QSize</tt>.
    /// \remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    constexpr explicit size2d(QSize const& other) : size2d(other.width(), other.height()) {}

    /// \brief Constructs new size2d from <tt>QSizeF</tt>.
    /// \param other The other <tt>QSizeF</tt>.
    /// \remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    constexpr explicit size2d(QSizeF const& other) : size2d(other.width(), other.height()) {}

    /// \brief Constructs new size2d from <tt>QSize</tt>.
    /// \param other The other <tt>QSize</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] static constexpr auto from_qsize(QSize const& other) -> size2d { return size2d(other.width(), other.height()); }

    /// \brief Constructs new size2d from <tt>QSizeF</tt>.
    /// \param other The other <tt>QSizeF</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] static constexpr auto from_qsizef(QSizeF const& other) -> size2d { return size2d(other.width(), other.height()); }
#endif
  };
} // namespace floppy::math