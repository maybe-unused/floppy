#pragma once

#include <stdexcept>
#include <fmt/format.h>

namespace floppy // NOLINT(*-concat-nested-namespaces)
{
  /// \brief Exceptions for the library.
  inline namespace exceptions
  {
    /// \brief Indicates a compile time error.
    /// \ingroup exceptions
    /// \headerfile floppy/floppy.h
    class [[maybe_unused]] compile_time_error : public std::exception
    {
     public:
      /// \brief Creates compile time error.
      explicit compile_time_error() = default;

      /// \brief Returns the message of the exception.
      /// \returns The message of the exception.
      [[nodiscard]] auto what() const noexcept -> char const* override {
        return "compile time error occurred";
      }
    };
  } // namespace exceptions
} // namespace floppy

/// \defgroup exceptions Exceptions
/// \ingroup foundation
/// \brief Library exception classes.