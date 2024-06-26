/// \file floppy/euclid/point2d.h
/// \brief A two-dimensional point tagged with a unit.
/// \author whs31

#pragma once

#include <functional>
#include <floppy/euclid/length.h>
#include <floppy/euclid/size2d.h>
#include <floppy/euclid/vector2d.h>
#include <floppy/euclid/detail/nt_traits2d.h>

#if defined(FL_QT_GUI)
# include <qpoint.h>
#endif

namespace floppy::math
{
  /// \brief A two-dimensional point tagged with a unit.
  /// \headerfile floppy/euclid.h
  /// \ingroup geometry
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
    using unit_type = U;

    /// \brief Underlying number type.
    using underlying_type = T;

    /// \brief Associated size2d type.
    using size2d_type = size2d<unit_type, underlying_type>;

    /// \brief Associated vector type.
    using vector2d_type = vector2d<unit_type, underlying_type>;

    /// \brief Constructs new point with zero coordinates.
    constexpr point2d() : detail::basic_two_dimensional_type<point2d<U, T>, U, T>() {}

    /// \brief Constructs new point from given scalar values.
    /// \param x The x-coordinate.
    /// \param y The y-coordinate.
    constexpr point2d(underlying_type x, underlying_type y) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new point from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    constexpr point2d(length<unit_type> x, length<unit_type> y) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new point, setting all components to the same value.
    /// \param value The value to set all components to.
    constexpr explicit point2d(underlying_type value) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(value) {}

    /// \brief Tags a unitless value with units.
    /// \param p Unitless point2d
    template <typename U2 = default_unit>
    requires (not std::is_same_v<U, U2>)
    constexpr explicit point2d(point2d<default_unit, T> const& p) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(p.x(), p.y()){}

    /// \brief Constructs new point from size2d.
    /// \param other The other size2d.
    constexpr explicit point2d(size2d_type const& other) : point2d(other.x(), other.y()) {}

    /// \brief Constructs new point from a vector2d.
    /// \param v The vector2d to copy.
    constexpr explicit point2d(vector2d<default_unit, underlying_type> const& v) : detail::basic_two_dimensional_type<point2d<U, T>, U, T>(v.x(), v.y()) {}

    /// \brief Applies the function <b>fn</b> to each component of the point.
    /// \tparam F The type of function to apply.
    /// \param fn The function to apply.
    template <std::invocable<underlying_type> F>
    constexpr auto map(F&& fn) const {
      return point2d<unit_type, decltype(fn(this->x()))>(fn(this->x()), fn(this->y()));
    }

    /// \brief Applies the function <b>fn</b> to each component of this point and the other point.
    /// \tparam F The type of function to apply.
    /// \param other The other point to apply.
    /// \param fn The function to apply.
    template <std::invocable<underlying_type, underlying_type> F>
    constexpr auto zip(point2d const& other, F&& fn) const -> vector2d<unit_type, decltype(fn(this->x(), other.x()))> {
      return vector2d<unit_type, decltype(fn(this->x(), other.x()))>(fn(this->x(), other.x()), fn(this->y(), other.y()));
    }
    // todo: extend https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.extend

    /// \brief Drops the units from the point, returning just the numeric scalar values.
    [[nodiscard]] constexpr auto to_untyped() const -> point2d<default_unit, underlying_type> {
      return point2d<default_unit, underlying_type>(this->x(), this->y());
    }

    /// \brief Converts this point2d into <tt>size2d</tt>.
    /// \return The resulting size2d.
    [[nodiscard]] constexpr auto to_size2d() const -> size2d_type {
      return size2d_type(this->x(), this->y());
    }

    /// \brief Converts this point2d into <tt>vector2d</tt>.
    /// \return The resulting vector2d.
    [[nodiscard]] constexpr auto to_vector2d() const -> vector2d<unit_type, underlying_type> {
      return vector2d<unit_type, underlying_type>(this->x(), this->y());
    }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Casts this point2d into <tt>QPoint</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] constexpr auto to_qpoint() const -> QPoint {
      auto const i = this->to_i32();
      return QPoint(i.x(), i.y());
    }

    /// \brief Casts this point2d into <tt>QPointF</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] constexpr auto to_qpointf() const -> QPointF { return QPointF(this->x(), this->y()); }
  #endif

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
    [[nodiscard]] constexpr auto cast() const -> point2d<unit_type, T2> {
      return point2d<unit_type, T2>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f32</tt> point2d.
    [[nodiscard]] constexpr auto to_f32() const -> point2d<unit_type, f32> {
      return point2d<unit_type, f32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f64</tt> point2d.
    [[nodiscard]] constexpr auto to_f64() const -> point2d<unit_type, f64> {
      return point2d<unit_type, f64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u32</tt> point2d.
    [[nodiscard]] constexpr auto to_u32() const -> point2d<unit_type, u32> {
      return point2d<unit_type, u32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u64</tt> point2d.
    [[nodiscard]] constexpr auto to_u64() const -> point2d<unit_type, u64> {
      return point2d<unit_type, u64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>usize</tt> point2d.
    [[nodiscard]] constexpr auto to_usize() const -> point2d<unit_type, usize> {
      return point2d<unit_type, usize>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i32</tt> point2d.
    [[nodiscard]] constexpr auto to_i32() const -> point2d<unit_type, i32> {
      return point2d<unit_type, i32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i64</tt> point2d.
    [[nodiscard]] constexpr auto to_i64() const -> point2d<unit_type, i64> {
      return point2d<unit_type, i64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>isize</tt> point2d.
    [[nodiscard]] constexpr auto to_isize() const -> point2d<unit_type, isize> {
      return point2d<unit_type, isize>(this->x(), this->y());
    }

    // todo: to_3d https://docs.rs/euclid/latest/euclid/struct.Point2D.html#method.to_3d

    /// \brief Returns distance between this and another point.
    /// \param other The other point.
    /// \return The distance.
    [[nodiscard]] constexpr auto distance_to(point2d<U, T> const& other) const -> length<unit_type> {
      return length<unit_type>(std::hypot(this->x() - other.x(), this->y() - other.y()));
    }

    /// \brief Calculates Euclidean division, the matching method for rem_euclid.
    /// \tparam Q The type of the divisor. Can be <tt>point2d</tt> or <tt>size2d</tt>.
    /// \param other The divisor.
    /// \return The quotient.
    /// \see floppy::math::rem_euclid, floppy::math::div_euclid
    /// \see rem_euclid
    template <concepts::any_of<point2d, size2d_type> Q>
    [[nodiscard]] constexpr auto div_euclid(Q const& other) const -> point2d {
      return point2d(
        fl::math::div_euclid(this->x(), other.x()),
        fl::math::div_euclid(this->y(), other.y())
      );
    }

    /// \brief Calculates the least nonnegative remainder of <tt>self (mod other)</tt>.
    /// \tparam Q The type of the divisor. Can be <tt>point2d</tt> or <tt>size2d</tt>.
    /// \param other The divisor.
    /// \return The remainder.
    /// \see floppy::math::rem_euclid, floppy::math::div_euclid
    /// \see div_euclid
    template <concepts::any_of<point2d, size2d_type> Q>
    [[nodiscard]] constexpr auto rem_euclid(Q const& other) const -> point2d {
      return point2d(
        fl::math::rem_euclid(this->x(), other.x()),
        fl::math::rem_euclid(this->y(), other.y())
      );
    }

    /// \brief Constructs new point with zero coordinates.
    [[nodiscard]] static constexpr auto origin() -> point2d { return point2d(); }

    /// \brief Returns the unchanged point.
    [[nodiscard]] constexpr auto operator+() const -> point2d { return *this; }

    /// \brief Returns the negated point.
    [[nodiscard]] constexpr auto operator-() const -> point2d { return point2d(-this->x(), -this->y()); }

    /// \brief Checks if two points are equal.
    /// \param other The other point.
    [[nodiscard]] constexpr auto operator==(point2d const& other) const -> bool {
      return eq(this->x(), other.x()) and eq(this->y(), other.y());
    }

    /// \brief Checks if two points are not equal.
    /// \param other The other point.
    [[nodiscard]] constexpr auto operator!=(point2d const& other) const -> bool {
      return not eq(this->x(), other.x()) or not eq(this->y(), other.y());
    }

    /// \brief Sums point and either point2d, size2d or vector2d.
    /// \tparam Q The type of the summand. Can be <tt>point2d</tt>, <tt>vector2d</tt> or <tt>size2d</tt>.
    /// \param other The summand.
    /// \return The sum.
    template <concepts::any_of<point2d, size2d_type, vector2d_type> Q>
    [[nodiscard]] constexpr auto operator+(Q const& other) const -> point2d {
      return point2d(this->x() + other.x(), this->y() + other.y());
    }

    /// \brief Subtracts point and either point2d, size2d or vector2d.
    /// \tparam Q The type of the subtrahend. Can be <tt>point2d</tt>, <tt>vector2d</tt> or <tt>size2d</tt>.
    /// \param other The subtrahend.
    /// \return The difference.
    template <concepts::any_of<point2d, size2d_type, vector2d_type> Q>
    [[nodiscard]] constexpr auto operator-(Q const& other) const -> point2d {
      return point2d(this->x() - other.x(), this->y() - other.y());
    }

    /// \brief Multiplies point with scalar.
    /// \param other The scalar.
    /// \return The product.
    [[nodiscard]] constexpr auto operator*(underlying_type const& other) const -> point2d {
      return point2d(this->x() * other, this->y() * other);
    }

    /// \brief Multiplies point with scale.
    /// \tparam U2 The unit type of the scale.
    /// \tparam T2 The value type of the scale.
    /// \param other The scale.
    /// \return The product.
    template <typename U2, concepts::num T2>
    [[nodiscard]] constexpr auto operator*(scale<unit_type, U2, T2> const& other) const -> point2d<U2, underlying_type> {
      return point2d<U2, underlying_type>(
        this->x() * other.template as<underlying_type>(),
        this->y() * other.template as<underlying_type>()
      );
    }

    /// \brief Divides point with scalar.
    /// \param other The scalar.
    /// \return The quotient.
    [[nodiscard]] constexpr auto operator/(underlying_type const& other) const -> point2d {
      return point2d(this->x() / other, this->y() / other);
    }

    /// \brief Divides point with scale.
    /// \tparam U2 The unit type of the scale.
    /// \tparam T2 The value type of the scale.
    /// \param other The scale.
    /// \return The quotient.
    template <typename U2, concepts::num T2>
    [[nodiscard]] constexpr auto operator/(scale<U2, unit_type, T2> const& other) const -> point2d<U2, underlying_type> {
      return point2d<U2, underlying_type>(
        this->x() / other.template as<underlying_type>(),
        this->y() / other.template as<underlying_type>()
      );
    }

    /// \brief Adds point and either point2d, size2d or vector2d.
    /// \tparam Q The type of the summand. Can be <tt>point2d</tt>, <tt>vector2d</tt> or <tt>size2d</tt>.
    /// \param other The summand.
    /// \return The sum.
    template <concepts::any_of<point2d, size2d_type, vector2d_type> Q>
    constexpr auto operator+=(Q const& other) -> point2d& {
      this->x_mut() += other.x();
      this->y_mut() += other.y();
      return *this;
    }

    /// \brief Subtracts point and either point2d, size2d or vector2d.
    /// \tparam Q The type of the subtrahend. Can be <tt>point2d</tt>, <tt>vector2d</tt> or <tt>size2d</tt>.
    /// \param other The subtrahend.
    /// \return The difference.
    template <concepts::any_of<point2d, size2d_type, vector2d_type> Q>
    constexpr auto operator-=(Q const& other) -> point2d& {
      this->x_mut() -= other.x();
      this->y_mut() -= other.y();
      return *this;
    }

    /// \brief Multiplies point with scalar.
    /// \param other The scalar.
    /// \return The product.
    constexpr auto operator*=(underlying_type const& other) -> point2d& {
      this->x_mut() *= other;
      this->y_mut() *= other;
      return *this;
    }

    /// \brief Divides point with scalar.
    /// \param other The scalar.
    /// \return The quotient.
    constexpr auto operator/=(underlying_type const& other) -> point2d& {
      this->x_mut() /= other;
      this->y_mut() /= other;
      return *this;
    }

    /// \brief Constructs new point from size2d.
    /// \param other The other size2d.
    [[nodiscard]] static constexpr auto from_size2d(size2d_type const& other) -> point2d { return point2d(other); }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Constructs new point from <tt>QPoint</tt>.
    /// \param other The other <tt>QPoint</tt>.
    /// \remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    constexpr explicit point2d(QPoint const& other) : point2d(other.x(), other.y()) {}

    /// \brief Constructs new point from <tt>QPointF</tt>.
    /// \param other The other <tt>QPointF</tt>.
    /// \remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    constexpr explicit point2d(QPointF const& other) : point2d(other.x(), other.y()) {}

    /// \brief Constructs new point from <tt>QPoint</tt>.
    /// \param other The other <tt>QPoint</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] static constexpr auto from_qpoint(QPoint const& other) -> point2d { return point2d(other.x(), other.y()); }

    /// \brief Constructs new point from <tt>QPointF</tt>.
    /// \param other The other <tt>QPointF</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] static constexpr auto from_qpointf(QPointF const& other) -> point2d { return point2d(other.x(), other.y()); }
  #endif
  };
} // namespace floppy::math