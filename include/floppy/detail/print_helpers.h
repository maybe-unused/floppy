#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

/// \brief Print helpers namespace.
namespace floppy::print_helpers
{
  /// \brief Print helpers internal namespace.
  namespace detail
  {
    /// \brief Prints a message to the console with google test style.
    /// \param format The format string.
    /// \param args The format arguments.
    /// \tparam Args The format arguments types (inferred from the format string and args).
    template <typename... Args>
#ifndef DOXYGEN_GENERATING_OUTPUT
    auto google_test_print(fmt::format_string<Args...> format, Args&&... args) -> void {
#else
    void google_test_print(fmt::format_string<Args...> format, Args&&... args) {
#endif
      fmt::println("\u001b[32m[          ] \u001b[33m{}\u001b[0m", fmt::format(format, std::forward<Args>(args)...));
    }
  } // namespace detail

  /// \brief Prints a message to stderr with error formatting.
  /// \param format The format string.
  /// \param args The format arguments.
  /// \tparam Args The format arguments types (inferred from the format string and args).
  template <typename... Args>
#ifndef DOXYGEN_GENERATING_OUTPUT
  auto critical_message(std::string_view format, Args&&... args) -> void {
#else
  void critical_message(std::string_view format, Args&&... args) {
#endif
    fmt::print(
      stderr,
      fmt::emphasis::bold | fg(fmt::color::red),
      "fatal: {}\n",
      fmt::format(fmt::runtime(format), std::forward<Args>(args)...)
    );
  }
}

/// \defgroup macros Macros

/// \ingroup macros
/// \brief Prints a message to the console with google test style.
/// \details Use this macro to print messages in google test style.
/// Example:
/// \code {.cpp}
/// GTEST_PRINT("Hello, {}!", "World");
/// GTEST_PRINT("Hello, {}!", 42);
/// \endcode
/// Output:
/// \code
/// [          ] Hello, World!
/// [          ] Hello, 42!
/// \endcode
#define GTEST_PRINT(...) ::floppy::print_helpers::detail::google_test_print(__VA_ARGS__)