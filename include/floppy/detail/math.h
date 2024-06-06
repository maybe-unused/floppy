#pragma once

#include <cmath>
#include <limits>
#include <numbers>
#include <floppy/detail/export.h>
#include <floppy/detail/concepts.h>

namespace floppy
{
  /// \brief Returns true if numbers are equal
  /// \details Compares floating point values using formula <tt>|a - b| <= epsilon</tt>
  /// \param a First number
  /// \param b Second number
  /// \tparam T Number type
  /// \return True if numbers are equal
  template <typename T>
  [[nodiscard]] constexpr auto eq(T a, T b) -> bool {
    if constexpr(std::is_floating_point_v<T>)
      return std::abs(a - b) <= std::numeric_limits<T>::epsilon();
    else
      return a == b;
  }

  /// \brief Returns true if type is equal to zero (number specific)
  /// \param num Number
  /// \tparam T Number type
  /// \return True if number is equal to zero
  template <concepts::num T>
  [[nodiscard]] constexpr auto null(T num) -> bool { return eq(num, T(0.0)); }

  /// \brief Converts radians to degrees
  /// \param rad Radians
  /// \tparam T Radians type
  /// \return Degrees
  template <concepts::num T>
  [[nodiscard]] constexpr auto to_radians(T deg) -> T {
    return static_cast<T>(deg * std::numbers::pi_v<T> / 180.0);
  }

  /// \brief Converts degrees to radians
  /// \param deg Degrees
  /// \tparam T Degrees type
  /// \return Radians
  template <concepts::num T>
  [[nodiscard]] constexpr auto to_degrees(T rad) -> T {
    return static_cast<T>(rad * 180.0 / std::numbers::pi_v<T>);
  }

  /// \brief Returns logarithm of a number in a given base.
  /// \param base Base
  /// \param num Number
  /// \tparam T Number type
  /// \return Logarithm of the number in the given base
  template <std::floating_point T>
  [[nodiscard]] constexpr auto log(T base, T num) -> T { return std::log(num) / std::log(base); }

  /// \brief Returns natural logarithm of a number.
  /// \param num Number
  /// \tparam T Number type
  /// \return Natural logarithm of the number
  template <concepts::num T>
  [[nodiscard]] constexpr auto log(T num) -> T { return std::log(num); }
} // namespace floppy