#pragma once

#include <cmath>
#include <limits>
#include <numbers>
#include <floppy/detail/export.h>
#include <floppy/detail/concepts.h>

namespace floppy
{
  /// \brief Numbers namespace.
  inline namespace numbers
  {
    namespace detail
    {
      template <concepts::num T, f64 V>
      struct number_base
      {
        using type = T;
        static constexpr type value = T{V};

        constexpr number_base() = default;
        constexpr operator type() const { return this->value; }

        template <concepts::num U>
        constexpr auto operator-(U const& other) const -> type { return this->value - other; }

        template <concepts::num U>
        constexpr auto operator*(U const& other) const -> type { return this->value * other; }

        template <concepts::num U>
        constexpr auto operator/(U const& other) const -> type { return this->value / other; }

        template <concepts::num U>
        constexpr auto operator+(U const& other) const -> type { return this->value + other; }
      };
    } // namespace detail

    template <concepts::num T = f64>
    struct pi : detail::number_base<T, std::numbers::pi>
    {
     public:
      [[maybe_unused]]
      [[nodiscard]]
      constexpr auto as_degrees() const -> T { return 180.0; } // NOLINT(*-magic-numbers)
    };
  }

  /// \brief Returns true if numbers are equal
  /// \details Compares floating point values using formula <tt>|a - b| <= epsilon</tt>
  /// \param a First number
  /// \param b Second number
  /// \tparam T Number type
  /// \return True if numbers are equal
  /// \see null
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
  /// \see eq
  template <concepts::num T>
  [[nodiscard]] constexpr auto null(T num) -> bool { return eq(num, T(0.0)); }

  /// \brief Converts radians to degrees
  /// \param rad Radians
  /// \tparam T Radians type
  /// \return Degrees
  /// \see to_radians
  template <concepts::num T>
  [[nodiscard]] constexpr auto to_radians(T deg) -> T {
    return static_cast<T>(deg * std::numbers::pi_v<T> / pi<T>().as_degrees());
  }

  /// \brief Converts degrees to radians
  /// \param deg Degrees
  /// \tparam T Degrees type
  /// \return Radians
  /// \see to_degrees
  template <concepts::num T>
  [[nodiscard]] constexpr auto to_degrees(T rad) -> T {
    return static_cast<T>(rad * pi<T>().as_degrees() / std::numbers::pi_v<T>);
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