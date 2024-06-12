#pragma once

#include <type_traits>

namespace floppy
{
  /// \brief Remove all cv qualifiers, references and pointers from a type.
  /// \details E.g: <br> - <tt>std::string&</tt> becomes <tt>std::string</tt>;<br>
  /// - <tt>const int*</tt> becomes <tt>int</tt>.
  /// - <tt>int**</tt> becomes <tt>int</tt>.
  /// - <tt>const volatile std::string**</tt> becomes <tt>std::string</tt>.
  /// \note Use <tt>plain_type_t<T>::maybe_pointer_type</tt> instead of <tt>plain_type_t<T>::type</tt>
  /// if you want to keep pointer, but remove cv and references.
  /// \tparam T The type to remove qualifiers from.
  /// \sa plain_type_t
  template <typename T>
  struct plain_type
  {
    using type = std::remove_cvref_t<std::remove_pointer_t<T>>;
    using const_type = std::add_const_t<type>;
    using maybe_pointer_type = std::remove_cvref_t<type>;
  };

  /// \brief Remove all cv qualifiers, references and pointers from a type.
  template <typename T>
  using plain_type_t = typename plain_type<T>::type;

  /// \brief Checks if type is one of the given types.
  /// \tparam U The type to check.
  /// \tparam T The types to check against.
  /// \sa one_of_v
  template <typename U, typename... T>
  struct one_of {
    static constexpr bool value = std::disjunction_v<std::is_same<U, T>...>;
    constexpr explicit operator bool() const noexcept { return value; }
  };

  /// \brief Checks if type is one of the given types.
  /// \tparam U The type to check.
  /// \tparam T The types to check against.
  /// \sa one_of
  template <typename U, typename... T>
  constexpr bool one_of_v = one_of<U, T...>::value;

  static_assert(not one_of_v<char, int, int>);
  static_assert(one_of_v<std::string, std::basic_string<char>, std::basic_string<wchar_t>>);
  static_assert(not one_of_v<std::u8string, std::basic_string<char>, std::basic_string<wchar_t>>);
} // namespace floppy