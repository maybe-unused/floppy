#pragma once

#include <utility>
#include <floppy/backtrace/handle.h>
#include <floppy/detail/platform.h>
#include <floppy/detail/corefn.h>

/// \brief Implementation details of stacktrace functionality.
namespace floppy::stacktrace::details
{
  template <typename Tag>
  struct demangler_impl
  {
    [[nodiscard]] static inline auto demangle(char const* func_name) -> std::string {
      return func_name;
    }
  };

  #if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
    template <> struct demangler_impl<system_tag::current>
    {
      demangler_impl() = default;

      [[nodiscard]] static inline auto demangle(char const* func_name) -> std::string {
        return rtti::demangle(func_name);
      }
    };
  #endif // FLOPPY_OS_LINUX || FLOPPY_OS_LINUX

  struct demangler : public demangler_impl<system_tag::current> {};

  /// \brief Split a string on the platform's PATH delimiter.
  /// \details Example: if delimiter is ":" then:
  /// \code
  ///   ""              --> []
  ///   ":"             --> ["",""]
  ///   "::"            --> ["","",""]
  ///   "/a/b/c"        --> ["/a/b/c"]
  ///   "/a/b/c:/d/e/f" --> ["/a/b/c","/d/e/f"]
  /// \endcode
  /// and etc.
  /// \param s String to split
  /// \return Vector of strings
  inline auto split_source_prefixes(std::string const& s) -> std::vector<std::string> {
    auto out = std::vector<std::string>();
    auto last = static_cast<std::size_t>(0);
    auto next = static_cast<std::size_t>(0);
    auto const delimiter_size = 1;
    while((next = s.find(platform::current().backward_path_delimiter, last)) != std::string::npos) {
      out.push_back(s.substr(last, next - last));
      last = next + delimiter_size;
    }
    if(last <= s.length())
      out.push_back(s.substr(last));
    return out;
  }
} // namespace floppy::stacktrace::details