#pragma once

#include <vector>
#include <floppy/backtrace/trace.h>

namespace floppy::stacktrace::impl
{
  template <typename Tag>
  class stack_trace_impl
  {
   public:
    [[nodiscard]] auto size() const -> size_t { return 0; }
    [[nodiscard]] auto operator[]([[maybe_unused]] size_t _) const -> trace { return trace(); }
    [[nodiscard]] auto load_here([[maybe_unused]] size_t _ = 0) -> size_t { return 0; }
    [[nodiscard]] auto load_from(
      [[maybe_unused]] void* unused1,
      [[maybe_unused]] size_t unused2 = 0,
      [[maybe_unused]] void* unused3 = nullptr,
      [[maybe_unused]] void* unused4 = nullptr
    ) -> size_t {
      return 0;
    }

    [[nodiscard]] auto thread_id() const -> size_t { return 0; }
    [[nodiscard]] auto begin() const -> void* const* { return nullptr; }
    auto skip_n_firsts([[maybe_unused]] size_t _) -> void {}
  };

  class stack_trace_impl_base
  {
   public:
    stack_trace_impl_base()
      : _thread_id(0)
      , _skip(0)
      , _context(nullptr)
      , _error_addr(nullptr)
    {}

    [[nodiscard]] auto thread_id() const -> size_t { return this->_thread_id; }
    auto skip_n_firsts(size_t n) -> void { this->_skip = n; }

   protected:
    auto load_thread_info() -> void {
      #ifdef FLOPPY_OS_LINUX
      # ifndef __ANDROID__
      this->_thread_id = static_cast<size_t>(syscall(SYS_gettid));
      # else // __ANDROID__
      this->_thread_id = static_cast<size_t>(gettid());
      # endif // __ANDROID__
      if(this->_thread_id == static_cast<size_t>(getpid()))
        this->_thread_id = 0;
      #elif defined(FLOPPY_OS_DARWIN)
      this->_thread_id = reinterpret_cast<size_t>(pthread_self());
      if(pthread_main_np() == 1)
        this->_thread_id = 0;
      #endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
    }

    [[nodiscard]] auto context() const -> void* { return this->_context; }
    auto set_context(void* context) -> void { this->_context = context; }

    [[nodiscard]] auto error_addr() const -> void* { return this->_error_addr; }
    auto set_error_addr(void* error_addr) -> void { this->_error_addr = error_addr; }

    [[nodiscard]] auto skip_n_firsts() const -> size_t { return this->_skip; }

   private:
    size_t _thread_id{};
    size_t _skip{};
    void* _context{};
    void* _error_addr{};
  };

  class stack_trace_impl_holder : public stack_trace_impl_base
  {
   public:
    [[nodiscard]] auto size() const -> size_t {
      return (this->_stacktrace.size() >= this->skip_n_firsts())
        ? this->_stacktrace.size() - this->skip_n_firsts()
        : 0;
    }

    [[nodiscard]] auto operator[](size_t idx) const -> trace {
      if(idx >= this->size())
        return {};
      return trace(this->_stacktrace[idx + this->skip_n_firsts()], idx);
    }

    [[nodiscard]] auto begin() const -> void* const* {
      if(this->size())
        return &this->_stacktrace[this->skip_n_firsts()];
      return nullptr;
    }

   protected:
    std::vector<void*> _stacktrace; // NOLINT(*-non-private-member-variables-in-classes, *-identifier-naming)
  };
} // namespace floppy::stacktrace::impl