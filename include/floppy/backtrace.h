#pragma once

#include <floppy/floppy.h>
#define BACKWARD_HAS_BACKTRACE 1
#if defined(FLOPPY_OS_WINDOWS)
# define BACKWARD_HAS_DW 1
#endif // FLOPPY_OS_WINDOWS
#include "backward.hpp"

/// \brief Stacktrace namespace.
/// \details Currently implemented using <a href="https://github.com/bombela/backward-cpp">backward-cpp</a> library.
/// \ingroup foundation
/// \sa https://github.com/bombela/backward-cpp
namespace floppy::stacktrace
{
  using signal_handler = backward::SignalHandling;

  using trace = backward::Trace;
  using resolved_trace = backward::ResolvedTrace;
  using stack_trace = backward::StackTrace;
  using trace_resolver = backward::TraceResolver;
  using source_file = backward::SourceFile;
  using snippet_factory = backward::SnippetFactory;
  using printer = backward::Printer;
  using colorize = backward::Colorize;

  namespace color = backward::Color; // NOLINT(*-unused-alias-decls)
  namespace color_mode = backward::ColorMode; // NOLINT(*-unused-alias-decls)
} // namespace floppy::stacktrace