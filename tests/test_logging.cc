#include <gtest/gtest.h>
#include <floppy/logging.h>

TEST(Logging, Simple)
{
  namespace log = floppy::log;
  spdlog::set_level(spdlog::level::trace);
  log::log(log::level::trace, "Hello, {}!", "Floppy");
  log::log(log::level::debug, "Hello, {}!", "Floppy");
  log::log(log::level::info, "Hello, {}!", "Floppy");
  log::log(log::level::warn, "Hello, {}!", "Floppy");
  log::log(log::level::err, "Hello, {}!", "Floppy");
  log::error("Hello, {}!", "Floppy");
  log::warn("Hello, {}!", "Floppy");
  log::info("Hello, {}!", "Floppy");
  log::debug("Hello, {}!", "Floppy");
  log::trace("Hello, {}!", "Floppy");
}