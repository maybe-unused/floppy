#pragma once

#include "../memory/propagate_const.h"
#include "../memory/box.h"

namespace fl
{
  /**
   * @brief Pointer-to-implementation pattern trait.
   * @details Allows to use <b>Pimpl</b> pattern in custom types and classes.
   *
   * Example declaration:
   *
   * @code {.cpp}
   *    class TestPimpl : fl::pin
   *    {
   *      public:
   *         TestPimpl() = default;
   *         [[nodiscard]] auto add(int a, int b) const -> int;
   *
   *      private:
   *        struct impl;
   *        fl::pimpl<struct impl> impl_;
   *    };
   *
   *    struct TestPimpl::impl
   *    {
   *        int add(int a, int b) const { return a + b; }
   *    };
   *
   *    int TestPimpl::add(int a, int b) const {
   *      return this->impl_->add(a, b);
   *    }
   * @endcode
   * @tparam T Type to implement.
   */
  template <typename T>
  using pimpl = propagate_const<box<T>>;
} // namespace fl