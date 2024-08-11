#pragma once

#include <floppy/detail/predefs.h>

namespace floppy::stacktrace::trace_resolver_tag
{
  #if defined(FLOPPY_OS_LINUX)
    struct libdw;
    struct libbfd;
    struct libdwarf;
    struct backtrace_symbol;
    using current = libdw;
  #elif defined(FLOPPY_OS_DARWIN)
    struct backtrace_symbol;
  #elif defined(FLOPPY_OS_WINDOWS)
    struct pdb_symbol;
    using current = pdb_symbol;
  #endif // defined(FLOPPY_OS_LINUX)
} // namespace floppy::stacktrace::trace_resolver_tag