#include <gtest/gtest.h>
#include <floppy/logging.h>

TEST(Logging, Simple)
{
  namespace l = floppy::logging;

  spdlog::set_level(spdlog::level::trace);
  l::log(l::lvl::trace, "Hello, {}!", "Floppy");
  l::log(l::lvl::debug, "Hello, {}!", "Floppy");
  l::log(l::lvl::info, "Hello, {}!", "Floppy");
  l::log(l::lvl::warn, "Hello, {}!", "Floppy");
  l::log(l::lvl::err, "Hello, {}!", "Floppy");
  l::error("Hello, {}!", "Floppy");
  l::warn("Hello, {}!", "Floppy");
  l::info("Hello, {}!", "Floppy");
  l::debug("Hello, {}!", "Floppy");
  l::trace("Hello, {}!", "Floppy");
}