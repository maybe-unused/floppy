#pragma once

#include <type_traits>

namespace floppy
{
  /// \brief Remove all cv qualifiers, references and pointers from a type.
  /// \headerfile floppy/floppy.h
  /// \ingroup helpers
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
    /// \brief The type without qualifiers, references and pointers.
    using type = std::remove_cvref_t<std::remove_pointer_t<T>>;

    /// \brief The type without qualifiers, references and pointers with a const qualifier.
    using const_type = std::add_const_t<type>;
  };

  /// \brief Remove all cv qualifiers, references and pointers from a type.
  /// \headerfile floppy/floppy.h
  /// \ingroup helpers
  template <typename T>
  using plain_type_t = typename plain_type<T>::type;

  /// \brief Checks if type is one of the given types.
  /// \headerfile floppy/floppy.h
  /// \ingroup helpers
  /// \tparam U The type to check.
  /// \tparam T The types to check against.
  /// \see one_of_v
  /// \see floppy::concepts::any_of
  template <typename U, typename... T>
  struct one_of {
    /// \brief The result of the check.
    static constexpr bool value = std::disjunction_v<std::is_same<U, T>...>;

    /// \brief The result of the check as a boolean.
    constexpr explicit operator bool() const noexcept { return value; }
  };

  /// \brief Checks if type is one of the given types.
  /// \headerfile floppy/floppy.h
  /// \ingroup helpers
  /// \tparam U The type to check.
  /// \tparam T The types to check against.
  /// \see one_of
  /// \see floppy::concepts::any_of
  template <typename U, typename... T>
  constexpr bool one_of_v = one_of<U, T...>::value;

  static_assert(not one_of_v<char, int, int>);
  static_assert(one_of_v<std::string, std::basic_string<char>, std::basic_string<wchar_t>>);
  static_assert(not one_of_v<std::u8string, std::basic_string<char>, std::basic_string<wchar_t>>);
} // namespace floppy