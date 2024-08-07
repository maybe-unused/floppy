#include <floppy/floppy.h>
#include <floppy/logging.h>
#include <backward.hpp>
#include "obj.hh"

[[maybe_unused]] backward::SignalHandling sh;

auto bad_boy() -> void {
  char* x = nullptr;
  *x = 42;
}

auto main(int argc, char** argv) -> int {
  throw_it_like_its_hot();
  bad_boy();
  return 0;
}