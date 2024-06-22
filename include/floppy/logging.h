#pragma once

#include <spdlog/spdlog.h>
#include <floppy/detail/types.h>
#include <floppy/detail/export.h>

namespace spdlog {} // namespace spdlog

/// \brief Namespace providing access to logging facilities.
namespace floppy::log {
  /// \brief Logging levels.
  namespace level {
    using namespace spdlog::level;
  } // namespace level

  /// \brief Logs message to given loger with level <b>trace</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param logger Logger to log to.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see trace
  template <typename... Args>
  auto trace_to(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    logger.trace(fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to default logger with level <b>trace</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see trace_to
  template <typename... Args>
  auto trace(fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    trace_to(*spdlog::default_logger(), fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to given loger with level <b>debug</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param logger Logger to log to.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see debug
  template <typename... Args>
  auto debug_to(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    logger.debug(fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to default logger with level <b>debug</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see debug_to
  template <typename... Args>
  auto debug(fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    debug_to(*spdlog::default_logger(), fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to given loger with level <b>info</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param logger Logger to log to.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see info
  template <typename... Args>
  auto info_to(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    logger.info(fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to default logger with level <b>info</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see info_to
  template <typename... Args>
  auto info(fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    info_to(*spdlog::default_logger(), fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to given loger with level <b>warn</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param logger Logger to log to.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see warn
  template <typename... Args>
  auto warn_to(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    logger.warn(fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to default logger with level <b>warn</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see warn_to
  template <typename... Args>
  auto warn(fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    warn_to(*spdlog::default_logger(), fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to given loger with level <b>err</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param logger Logger to log to.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see error
  template <typename... Args>
  auto error_to(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    logger.error(fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to default logger with level <b>err</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see error_to
  template <typename... Args>
  auto error(fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    error_to(*spdlog::default_logger(), fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to given loger with level <b>critical</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param logger Logger to log to.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see critical
  template <typename... Args>
  auto critical_to(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    logger.critical(fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to default logger with level <b>critical</b>.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see critical_to
  template <typename... Args>
  auto critical(fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    critical_to(*spdlog::default_logger(), fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to given loger with given level.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param level Log level.
  /// \param logger Logger to log to.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see log
  template <typename... Args>
  auto log_to(level::level_enum level, spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    logger.log(level, fmt, std::forward<Args>(args)...);
  }

  /// \brief Logs message to default logger with given level.
  /// \headerfile floppy/logging.h 
  /// \ingroup logging
  /// \tparam Args Format arguments.
  /// \param level Log level.
  /// \param fmt Format string.
  /// \param args Format arguments.
  /// \see log_to
  template <typename... Args>
  auto log(level::level_enum level, fmt::format_string<Args...> const& fmt, Args&&... args) -> void {
    log_to(level, *spdlog::default_logger(), fmt, std::forward<Args>(args)...);
  }
} // namespace floppy::log

/// \defgroup logging Logging
/// \ingroup foundation
/// \brief Logging-related functions.
/// \details Provides convenient wrapper over **spdlog**.