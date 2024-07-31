#pragma once

#include <cmath>
#include <limits>
#include <numbers>
#include <compare>
#include <floppy/detail/export.h>
#include <floppy/detail/concepts.h>

/// \brief Math namespace.
namespace floppy::math
{
  /// \brief Returns true if numbers are equal.
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \details Compares floating point values using formula <tt>|a - b| <= epsilon</tt>
  /// \param a First number
  /// \param b Second number
  /// \tparam T Number type
  /// \return True if numbers are equal
  /// \see is_null, approx_eq, strong_compare
  template <typename T>
  [[nodiscard]] constexpr auto eq(T a, T b) -> bool {
    if constexpr(std::is_floating_point_v<T>)
      return std::abs(a - b) <= std::numeric_limits<T>::epsilon();
    else
      return a == b;
  }

  /// \brief Returns true if numbers are approximately equal.
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \details Compares floating point values using formula <tt>|a - b| <= epsilon * epsilon_factor</tt>.
  /// \param a First number
  /// \param b Second number
  /// \param epsilon_factor Epsilon factor for comparison
  /// \tparam T Number type
  /// \return True if numbers are approximately equal
  /// \see is_null, eq, strong_compare
  template <typename T>
  [[nodiscard]] constexpr auto approx_eq(T a, T b, T epsilon_factor) -> bool {
    if constexpr(std::is_floating_point_v<T>)
      return std::abs(a - b) <= std::numeric_limits<T>::epsilon() * epsilon_factor;
    else
      return a == b;
  }

  /// \brief Returns true if type is equal to zero (number specific).
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param num Number
  /// \tparam T Number type
  /// \return True if number is equal to zero
  /// \see eq, approx_eq, strong_compare
  template <concepts::num T>
  [[nodiscard]] constexpr auto is_null(T num) -> bool { return eq(num, T(0.0)); }

  /// \brief Three-ways compare two numbers, forcing them to <i>strong_order</i>.
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param a First number
  /// \param b Second number
  /// \tparam T Number type
  /// \note Treats <tt>NaN</tt> as equal to <tt>NaN</tt> and <tt>inf</tt> as equal to <tt>inf</tt> in case of floating points.
  /// \see eq, approx_eq, is_null
  /// \return Comparison result
  template <concepts::num T>
  [[nodiscard]] constexpr auto strong_compare(T a, T b) -> std::strong_ordering {
    if constexpr(std::is_floating_point_v<T>) {
      if(std::isinf(a) and std::isinf(b))
        return std::strong_ordering::equal;
      if(std::isnan(a) or std::isnan(b))
        return std::strong_ordering::equal;
      return std::abs(a - b) <= std::numeric_limits<T>::epsilon()
        ? std::strong_ordering::equal
        : (a < b
            ? std::strong_ordering::less
            : std::strong_ordering::greater
          );
    }
    else
      return a == b
        ? std::strong_ordering::equal
        : (a < b
            ? std::strong_ordering::less
            : std::strong_ordering::greater
          );
  }
  
  /// \brief Converts degrees to radians
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param deg Degrees
  /// \tparam T Degrees type
  /// \return Radians
  /// \see to_degrees
  template <concepts::num T>
  [[nodiscard]] constexpr auto to_radians(T deg) -> T {
    return static_cast<T>(deg * std::numbers::pi_v<T> / 180.F);
  }

  /// \brief Converts radians to degrees
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param rad Radians
  /// \tparam T Radians type
  /// \return Degrees
  /// \see to_radians
  template <concepts::num T>
  [[nodiscard]] constexpr auto to_degrees(T rad) -> T {
    return static_cast<T>(rad * 180.F / std::numbers::pi_v<T>);
  }

  /// \brief Returns logarithm of a number in a given base.
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param base Base
  /// \param num Number
  /// \tparam T Number type
  /// \return Logarithm of the number in the given base
  template <std::floating_point T>
  [[nodiscard]] constexpr auto log(T base, T num) -> T { return std::log(num) / std::log(base); }

  /// \brief Returns natural logarithm of a number.
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param num Number
  /// \tparam T Number type
  /// \return Natural logarithm of the number
  template <concepts::num T>
  [[nodiscard]] constexpr auto log(T num) -> T { return std::log(num); }

  /// \brief Calculates Euclidean division, the matching method for <b>rem_euclid</b>.
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param a Dividend
  /// \param b Divisor
  template <concepts::num T>
  [[nodiscard]] auto div_euclid(T a, T b) -> T {
    auto const q = std::trunc(a / b);
    if constexpr(std::is_floating_point_v<T>) {
      if(std::fmod(a, b) < 0.0)
        return b > 0.0 ? q - 1.0 : q + 1.0;
    } else {
      if(a % b < 0)
        return b > 0 ? q - 1 : q + 1;
    }
    return q;
  }

  /// \brief Calculates the least nonnegative remainder of <tt>self (mod rhs)</tt>.
  /// \headerfile floppy/floppy.h
  /// \ingroup calc
  /// \param a Dividend
  /// \param b Divisor
  template <concepts::num T>
  [[nodiscard]] auto rem_euclid(T a, T b) -> T {
    if constexpr(std::is_floating_point_v<T>) {
      auto const r = std::fmod(a, b);
      return r < 0.0 ? r + std::abs(b) : r;
    } else {
      auto const r = a % b;
      return r < 0 ? r + std::abs(b) : r;
    }
  }
} // namespace floppy::math

/// \defgroup calc Mathematical functions
/// \brief Mathematical functions and classes