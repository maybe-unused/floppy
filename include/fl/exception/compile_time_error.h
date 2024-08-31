#pragma once

#include <stdexcept>
#include <string_view>
#include <format>

namespace fl
{
  /**
   * @brief Inline namespace for library exception classes.
   */
  inline namespace exceptions {} // namespace concepts
} // namespace fl

namespace fl::inline exceptions
{
  class compile_time_error : public std::exception
  {
    public:
      compile_time_error()
        : std::exception("compile-time error occurred.\n")
      {}

      explicit compile_time_error(std::string_view message)
        : std::exception(std::format("compile-time error: {}\n", message).c_str())
      {}

      template <typename... Args>
      explicit compile_time_error(std::string_view message, Args&&... args)
        : std::exception(std::format("compile-time error: {}\n", std::format(message, std::forward<Args>(args)...)).c_str())
      {}

      virtual ~compile_time_error() noexcept = default;

      [[nodiscard]] char const* what() const noexcept override {
        return std::exception::what();
      }
  };
} // namespace fl::inline exceptions