#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

namespace floppy::print_helpers
{
  template <typename... Args>
  auto google_test_print(fmt::format_string<Args...> format, Args&&... args) -> void {
    fmt::println("\u001b[32m[          ] \u001b[33m{}\u001b[0m", fmt::format(format, std::forward<Args>(args)...));
  }

  template <typename... Args>
  auto critical_message(std::string_view format, Args&&... args) -> void {
    fmt::print(
      fmt::emphasis::bold | fg(fmt::color::red),
      "fatal: {}\n",
      fmt::format(fmt::runtime(format), std::forward<Args>(args)...)
    );
  }
}

#define GTEST_PRINT(...) ::floppy::print_helpers::google_test_print(__VA_ARGS__)