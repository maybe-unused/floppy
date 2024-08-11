#pragma once

#include <string>
#include <string_view>
#include <algorithm>
#include <floppy/detail/exceptions.h>
#include <floppy/detail/cxx_ver.h>

namespace floppy // NOLINT(*-concat-nested-namespaces)
{
  /// \brief Constexpr-friendly string related functions.
  inline namespace string_utils
  {
    /// \brief Satisfied if the given type is a character type, except for <code>char8_t</code>, <code>char16_t</code> and <code>char32_t</code>.
    /// \ingroup string_utils
    /// \headerfile floppy/floppy.h
    /// \invariant Available only in C++20 or later.
    #if defined(FL_CXX20) || defined(FL_DOC)
    template <typename T>
    concept char_type = std::is_same_v<T, char>
      or std::is_same_v<T, unsigned char>
      or std::is_same_v<T, wchar_t>;
    #endif

    /// \brief Checks if the given character is a digit.
    /// \ingroup string_utils
    /// \headerfile floppy/floppy.h
    /// \tparam C The character type. Must satisfy concept \ref char_type.
    /// \param c The character to check.
    /// \returns <code>true</code> if the character is a digit, <code>false</code> otherwise.
    template <__wrap_concept__(char_type) C>
    [[nodiscard]] constexpr auto is_digit(C c) -> bool {
      if constexpr(std::is_same_v<C, wchar_t>)
        return c >= L'0' and c <= L'9';
      else {
        auto const _c = static_cast<unsigned char>(c);
        return _c >= '0' and _c <= '9';
      }
    }

    /// \brief Converts the given character to uppercase.
    /// \ingroup string_utils
    /// \headerfile floppy/floppy.h
    /// \tparam C The character type. Must satisfy concept \ref char_type.
    /// \param c The character to convert.
    /// \returns The uppercase character.
    template <__wrap_concept__(char_type) C>
    [[nodiscard]] constexpr auto to_uppercase(C c) -> C {
      if constexpr(std::is_same_v<C, wchar_t>)
        return c >= L'a' and c <= L'z' ? static_cast<wchar_t>(c - L'a' + L'A') : c;
      else
        return c >= 'a' and c <= 'z' ? static_cast<unsigned char>(c - 'a' + 'A') : c;
    }

    /// \brief Converts the given string to uppercase.
    /// \ingroup string_utils
    /// \headerfile floppy/floppy.h
    /// \tparam C The string's character type. Must satisfy concept \ref char_type.
    /// \param str The string to convert.
    /// \returns The uppercase string.
    template <__wrap_concept__(char_type) C>
    [[nodiscard]] auto to_uppercase(std::basic_string_view<C> str) -> std::basic_string<C> {
      auto result = std::basic_string<C>();
      std::transform(str.begin(), str.end(), std::back_inserter(result), to_uppercase<C>);
      return result;
    }

    /// \brief Converts the given character to lowercase.
    /// \ingroup string_utils
    /// \headerfile floppy/floppy.h
    /// \tparam C The character type. Must satisfy concept \ref char_type.
    /// \param c The character to convert.
    /// \returns The lowercase character.
    template <__wrap_concept__(char_type) C>
    [[nodiscard]] constexpr auto to_lowercase(C c) -> C {
      if constexpr(std::is_same_v<C, wchar_t>)
        return c >= L'A' and c <= L'Z' ? static_cast<wchar_t>(c - L'A' + L'a') : c;
      else
        return c >= 'A' and c <= 'Z' ? static_cast<unsigned char>(c - 'A' + 'a') : c;
    }

    /// \brief Converts the given string to lowercase.
    /// \ingroup string_utils
    /// \headerfile floppy/floppy.h
    /// \tparam C The string's character type. Must satisfy concept \ref char_type.
    /// \param str The string to convert.
    /// \returns The lowercase string.
    template <__wrap_concept__(char_type) C>
    [[nodiscard]] auto to_lowercase(std::basic_string_view<C> str) -> std::basic_string<C> {
      auto result = std::basic_string<C>();
      std::transform(str.begin(), str.end(), std::back_inserter(result), to_lowercase<C>);
      return result;
    }
  } // namespace string_utils
} // namespace floppy

/// \defgroup string_utils String utilities
/// \ingroup foundation
/// \brief Constexpr-friendly string related functions.