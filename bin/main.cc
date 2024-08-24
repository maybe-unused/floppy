#include <memory>

#include <fl/global.h>
#include <fl/contracts.h>

auto main(int argc, char** argv) -> int {
  fl::contracts::broken_postcondition();
  fl::contracts::invariant(false, "Hello, world!");
  return 0;
}