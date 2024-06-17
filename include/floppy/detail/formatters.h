#pragma once

#include <string_view>
#include <sstream>
#include <iosfwd>
#include <fmt/format.h>

namespace floppy
{
  /// \brief Formatter for types which support stream output.
  /// \details Example usage for <tt>std::ostream</tt>:
  ///
  /// \code {.cpp}
  /// template <> struct fmt::formatter<YourType> : floppy::ostream_formatter<char> {};
  /// \endcode
  ///
  /// Example usage for <tt>std::wostream</tt>:
  ///
  /// \code {.cpp}
  /// template <> struct fmt::formatter<YourType> : floppy::ostream_formatter<wchar_t> {};
  /// \endcode
  ///
  /// \tparam C Type of the stream character type.
  template <typename C>
  struct [[maybe_unused]] ostream_formatter : fmt::formatter<std::basic_string_view<C>, C>
  {
    template <typename T, typename OutputIt>
    auto format(T const& value, fmt::basic_format_context<OutputIt, C>& ctx) const -> OutputIt {
      auto ss = std::basic_stringstream<C>();
      ss << value;
      return fmt::formatter<std::basic_string_view<C>, C>::format(ss.str(), ctx);
    }
  };
} // namespace floppy

// todo: formatters for version and meta