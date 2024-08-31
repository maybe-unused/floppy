#pragma once

#include <concepts>
#include <type_traits>

namespace fl::concepts
{
  template <typename T>
  concept standard_copyable_and_movable = std::is_nothrow_move_constructible_v<T>
    and std::is_nothrow_move_assignable_v<T>
    and std::copyable<T>;
} // namespace fl::concepts