#include <floppy/floppy.h>
#include <floppy/logging.h>
#include <floppy/stacktrace.h>
#include "obj.hh"

[[maybe_unused]] fl::stacktrace::signal_watcher sw;

auto bad_boy() -> void {
  char* x = nullptr;
  *x = 42;
}

auto main(int argc, char** argv) -> int {
  bad_boy();
  throw_it_like_its_hot();
  return 0;
}