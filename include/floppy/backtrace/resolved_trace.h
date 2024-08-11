#pragma once

#include <vector>
#include <floppy/detail/backports.h>
#include <floppy/backtrace/trace.h>

namespace floppy::stacktrace::impl
{
  struct resolved_trace : public trace
  {
    using source_loc_t = source_location;
    using source_locs_t = std::vector<source_loc_t>;

    resolved_trace() = default;

    resolved_trace(trace const& mini_trace)
      : trace(mini_trace)
    {}

    /// \brief In which binary object this trace is located.
    std::string object_filename;

    /// \brief The function in the object that contain the trace.
    /// \note This is not the same as source.function which can be an function inlined in object_function.
    std::string object_function;

    /// \brief The source location of this trace.
    /// \details It is possible for filename to be empty and for line/col to be invalid (value 0)
    /// if this information couldn't be deduced, for example if there is no debug information in the
    /// binary object.
    source_location source;

    /// \brief An optionals list of "inliners".
    /// \details All the successive sources location from where the source location of the trace
    /// (the attribute right above) is inlined. It is especially useful when you compiled with optimization.
    source_locs_t inliners;
  };
} // namespace floppy::stacktrace::impl
