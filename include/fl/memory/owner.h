#pragma once

#include "../concepts/ptr.h"

namespace fl
{
  /**
   * @brief GSL-like owning raw pointer typedef.
   * @details <code>fl::owner<T></code> is designed as a safety mechanism for code that must deal
   * directly with raw pointers that own memory.
   *
   * Ideally such code should be restricted to the implementation of low-level abstractions.
   * <code>fl::owner<T></code> can also be used as a stepping point in converting legacy code
   * to use more modern RAII constructs, such as smart pointers.
   * @tparam T Pointer type.
   * @sa https://github.com/microsoft/GSL/blob/main/include/gsl/pointers
   */
  template <concepts::c_ptr T>
  using owner = T;
} // namespace fl