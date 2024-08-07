#include <floppy/floppy.h>
#include <floppy/logging.h>
#include <floppy/backtrace.h>
#include "obj.hh"

[[maybe_unused]] fl::stacktrace::signal_handler sh;

auto bad_boy() -> void {
  char* x = nullptr;
  *x = 42;
}

auto main(int argc, char** argv) -> int {
  bad_boy();
  return 0;
}