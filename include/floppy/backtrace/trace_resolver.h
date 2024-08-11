#pragma once

#include <floppy/detail/predefs.h>
#include <floppy/backtrace/resolved_trace.h>
#include <floppy/backtrace/demangle.h>

namespace floppy::stacktrace::impl
{
  class trace_resolver_impl_base
  {
   public:
    virtual ~trace_resolver_impl_base() = default;

    virtual void load_addresses(void* const* addresses, int address_count) {
      (void)addresses;
      (void)address_count;
    }

    template <class ST>
    void load_stacktrace(ST& st) {
      this->load_addresses(st.begin(), static_cast<int>(st.size()));
    }

    [[nodiscard]] virtual auto resolve(resolved_trace t) -> resolved_trace { return t; }

   protected:
    [[nodiscard]] auto demangle(char const* func_name) -> std::string {
      return floppy::stacktrace::details::demangler::demangle(func_name);
    }

   private:
    details::demangler _demangler;
  };

  template <typename Tag>
  class trace_resolver_impl;

  #ifdef FLOPPY_OS_UNKNOWN
  template <> class trace_resolver_impl<system_tag::unknown> : public trace_resolver_impl_base {};
  #endif // FLOPPY_OS_UNKNOWN
} // namespace floppy::stacktrace::impl
