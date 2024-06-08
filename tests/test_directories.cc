#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/directories.h>

TEST(Directories, ApplicationDirs)
{
  using enum fl::filesystem::application_dirs::dir;
  auto dirs = fl::filesystem::application_dirs("com", "Foo Corp", "Bar App");
  #if defined(FLOPPY_OS_WINDOWS)
  auto username = std::string(std::getenv("USERNAME"));
    EXPECT_EQ(dirs[cache].string(), R"(C:\Users\)" + username + R"(\AppData\Local\Foo Corp\Bar App\cache)");
    EXPECT_EQ(dirs[config].string(), R"(C:\Users\)" + username + R"(\AppData\Roaming\Foo Corp\Bar App\config)");
    EXPECT_EQ(dirs[data].string(), R"(C:\Users\)" + username + R"(\AppData\Roaming\Foo Corp\Bar App\data)");
    EXPECT_EQ(dirs[data_local].string(), R"(C:\Users\)" + username + R"(\AppData\Local\Foo Corp\Bar App\data)");
    EXPECT_EQ(dirs[config_local].string(), R"(C:\Users\)" + username + R"(\AppData\Local\Foo Corp\Bar App\config)");
    EXPECT_EQ(dirs[preferences].string(), R"(C:\Users\)" + username + R"(\AppData\Roaming\Foo Corp\Bar App\config)");
    EXPECT_FALSE(dirs.runtime_dir().has_value());
    EXPECT_FALSE(dirs.state_dir().has_value());
  #else
    GTEST_SUCCEED();
  #endif
}