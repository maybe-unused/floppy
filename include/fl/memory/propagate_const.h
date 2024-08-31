#pragma once

#include <utility>
#include "../concepts/ptr.h"

namespace fl
{
  template <concepts::smart_ptr T>
  class propagate_const : public T
  {
   public:
    using T::operator=;

    propagate_const() = default;

    template <typename... Args>
    inline propagate_const(Args&&... args)
      : T(std::forward<Args>(args)...)
    {}

    [[nodiscard]] inline typename T::element_type& operator*() {
      return T::operator*();
    }

    [[nodiscard]] inline typename T::element_type const& operator*() const {
      return T::operator*();
    }

    [[nodiscard]] inline typename T::element_type* operator->() {
      return T::operator->();
    }

    [[nodiscard]] inline typename T::element_type const* operator->() const {
      return T::operator->();
    }

    [[nodiscard]] inline typename T::element_type* get() {
      return T::get();
    }

    [[nodiscard]] inline typename T::element_type const* get() const {
      return T::get();
    }

    [[nodiscard]] inline operator bool() const noexcept {
      return T::operator bool();
    }
  };
} // namespace fl