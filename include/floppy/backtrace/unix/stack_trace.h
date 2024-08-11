#pragma once

#include <floppy/detail/predefs.h>
#if defined(FLOPPY_OS_LINUX)
# include <fcntl.h>
# ifdef __ANDROID__
#   define _Unwind_Ptr _Unwind_Ptr_Custom
#   include <link.h>
#   undef _Unwind_Ptr
# else // __ANDROID__
#   include <link.h>
# endif // __ANDROID__
# if defined(__ppc__) || defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__)
#   include <asm/ptrace.h>
# endif // defined(__ppc__) || defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__)
# include <sys/stat.h>
# include <syscall.h>
# include <unistd.h>
# ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#   include <dlfcn.h>
#   undef _GNU_SOURCE
# else // _GNU_SOURCE
#   include <dlfcn.h>
# endif // _GNU_SOURCE
# include <dwarf.h>
# include <elfutils/libdw.h>
# include <elfutils/libdwfl.h>
# include <execinfo.h>
#endif // defined(FLOPPY_OS_LINUX)
#if defined(FLOPPY_OS_DARWIN)
# include <fcntl.h>
# include <pthread.h>
# include <sys/stat.h>
# include <unistd.h>
# include <execinfo.h>
#endif // defined(FLOPPY_OS_DARWIN)
#include <floppy/detail/export.h>
#include <floppy/backtrace/stack_trace.h>
#include <floppy/backtrace/trace_resolver_tag.h>

namespace floppy::stacktrace::impl
{
  template <>
  class stack_trace_impl<system_tag::current> : public stack_trace_impl_holder {
   public:
    __noinline__
      size_t load_here(size_t depth = 32, void *context = nullptr,
                void *error_addr = nullptr) {
      set_context(context);
      set_error_addr(error_addr);
      load_thread_info();
      if(depth == 0)
        return 0;
      _stacktrace.resize(depth + 1);
      size_t trace_cnt = backtrace(&_stacktrace[0], _stacktrace.size());
      _stacktrace.resize(trace_cnt);
      skip_n_firsts(1);
      return size();
    }

    size_t load_from(void *addr, size_t depth = 32, void *context = nullptr,
                     void *error_addr = nullptr) {
      load_here(depth + 8, context, error_addr);

      for (size_t i = 0; i < _stacktrace.size(); ++i) {
        if (_stacktrace[i] == addr) {
          skip_n_firsts(i);
          _stacktrace[i] = (void *)((uintptr_t)_stacktrace[i] + 1);
          break;
        }
      }

      _stacktrace.resize(std::min(_stacktrace.size(), skip_n_firsts() + depth));
      return size();
    }
  };
} // namespace floppy::stacktrace::impl