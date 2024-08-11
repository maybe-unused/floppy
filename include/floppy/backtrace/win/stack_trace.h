#pragma once

#ifndef NOMINMAX
# define NOMINMAX
#endif // NOMINMAX

#include <windows.h>
#include <winnt.h>
#include <basetsd.h>
#include <psapi.h>
#ifdef _MSC_VER
# pragma comment(lib, "psapi.lib")
# pragma comment(lib, "dbghelp.lib")
#endif // _MSC_VER
#pragma pack(push, before_imagehlp, 8)
# include <imagehlp.h>
#pragma pack(pop, before_imagehlp)
#include <floppy/detail/export.h>
#include <floppy/backtrace/stack_trace.h>
#include <floppy/backtrace/trace_resolver_tag.h>

namespace floppy::stacktrace::impl
{
  template <>
  class stack_trace_impl<system_tag::current> : public stack_trace_impl_holder {
   public:
    void set_machine_type(DWORD machine_type) { this->machine_type_ = machine_type; }
    void set_context(CONTEXT* ctx) { this->ctx_ = ctx; }
    void set_thread_handle(HANDLE handle) { this->thd_ = handle; }

    __noinline__ auto load_here(
      size_t depth = 32,
      void* context = nullptr,
      void* error_addr = nullptr
    ) -> size_t {
      this->set_context(static_cast<CONTEXT*>(context));
      this->set_error_addr(error_addr);
      CONTEXT localCtx; // used when no context is provided

      if(depth == 0)
        return 0;
      if(not this->ctx_) {
        this->ctx_ = &localCtx;
        ::RtlCaptureContext(this->ctx_);
      }
      if(not this->thd_)
        this->thd_ = ::GetCurrentThread();
      HANDLE process = ::GetCurrentProcess();
      STACKFRAME64 s;
      std::memset(&s, 0, sizeof(STACKFRAME64));

      s.AddrStack.Mode = AddrModeFlat;
      s.AddrFrame.Mode = AddrModeFlat;
      s.AddrPC.Mode = AddrModeFlat;

      #ifdef _M_X64
      s.AddrPC.Offset = ctx_->Rip;
      s.AddrStack.Offset = ctx_->Rsp;
      s.AddrFrame.Offset = ctx_->Rbp;
      #else // _M_X64
      s.AddrPC.Offset = ctx_->Eip;
      s.AddrStack.Offset = ctx_->Esp;
      s.AddrFrame.Offset = ctx_->Ebp;
      #endif // _M_X64

      if(not this->machine_type_)
        #ifdef _M_X64
          this->machine_type_ = IMAGE_FILE_MACHINE_AMD64;
        #else // _M_X64
          this->machine_type_ = IMAGE_FILE_MACHINE_I386;
        #endif // _M_X64

      for(;;) {
        ::SetLastError(0);
        if(not ::StackWalk64(
          machine_type_,
          process,
          thd_,
          &s,
          ctx_,
          nullptr,
          ::SymFunctionTableAccess64,
          ::SymGetModuleBase64,
          nullptr
        ))
          break;
        if(s.AddrReturn.Offset == 0)
          break;
        this->_stacktrace.push_back(reinterpret_cast<void*>(s.AddrPC.Offset));
        if(this->size() >= depth)
          break;
      }
      return this->size();
    }

    auto load_from(
      void* addr,
      size_t depth = 32,
      void* context = nullptr,
      void* error_addr = nullptr
    ) -> size_t {
      this->load_here(depth + 8, context, error_addr);
      for(size_t i = 0; i < this->_stacktrace.size(); ++i) {
        if(this->_stacktrace[i] == addr) {
          this->skip_n_firsts(i);
          break;
        }
      }
      this->_stacktrace.resize(std::min(this->_stacktrace.size(), this->skip_n_firsts() + depth));
      return size();
    }

   private:
    DWORD machine_type_ = 0;
    HANDLE thd_ = 0;
    CONTEXT* ctx_ = nullptr;
  };
} // namespace floppy::stacktrace::impl