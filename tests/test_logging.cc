#include <gtest/gtest.h>
#include <floppy/logging.h>

TEST(Logging, Simple)
{
  namespace llog = floppy::log;
  spdlog::set_level(spdlog::level::trace);
  auto str = std::string_view("Floppy");
  llog::log()(llog::level::trace, "{}", 1);
  llog::log()(llog::level::debug, "{}", 2);
  llog::log()(llog::level::info, "{}", 3);
  llog::log()(llog::level::warn, "{}", 4);
  llog::log()(llog::level::err, "{}", 5);
  llog::log()(llog::level::critical, "{}", 6);
  llog::log()(llog::level::off, "{}", 7);
  llog::log()(llog::level::trace, "{}", str);
  llog::log()(llog::level::debug, "{}", str);
  llog::log()(llog::level::info, "{}", str);
  llog::log()(llog::level::warn, "{}", str);
  llog::log()(llog::level::err, "{}", str);
  llog::log()(llog::level::critical, "{}", str);
  llog::log()(llog::level::off, "{}", str);
}