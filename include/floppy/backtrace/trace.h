#pragma once

#include <floppy/detail/stdint.h>

namespace floppy::stacktrace::impl
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
} // namespace floppy::stacktrace::impl