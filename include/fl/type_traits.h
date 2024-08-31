#pragma once

#include <type_traits>

namespace fl
{
  /**
   * @brief Remove all cv qualifiers, references and pointers from a type.
   * @details For example:
   * <ul>
   *    <li><tt>std::string&</tt> becomes <tt>std::string</tt></li>
   *    <li><tt>const int*</tt> becomes <tt>int</tt></li>
   *    <li><tt>int**</tt> becomes <tt>int</tt></li>
   *    <li><tt>const volatile std::string**</tt> becomes <tt>std::string</tt></li>
   * </ul>
   * @tparam T The type to remove qualifiers from.
   * @see plain_type_t
   */
  template <typename T>
  struct plain_type
  {
    /**
     * @brief The type without qualifiers, references and pointers.
     */
    using type = std::remove_cvref_t<std::remove_pointer_t<T>>;

    /**
     * @brief The type without qualifiers, references and pointers with a const qualifier.
     */
    using const_type = std::add_const_t<type>;
  };

  /**
   * @brief Remove all cv qualifiers, references and pointers from a type.
   * @see plain_type
   */
  template <typename T>
  using plain_type_t = typename plain_type<T>::type;
} // namespace fl