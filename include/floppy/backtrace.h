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
  using colorize = Colorize;

  namespace color = Color; // NOLINT(*-unused-alias-decls)
  namespace color_mode = ColorMode; // NOLINT(*-unused-alias-decls)
} // namespace floppy::stacktrace