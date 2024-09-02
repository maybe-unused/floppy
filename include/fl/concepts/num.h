#pragma once

#include <concepts>
#include "../type_traits.h"

namespace fl
{
  /**
   * @brief Concepts namespace.
   */
  namespace concepts {} // namespace concepts
} // namespace fl

namespace fl::concepts
{
  /**
   * @brief Concept that is true if T is a number.
   * @details Will yield <code>true</code> for any type that is an integral or floating point type,
   * except for <code>bool</code> and <code>std::nullptr_t</code>.
   * @note This concept will also yield <code>true</code> for references to numbers.
   * @tparam T Type to check.
   */
  template <typename T>
  concept num = std::integral<plain_type_t<T>>
    or std::floating_point<plain_type_t<T>>
    and (not std::is_same_v<plain_type_t<T>, bool>)
    and (not std::is_same_v<plain_type_t<T>, std::nullptr_t>);
} // namespace fl::concepts