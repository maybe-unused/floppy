/// \file floppy/euclid/vector2d.h
/// \brief A two-dimensional vector tagged with a unit.
/// \author whs31

#pragma once

#include <functional>
#include <floppy/euclid/angle.h>
#include <floppy/euclid/length.h>
#include <floppy/euclid/size2d.h>
#include <floppy/euclid/detail/nt_traits2d.h>

#if defined(FL_QT_GUI)
# include <qvector2d.h>
#endif

namespace floppy::math
{
  template <typename U, concepts::num T>
  class point2d;

  /// \brief A two-dimensional vector tagged with a unit.
  /// \headerfile floppy/euclid.h
  /// \tparam U Associated unit of measurement. Default is \ref default_unit.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  /// \see floppy::math::length
  /// \see floppy::math::size2d
  /// \see floppy::math::point2d
  template <typename U = default_unit, concepts::num T = f32>
  class vector2d : public detail::basic_two_dimensional_type<vector2d<U, T>, U, T>
  {
   public:
    /// \brief Associated unit type.
    using unit_type = U;

    /// \brief Underlying number type.
    using underlying_type = T;

    /// \brief Associated size2d type.
    using size2d_type = size2d<unit_type, underlying_type>;

    /// \brief Associated angle type.
    using angle_type = angle<underlying_type>;

    /// \brief Associated length type.
    using length_type = length<unit_type, underlying_type>;

    /// \brief Constructs new vector with zero coordinates.
    constexpr vector2d() : detail::basic_two_dimensional_type<vector2d<U, T>, U, T>() {}

    /// \brief Constructs new vector from given scalar values.
    /// \param x The x-coordinate.
    /// \param y The y-coordinate.
    constexpr vector2d(underlying_type x, underlying_type y) : detail::basic_two_dimensional_type<vector2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new vector from proper <i>length</i> values.
    /// \param x The x-coordinate in <i>unit</i>.
    /// \param y The y-coordinate in <i>unit</i>.
    constexpr vector2d(length_type x, length_type y) : detail::basic_two_dimensional_type<vector2d<U, T>, U, T>(x, y) {}

    /// \brief Constructs new vector, setting all components to the same value.
    /// \param value The value to set all components to.
    constexpr explicit vector2d(underlying_type value) : detail::basic_two_dimensional_type<vector2d<U, T>, U, T>(value) {}

    /// \brief Tags a unitless value with units.
    /// \param p Unitless vector2d
    template <typename U2 = default_unit>
      requires (not std::is_same_v<U, U2>)
    constexpr explicit vector2d(vector2d<default_unit, T> const& p) : detail::basic_two_dimensional_type<vector2d<U, T>, U, T>(p.x(), p.y()){}

    /// \brief Constructs new vector from size2d.
    /// \param other The other size2d.
    [[nodiscard]] constexpr explicit vector2d(size2d_type const& other) : vector2d(other.x(), other.y()) {}

    /// \brief Constructs new vector from point2d.
    /// \param other The other point2d.
    [[nodiscard]] constexpr explicit vector2d(point2d<U, T> const& other)
      : detail::basic_two_dimensional_type<vector2d<U, T>, U, T>(other.x(), other.y())
    {}

    /// \brief Constructs new vector from angle and length.
    /// \param angle The angle in <i>unit</i>.
    /// \param length The length in <i>unit</i>.
    [[nodiscard]] constexpr explicit vector2d(angle_type angle, length_type length)
      : detail::basic_two_dimensional_type<vector2d<U, T>, U, T>(length * angle.cos(), length * angle.sin())
    {}

    /// \brief Applies the function <b>fn</b> to each component of the vector2d.
    /// \tparam F The type of function to apply.
    /// \param fn The function to apply.
    template <std::invocable<underlying_type> F>
    constexpr auto map(F&& fn) const {
      return vector2d<unit_type, decltype(fn(this->x()))>(fn(this->x()), fn(this->y()));
    }

    /// \brief Applies the function <b>fn</b> to each component of this vector2d and the other vector2d.
    /// \tparam F The type of function to apply.
    /// \param other The other vector2d to apply.
    /// \param fn The function to apply.
    template <std::invocable<underlying_type, underlying_type> F>
    constexpr auto zip(vector2d const& other, F&& fn) const -> vector2d<unit_type, decltype(fn(this->x(), other.x()))> {
      return vector2d<unit_type, decltype(fn(this->x(), other.x()))>(fn(this->x(), other.x()), fn(this->y(), other.y()));
    }

    // todo: extend

    /// \brief Casts this vector to a size2d.
    /// \return The size2d representation of this vector.
    [[nodiscard]] constexpr auto to_size2d() const { return size2d_type(this->x(), this->y()); }

    /// \brief Casts this vector to a point2d.
    /// \return The point2d representation of this vector.
    [[nodiscard]] constexpr auto to_point2d() const { return point2d(this->x(), this->y()); }

    /// \brief Drops the units from the vector, returning just the numeric scalar values.
    [[nodiscard]] constexpr auto to_untyped() const -> vector2d<default_unit, underlying_type> {
      return vector2d<default_unit, underlying_type>(this->x(), this->y());
    }

    // todo: to3d

    /// \brief Returns this vector3d's length as scalar value.
    /// \return The length of this vector3d.
    [[nodiscard]] constexpr auto length() const -> underlying_type {
      return underlying_type(std::sqrt(this->length_squared()));
    }

    /// \brief Returns this vector3d's length in associated <i>unit</i>.
    /// \return The length of this vector3d in <i>unit</i>.
    [[nodiscard]] constexpr auto length_typed() const -> length_type {
      return length_type(std::sqrt(this->length_squared()));
    }

    /// \brief Returns this vector3d's length squared.
    /// \return The length squared of this vector3d.
    [[nodiscard]] constexpr auto length_squared() const -> underlying_type {
      return this->x() * this->x() + this->y() * this->y();
    }

    /// \brief Returns this vector projected onto another vector.
    /// \param other The other vector.
    /// \return The projection of this vector onto the other vector.
    /// \warning Projecting onto a zero vector will result in a division by zero.
    [[nodiscard]] constexpr auto project(vector2d const& other) const -> vector2d {
      return other * (this->dot(other) / other.length_squared());
    }

    /// \brief Returns the signed angle between this vector and another vector.
    /// \param other The other vector.
    /// \return The signed angle between this vector and the other vector.
    [[nodiscard]] constexpr auto angle_to(vector2d const& other) const -> angle_type {
      return angle_type::from_radians(std::atan2(this->cross(other), this->dot(other)));
    }

    /// \brief Returns the signed angle between this vector and the x axis.
    /// \details Positive values counted counterclockwise, where 0 is <tt>+x</tt> axis, and <tt>PI/2</tt>
    /// is <tt>+y</tt> axis.
    /// \return The signed angle between this vector and the x axis.
    [[nodiscard]] constexpr auto angle_to_x_axis() const -> angle_type {
      return angle_type::from_radians(std::atan2(this->y(), this->x()));
    }

    /// \brief Returns the vector with length normalized.
    /// \return The normalized vector.
    [[nodiscard]] constexpr auto normalized() const -> vector2d { return *this / this->length(); }

    /// \brief Returns this vector scaled to fit the provided length.
    /// \param l The length to scale to.
    /// \return The scaled vector.
    [[nodiscard]] constexpr auto scaled_to(unit_type l) const -> vector2d { return this->normalized() * l; }

    /// \brief Returns a reflection vector using an incident ray and a surface normal.
    /// \param normal The surface normal.
    /// \return The reflection vector.
    [[nodiscard]] constexpr auto reflected(vector2d const& normal) const -> vector2d {
      return *this - normal * underlying_type(2.0) * this->dot(normal);
    }

    /// \brief Casts the unit of measurement.
    /// \tparam U2 New unit of measurement.
    /// \return The vector2d with the new unit of measurement and the same value.
    template <typename U2>
    [[nodiscard]] constexpr auto cast_unit() const -> vector2d<U2, underlying_type> {
      return vector2d<U2, underlying_type>(this->x(), this->y());
    }

    /// \brief Cast from one numeric representation to another, preserving the units.
    /// \tparam T2 New number type.
    /// \return The vector2d with the new number type and the same value.
    template <concepts::num T2>
    [[nodiscard]] constexpr auto cast() const -> vector2d<unit_type, T2> {
      return vector2d<unit_type, T2>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f32</tt> vector2d.
    [[nodiscard]] constexpr auto to_f32() const -> vector2d<unit_type, f32> {
      return vector2d<unit_type, f32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>f64</tt> vector2d.
    [[nodiscard]] constexpr auto to_f64() const -> vector2d<unit_type, f64> {
      return vector2d<unit_type, f64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u32</tt> vector2d.
    [[nodiscard]] constexpr auto to_u32() const -> vector2d<unit_type, u32> {
      return vector2d<unit_type, u32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>u64</tt> vector2d.
    [[nodiscard]] constexpr auto to_u64() const -> vector2d<unit_type, u64> {
      return vector2d<unit_type, u64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>usize</tt> vector2d.
    [[nodiscard]] constexpr auto to_usize() const -> vector2d<unit_type, usize> {
      return vector2d<unit_type, usize>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i32</tt> vector2d.
    [[nodiscard]] constexpr auto to_i32() const -> vector2d<unit_type, i32> {
      return vector2d<unit_type, i32>(this->x(), this->y());
    }

    /// \brief Casts into <tt>i64</tt> vector2d.
    [[nodiscard]] constexpr auto to_i64() const -> vector2d<unit_type, i64> {
      return vector2d<unit_type, i64>(this->x(), this->y());
    }

    /// \brief Casts into <tt>isize</tt> vector2d.
    [[nodiscard]] constexpr auto to_isize() const -> vector2d<unit_type, isize> {
      return vector2d<unit_type, isize>(this->x(), this->y());
    }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Casts this vector2d into <tt>QVector2D</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] constexpr auto to_qvector2d() const -> QVector2D { return QVector2D(this->x(), this->y()); }
  #endif

    /// \brief Constructs new vector with all values set to one.
    [[nodiscard]] static constexpr auto one() -> vector2d { return vector2d::splat(1.0); }

    /// \brief Constructs new vector from angle and length.
    /// \param angle The angle in <i>unit</i>.
    /// \param length The length in <i>unit</i>.
    [[nodiscard]] static constexpr auto from_angle_and_length(angle_type angle, length_type length) -> vector2d {
      return vector2d(angle, length);
    }

    [[nodiscard]] constexpr auto operator+() const -> vector2d { return *this; }
    [[nodiscard]] constexpr auto operator-() const -> vector2d { return vector2d(-this->x(), -this->y()); }

    [[nodiscard]] constexpr auto operator==(vector2d const& other) const -> bool {
      return approx_eq(this->x(), other.x(), static_cast<underlying_type>(3.0F))
             and approx_eq(this->y(), other.y(), static_cast<underlying_type>(3.0F));
    }

    [[nodiscard]] constexpr auto operator!=(vector2d const& other) const -> bool {
      return not approx_eq(this->x(), other.x(), static_cast<underlying_type>(3.0F))
             or not approx_eq(this->y(), other.y(), static_cast<underlying_type>(3.0F));
    }

    template <concepts::any_of<vector2d, size2d_type> Q>
    [[nodiscard]] constexpr auto operator+(Q const& other) const -> vector2d {
      return vector2d(this->x() + other.x(), this->y() + other.y());
    }

    template <concepts::any_of<vector2d, size2d_type> Q>
    [[nodiscard]] constexpr auto operator-(Q const& other) const -> vector2d {
      return vector2d(this->x() - other.x(), this->y() - other.y());
    }

    [[nodiscard]] constexpr auto operator*(underlying_type const& other) const -> vector2d {
      return vector2d(this->x() * other, this->y() * other);
    }

    template <typename U2, concepts::num T2>
    [[nodiscard]] constexpr auto operator*(scale<unit_type, U2, T2> const& other) const -> vector2d<U2, underlying_type> {
      return vector2d<U2, underlying_type>(
        this->x() * other.template as<underlying_type>(),
        this->y() * other.template as<underlying_type>()
      );
    }

    [[nodiscard]] constexpr auto operator/(underlying_type const& other) const -> vector2d {
      return vector2d(this->x() / other, this->y() / other);
    }

    template <typename U2, concepts::num T2>
    [[nodiscard]] constexpr auto operator/(scale<U2, unit_type, T2> const& other) const -> vector2d<U2, underlying_type> {
      return vector2d<U2, underlying_type>(
        this->x() / other.template as<underlying_type>(),
        this->y() / other.template as<underlying_type>()
      );
    }

    template <concepts::any_of<vector2d, size2d_type> Q>
    constexpr auto operator+=(Q const& other) -> vector2d& {
      this->x_mut() += other.x();
      this->y_mut() += other.y();
      return *this;
    }

    template <concepts::any_of<vector2d, size2d_type> Q>
    constexpr auto operator-=(Q const& other) -> vector2d& {
      this->x_mut() -= other.x();
      this->y_mut() -= other.y();
      return *this;
    }

    constexpr auto operator*=(underlying_type const& other) -> vector2d& {
      this->x_mut() *= other;
      this->y_mut() *= other;
      return *this;
    }

    constexpr auto operator/=(underlying_type const& other) -> vector2d& {
      this->x_mut() /= other;
      this->y_mut() /= other;
      return *this;
    }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Constructs new vector2d from <tt>QVector2D</tt>.
    /// \param other The other <tt>QVector2D</tt>.
    /// \remarks This constructor is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    constexpr explicit vector2d(QVector2D const& other) : vector2d(other.x(), other.y()) {}

    /// \brief Constructs new vector2d from <tt>QVector2D</tt>.
    /// \param other The other <tt>QVector2D</tt>.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    [[nodiscard]] static constexpr auto from_qvector2d(QVector2D const& other) -> vector2d { return vector2d(other.x(), other.y()); }
  #endif
  };
} // namespace floppy::math