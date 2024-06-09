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
  /// \see null
  template <typename T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  [[nodiscard]] constexpr auto eq(T a, T b) -> bool {
#else
  [[nodiscard]] constexpr bool eq(T a, T b) {
#endif
    if constexpr(std::is_floating_point_v<T>)
      return std::abs(a - b) <= std::numeric_limits<T>::epsilon();
    else
      return a == b;
  }

  /// \brief Returns true if type is equal to zero (number specific)
  /// \param num Number
  /// \tparam T Number type
  /// \return True if number is equal to zero
  /// \see eq
  template <concepts::num T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  [[nodiscard]] constexpr auto null(T num) -> bool { return eq(num, T(0.0)); }
#else
  [[nodiscard]] constexpr bool null(T num) { return eq(num, T(0.0)); }
#endif

  /// \brief Converts radians to degrees
  /// \param rad Radians
  /// \tparam T Radians type
  /// \return Degrees
  /// \see to_radians
  template <concepts::num T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  [[nodiscard]] constexpr auto to_radians(T deg) -> T {
#else
  [[nodiscard]] constexpr T to_radians(T deg) {
#endif
    return static_cast<T>(deg * std::numbers::pi_v<T> / 180.0);
  }

  /// \brief Converts degrees to radians
  /// \param deg Degrees
  /// \tparam T Degrees type
  /// \return Radians
  /// \see to_degrees
  template <concepts::num T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  [[nodiscard]] constexpr auto to_degrees(T rad) -> T {
#else
  [[nodiscard]] constexpr T to_degrees(T rad) {
#endif
    return static_cast<T>(rad * 180.0 / std::numbers::pi_v<T>);
  }

  /// \brief Returns logarithm of a number in a given base.
  /// \param base Base
  /// \param num Number
  /// \tparam T Number type
  /// \return Logarithm of the number in the given base
  template <std::floating_point T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  [[nodiscard]] constexpr auto log(T base, T num) -> T { return std::log(num) / std::log(base); }
#else
  [[nodiscard]] constexpr T log(T base, T num) { return std::log(num) / std::log(base); }
#endif

  /// \brief Returns natural logarithm of a number.
  /// \param num Number
  /// \tparam T Number type
  /// \return Natural logarithm of the number
  template <concepts::num T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  [[nodiscard]] constexpr auto log(T num) -> T { return std::log(num); }
#else
  [[nodiscard]] constexpr T log(T num) { return std::log(num); }
#endif
} // namespace floppy