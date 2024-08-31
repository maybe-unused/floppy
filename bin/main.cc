#include <memory>

#include <fl/global.h>
#include <fl/memory/box.h>
#include <fl/types/stdint.h>

auto main(int argc, char** argv) -> int {
  fl::contracts::broken_postcondition();
  fl::contracts::invariant(false, "Hello, world!");
  return 0;
}