#pragma once

#include <floppy/detail/types.h>

namespace floppy::stacktrace
{
  struct trace
  {
    void* addr;
    usize idx;

    trace()
      : addr(nullptr)
      , idx(0)
    {}

    explicit trace(void* address, usize index)
      : addr(address)
      , idx(index)
    {}
  };
} // namespace floppy::stacktrace