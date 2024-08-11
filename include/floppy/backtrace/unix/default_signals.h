#pragma once

#include <vector>
#include <floppy/detail/predefs.h>

namespace floppy::stacktrace::impl
{
  inline auto make_default_signals() -> std::vector<int> {
    const int posix_signals[] = {
      // Signals for which the default action is "Core".
      SIGABRT, // Abort signal from abort(3)
      SIGBUS,  // Bus error (bad memory access)
      SIGFPE,  // Floating point exception
      SIGILL,  // Illegal Instruction
      SIGIOT,  // IOT trap. A synonym for SIGABRT
      SIGQUIT, // Quit from keyboard
      SIGSEGV, // Invalid memory reference
      SIGSYS,  // Bad argument to routine (SVr4)
      SIGTRAP, // Trace/breakpoint trap
      SIGXCPU, // CPU time limit exceeded (4.2BSD)
      SIGXFSZ, // File size limit exceeded (4.2BSD)
      #if defined(FLOPPY_OS_DARWIN)
      SIGEMT, // Emulation instruction executed
      #endif // defined(FLOPPY_OS_DARWIN)
    };
    
    return std::vector<int>(
      posix_signals,
      posix_signals + sizeof posix_signals / sizeof posix_signals[0]);
  }
} // namespace floppy::stacktrace::impl