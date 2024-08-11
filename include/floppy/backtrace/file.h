#pragma once

#include <chrono>
#include <filesystem>
#include <sstream>

namespace floppy::stacktrace::impl
{
  [[maybe_unused]] [[nodiscard]] auto make_crash_report_path(std::filesystem::path const& folder = std::filesystem::current_path() / "crashdumps")
    -> std::filesystem::path {
    if(not std::filesystem::exists(folder))
      std::filesystem::create_directories(folder);
    auto current_time = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(current_time);
    auto ss = std::stringstream();
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d_%H-%M-%S");
    return folder / ("crash_reports_" + ss.str() + ".log");
  }
} // namespace floppy::stacktrace::impl