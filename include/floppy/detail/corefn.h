#pragma once

#include <typeinfo>
#include <floppy/detail/export.h>
#include <floppy/detail/backports.h>
#include <floppy/detail/concepts.h>
#include <floppy/detail/print_helpers.h>

namespace floppy
{
  /**
   * \brief Terminates program execution with detailed error message.
   * \details Logs the error message and terminates program execution via <tt>std::terminate</tt>
   * \note You can catch the panic via <tt>std::set_terminate</tt> custom handler, but it's not recommended.
   * \sa https://en.cppreference.com/w/cpp/error/terminate
   * \sa https://en.cppreference.com/w/cpp/error/terminate_handler
   * \param message Error message.
   * \param location Source location of the panic. Defaults to current location.
   */
  [[noreturn]] inline auto panic(std::string_view message, source_location location = source_location::current()) -> void {
    print_helpers::critical_message("panic in file `{}` line {}", location.file_name(), location.line());
    print_helpers::critical_message("in function `{}`", location.function_name());
    print_helpers::critical_message("reason: {}", message);
    print_helpers::critical_message("terminate will be called now.");
    std::terminate();
  }

  /**
   * \brief Converts number type to another number type.
   * \warning This function doesn't check for overflow or underflow.
   * \tparam T Target number type
   * \tparam U Source number type
   * \param u Source number value
   * \return Converted number
   */
  template <concepts::num T, concepts::num U>
  constexpr auto narrow_cast(U&& u) noexcept -> T {
    return static_cast<T>(std::forward<U>(u));
  }

  /// \brief Namespace with RTTI-related functions.
  namespace rtti
  {
    /// \brief Demangles C++ type name from string.
    [[nodiscard]] FLOPPY_EXPORT auto demangle(char const* name) -> std::string;

    /// \brief Returns C++ type name or signature from <b>typeid</b> object using RTTI.
    template <typename T>
    [[nodiscard]] auto type_name() -> std::string {
      return rtti::demangle(typeid(T).name());
    }
  } // namespace rtti
} // namespace floppy