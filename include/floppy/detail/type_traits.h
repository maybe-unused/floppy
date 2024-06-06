#pragma once

#include <type_traits>

namespace floppy
{
  /**
   * \brief Remove all cv qualifiers, references and pointers from a type.
   * \details E.g: <br> - <tt>std::string&</tt> becomes <tt>std::string</tt>;<br>
   * - <tt>const int*</tt> becomes <tt>int</tt>.
   * - <tt>int**</tt> becomes <tt>int</tt>.
   * - <tt>const volatile std::string**</tt> becomes <tt>std::string</tt>.
   * \note Use <tt>plain_type_t<T>::maybe_pointer_type</tt> instead of <tt>plain_type_t<T>::type</tt>
   * if you want to keep pointer, but remove cv and references.
   * \tparam T The type to remove qualifiers from.
   * \sa plain_type_t
   */
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
};