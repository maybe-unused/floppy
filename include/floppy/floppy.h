#pragma once

#include <floppy/detail/backports.h>
#include <floppy/detail/concepts.h>
#include <floppy/detail/corefn.h>
#include <floppy/detail/export.h>
#include <floppy/detail/formatters.h>
#include <floppy/detail/math.h>
#include <floppy/detail/platform.h>
#include <floppy/detail/predefs.h>
#include <floppy/detail/print_helpers.h>
#include <floppy/detail/type_traits.h>
#include <floppy/detail/types.h>

/// \brief Formatter for `floppy::platform::vendor`.
template <> struct [[maybe_unused]] fmt::formatter<floppy::platform::vendor> : floppy::ostream_formatter<char> {};

/// \brief Formatter for `floppy::platform::operating_system`.
template <> struct [[maybe_unused]] fmt::formatter<enum floppy::platform::operating_system> : floppy::ostream_formatter<char> {};

/// \brief Formatter for `floppy::platform::arch`.
template <> struct [[maybe_unused]] fmt::formatter<floppy::platform::arch> : floppy::ostream_formatter<char> {};

/// \brief Formatter for `floppy::platform::compiler`.
template <> struct [[maybe_unused]] fmt::formatter<enum floppy::platform::compiler> : floppy::ostream_formatter<char> {};

/// \brief Formatter for `floppy::platform::endianness`.
template <> struct [[maybe_unused]] fmt::formatter<enum floppy::platform::endianness> : floppy::ostream_formatter<char> {};

/// \brief Formatter for `floppy::platform`.
template <> struct [[maybe_unused]] fmt::formatter<floppy::platform> : floppy::ostream_formatter<char> {};