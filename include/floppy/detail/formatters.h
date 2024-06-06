#pragma once

#include <string_view>
#include <sstream>
#include <iosfwd>
#include <fmt/format.h>

namespace floppy
{
  template <typename C>
  struct ostream_formatter : fmt::formatter<std::basic_string_view<C>, C>
  {
    template <typename T, typename OutputIt>
    inline auto format(T const& value, fmt::basic_format_context<OutputIt, C>& ctx) const -> OutputIt {
      auto ss = std::basic_stringstream<C>();
      ss << value;
      return fmt::formatter<std::basic_string_view<C>, C>::format(ss.str(), ctx);
    }
  };
} // namespace floppy