#pragma once

#include <fmt/format.h>

namespace floppy::print_helpers
{
  template <typename... Args>
  auto google_test_print(std::string_view format, Args&&... args) -> void {
    auto str = fmt::format(fmt::runtime(format), std::forward<Args>(args)...);
    fmt::println("\u001b[32m[          ] \u001b[33m{}\u001b[0m", str);
  }
}

#define GTEST_PRINT(...) ::floppy::print_helpers::google_test_print(__VA_ARGS__)