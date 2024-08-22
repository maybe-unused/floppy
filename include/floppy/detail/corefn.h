#pragma once

#include <typeinfo>
#include <floppy/detail/export.h>
#include <floppy/detail/backports.h>
#include <floppy/detail/concepts.h>
#include <floppy/detail/print_helpers.h>

namespace floppy
{
  /// \brief Terminates program execution with detailed error message.
  /// \headerfile floppy/floppy.h
  /// \ingroup helpers
  /// \details Logs the error message and terminates program execution via <tt>std::terminate</tt>
  /// \note You can catch the panic via <tt>std::set_terminate</tt> custom handler, but it's not recommended.
  /// \sa https://en.cppreference.com/w/cpp/error/terminate
  /// \sa https://en.cppreference.com/w/cpp/error/terminate_handler
  /// \param message Error message.
  /// \param location Source location of the panic. Defaults to current location.
  [[noreturn]] inline void panic(std::string_view message, source_location location = source_location::current()) {
    print_helpers::critical_message("panic in file `{}` line {}", location.file_name(), location.line());
    print_helpers::critical_message("in function `{}`", location.function_name());
    print_helpers::critical_message("reason: {}", message);
    print_helpers::critical_message("terminate will be called now.");
    std::terminate();
  }

  /// \brief Converts number type to another number type.
  /// \headerfile floppy/floppy.h
  /// \ingroup helpers
  /// \warning This function doesn't check for overflow or underflow.
  /// \tparam T Target number type
  /// \tparam U Source number type
  /// \param u Source number value
  /// \return Converted number
  template <concepts::num T, concepts::num U>
  constexpr T narrow_cast(U&& u) noexcept {
    return static_cast<T>(std::forward<U>(u));
  }

  /// \brief Namespace with RTTI-related functions.
  namespace rtti
  {
    /// \brief Demangles C++ type name from string.
    /// \headerfile floppy/floppy.h
    /// \ingroup helpers
    /// \invariant Confirmed support on:
    /// <ul>
    ///   <li>MSVC</li>
    ///   <li>Clang</li>
    ///   <li>GCC</li>
    /// </ul>
    /// \see type_name
    [[nodiscard]] FLOPPY_EXPORT std::string demangle(char const* name);

    /// \brief Returns C++ type name or signature from <b>typeid</b> object using RTTI.
    /// \headerfile floppy/floppy.h
    /// \ingroup helpers
    /// \details Example usage:
    /// \code {.cpp}
    /// auto name = rtti::type_name<int>();
    /// assert(name == "int");
    ///
    /// auto name = rtti::type_name<std::vector<int>>();
    /// assert(name == "std::vector<int>");
    /// \endcode
    /// \invariant Confirmed support on:
    /// <ul>
    ///   <li>MSVC</li>
    ///   <li>Clang</li>
    ///   <li>GCC</li>
    /// </ul>
    /// \see demangle
    template <typename T>
    [[nodiscard]] std::string type_name() {
      return rtti::demangle(typeid(T).name());
    }
  } // namespace rtti

  /// \brief Generic bitwise <b>or</b> operator for enum types.
  /// \details Requires opt-ins <code>enable_bitwise_or</code> or <code>enum_flag</code> to be defined
  /// for the enum type.
  /// Example:
  /// \code {.cpp}
  /// enum class Example {
  ///   A = 1,
  ///   B = 2,
  ///   C = 4
  /// };
  ///
  /// // opt-in bitwise or
  /// void consteval enable_bitwise_or(Example);
  ///
  /// // alternatively, opt-in all operators
  /// void consteval enum_flag(Example);
  ///
  /// auto value = Example::A | Example::B;
  /// \endcode
  /// \ingroup helpers
  /// \headerfile floppy/floppy.h
  /// \tparam T Enum type
  /// \param lhs Left operand
  /// \param rhs Right operand
  /// \return Result of bitwise or operation
  template <typename T>
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_or(e);
  } or requires(T e) {
    enum_flag(e);
  })
  constexpr T operator|(T const lhs, T const rhs) noexcept {
    return static_cast<T>(to_underlying(lhs) | to_underlying(rhs));
  }

  /// \brief Generic bitwise <b>and</b> operator for enum types.
  /// \details Requires opt-ins <code>enable_bitwise_and</code> or <code>enum_flag</code> to be defined
  /// for the enum type.
  /// Example:
  /// \code {.cpp}
  /// enum class Example {
  ///   A = 1,
  ///   B = 2,
  ///   C = 4
  /// };
  ///
  /// // opt-in bitwise and
  /// void consteval enable_bitwise_and(Example);
  ///
  /// // alternatively, opt-in all operators
  /// void consteval enum_flag(Example);
  ///
  /// auto value = Example::A & Example::B;
  /// \endcode
  /// \ingroup helpers
  /// \headerfile floppy/floppy.h
  /// \tparam T Enum type
  /// \param lhs Left operand
  /// \param rhs Right operand
  /// \return Result of bitwise and operation
  template <typename T>
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_and(e);
  } or requires(T e) {
    enum_flag(e);
  })
  constexpr T operator&(T const lhs, T const rhs) noexcept {
    return static_cast<T>(to_underlying(lhs) & to_underlying(rhs));
  }

  /// \brief Generic bitwise <b>xor</b> operator for enum types.
  /// \details Requires opt-ins <code>enable_bitwise_xor</code> or <code>enum_flag</code> to be defined
  /// for the enum type.
  /// Example:
  /// \code {.cpp}
  /// enum class Example {
  ///   A = 1,
  ///   B = 2,
  ///   C = 4
  /// };
  ///
  /// // opt-in bitwise or
  /// void consteval enable_bitwise_xor(Example);
  ///
  /// // alternatively, opt-in all operators
  /// void consteval enum_flag(Example);
  ///
  /// auto value = Example::A ^ Example::B;
  /// \endcode
  /// \ingroup helpers
  /// \headerfile floppy/floppy.h
  /// \tparam T Enum type
  /// \param lhs Left operand
  /// \param rhs Right operand
  /// \return Result of bitwise xor operation
  template <typename T>
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_xor(e);
  } or requires(T e) {
    enum_flag(e);
  })
  constexpr T operator^(T const lhs, T const rhs) noexcept {
    return static_cast<T>(to_underlying(lhs) ^ to_underlying(rhs));
  }

  /// \brief Generic bitwise <b>not</b> operator for enum types.
  /// \details Requires opt-ins <code>enable_bitwise_not</code> or <code>enum_flag</code> to be defined
  /// for the enum type.
  /// Example:
  /// \code {.cpp}
  /// enum class Example {
  ///   A = 1,
  ///   B = 2,
  ///   C = 4
  /// };
  ///
  /// // opt-in bitwise not
  /// void consteval enable_bitwise_not(Example);
  ///
  /// // alternatively, opt-in all operators
  /// void consteval enum_flag(Example);
  ///
  /// auto value = ~Example::A;
  /// \endcode
  /// \ingroup helpers
  /// \headerfile floppy/floppy.h
  /// \tparam T Enum type
  /// \param lhs Left operand
  /// \param rhs Right operand
  /// \return Result of bitwise not operation
  template <typename T>
  requires(std::is_enum_v<T> and requires(T e) {
    enable_bitwise_not(e);
  } or requires(T e) {
    enum_flag(e);
  })
  constexpr T operator~(T const value) noexcept {
    return static_cast<T>(~to_underlying(value));
  }
} // namespace floppy

/// \defgroup helpers Helpers
/// \brief Useful functions for your C++ code!