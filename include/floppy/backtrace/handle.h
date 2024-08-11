#pragma once

#include <utility>
#include <floppy/detail/memory.h>

namespace floppy::stacktrace::details
{
  template <typename T, typename Deleter = deleter<void, void*, &::free> >
  class handle
  {
    struct dummy;
    T _val;
    bool _empty;

   public:
    handle(handle const&) = delete;
    auto operator=(handle const&) -> handle& = delete;

    ~handle() {
      if(not this->_empty)
        Deleter()(_val);
    }

    explicit handle()
      : _val()
      , _empty(true)
    {}

    explicit handle(T val)
      : _val(val)
      , _empty(false) {
      if(not this->_val)
        this->_empty = true;
    }

    handle(handle&& from) noexcept
      : _empty(true) {
      this->swap(from);
    }

    auto operator=(handle&& from) noexcept -> handle& {
      this->swap(from);
      return *this;
    }

    auto reset(T new_val) -> void {
      auto tmp = handle(new_val);
      this->swap(tmp);
    }

    auto update(T new_val) -> void {
      this->_val = new_val;
      this->_empty = not static_cast<bool>(new_val);
    }

    explicit operator const dummy*() const {
      if(this->_empty)
        return nullptr;
      return reinterpret_cast<dummy const*>(this->_val); // NOLINT(*-pro-type-reinterpret-cast)
    }

    auto get() -> T {
      return this->_val;
    }

    [[maybe_unused]] auto release() -> T {
      this->_empty = true;
      return this->_val;
    }

    auto swap(handle& b) noexcept -> void {
      std::swap(b._val, this->_val);
      std::swap(b._empty, this->_empty);
    }

    auto operator->() -> T& { return this->_val; }
    auto operator->() const -> T const& { return this->_val; }

    using ref_t = std::remove_pointer_t<T>&;
    using const_ref_t = std::remove_pointer_t<T> const&;

    auto operator*() -> ref_t { return *this->_val; }
    auto operator*() const -> const_ref_t { return *this->_val; }
    auto operator[](size_t idx) -> ref_t { return this->_val[idx]; }
    auto operator&() -> T* {
      this->_empty = false;
      return &this->_val;
    }

    explicit operator bool() const { return not this->_empty; }
  };
} // namespace floppy::stacktrace::details