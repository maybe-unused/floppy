#pragma once

#if defined(FLOPPY_OS_WINDOWS)
# include <floppy/backtrace/win/trace_resolver.h>
#else // defined(FLOPPY_OS_WINDOWS)
# include <floppy/backtrace/unix/trace_resolver.h>
#endif // defined(FLOPPY_OS_WINDOWS)

namespace floppy::stacktrace::impl
{
  class stack_trace : public stack_trace_impl<system_tag::current> {};
  class trace_resolver : public trace_resolver_impl<system_tag::current> {};
} // namespace floppy::stacktrace::impl
