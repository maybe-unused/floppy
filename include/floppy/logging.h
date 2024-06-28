#pragma once

#include <spdlog/spdlog.h>
#include <floppy/detail/types.h>
#include <floppy/detail/export.h>
#include <floppy/detail/backports.h>

namespace spdlog {} // namespace spdlog

/// \brief Namespace providing access to logging facilities.
namespace floppy::log {
  /// \brief Logs message with given level.
  /// \headerfile floppy/logging.h
  /// \ingroup logging
  /// \details
  /// This class is a wrapper around spdlog::logger.
  /// Provides source location information in log messages.
  /// Example usage syntax:
  /// \code {.cpp}
  /// floppy::log::log()(floppy::log::level::info, "Message"); // logs message at info level to default logger
  /// floppy::log::log()(floppy::log::level::info, "Message {}", 1); // logs message at info level to default logger with 1 as argument
  /// floppy::log::log()(floppy::log::level::info, *spdlog::default_logger(), "Message {} and {}", 1, 2); // logs message at info level to selected logger with 1 and 2 as arguments
  /// \endcode
  /// If you want to log message without source location, pass <code>none</code> to first pair of brackets:
  /// \code {.cpp}
  /// floppy::log::log(none)(floppy::log::level::info, "Message {} and {}", 1, 2);
  /// \endcode
  /// \see source_location
  /// \see trace, debug, info, warn, error, critical
  class log final
  {
   public:
    /// \brief Constructs log object with source location information.
    /// \param loc Source location to use. Defaults to current source location. Pass <code>none</code> to disable source location info in logs.
    /// \see source_location
    constexpr explicit log(option<source_location> const& loc = source_location::current())
      : loc_(loc)
    {}

    /// \brief Logs message to given loger with given level.
    /// \details This is alias for call operator.
    /// \tparam Args Format arguments.
    /// \param level Log level.
    /// \param logger Logger to log to.
    /// \param fmt Format string.
    /// \param args Format arguments.
    /// \see operator()
    /// \see write
    template <typename... Args>
    auto write_to(
      spdlog::level::level_enum lvl,
      spdlog::logger& logger,
      fmt::format_string<Args...> const& fmt,
      Args&&... args
    ) const -> void {
      switch(lvl) {
        case spdlog::level::off: break;
        case spdlog::level::trace:
          if(not this->loc_)
            logger.trace(fmt, std::forward<Args>(args)...);
          else
            logger.trace("{}: {}", this->loc_->function_name(), fmt::format(fmt, std::forward<Args>(args)...));
          break;
        case spdlog::level::debug:
          if(not this->loc_)
            logger.debug(fmt, std::forward<Args>(args)...);
          else
            logger.debug("{}: {}", this->loc_->function_name(), fmt::format(fmt, std::forward<Args>(args)...));
          break;
        case spdlog::level::info:
          if(not this->loc_)
            logger.info(fmt, std::forward<Args>(args)...);
          else
            logger.info("{}: {}", this->loc_->function_name(), fmt::format(fmt, std::forward<Args>(args)...));
          break;
        case spdlog::level::warn:
          if(not this->loc_)
            logger.warn(fmt, std::forward<Args>(args)...);
          else
            logger.warn("{}: {}", this->loc_->function_name(), fmt::format(fmt, std::forward<Args>(args)...));
          break;
        case spdlog::level::err:
          if(not this->loc_)
            logger.error(fmt, std::forward<Args>(args)...);
          else
            logger.error("{}: {}", this->loc_->function_name(), fmt::format(fmt, std::forward<Args>(args)...));
          break;
        case spdlog::level::critical:
          if(not this->loc_)
            logger.critical(fmt, std::forward<Args>(args)...);
          else
            logger.critical("{}: {}", this->loc_->function_name(), fmt::format(fmt, std::forward<Args>(args)...));
          break;
        case spdlog::level::n_levels: break;
      }
    }

    /// \brief Logs message to default logger with given level.
    /// \details This is alias for call operator.
    /// \tparam Args Format arguments.
    /// \param level Log level.
    /// \param fmt Format string.
    /// \param args Format arguments.
    /// \sa operator()
    /// \sa write_to
    template <typename... Args>
    auto write(
      spdlog::level::level_enum lvl,
      fmt::format_string<Args...> const& fmt,
      Args&&... args
    ) const -> void {
      this->write_to(lvl, *spdlog::default_logger(), fmt, std::forward<Args>(args)...);
    }

    /// \brief Logs message to given loger with given level.
    /// \details This is alias for function \ref write_to().
    /// \tparam Args Format arguments.
    /// \param level Log level.
    /// \param logger Logger to log to.
    /// \param fmt Format string.
    /// \param args Format arguments.
    /// \see write, write_to
    template <typename... Args>
    auto operator()(
      spdlog::level::level_enum lvl,
      spdlog::logger& logger,
      fmt::format_string<Args...> const& fmt,
      Args&&... args
    ) const -> void {
      this->write_to(lvl, logger, fmt, std::forward<Args>(args)...);
    }

    /// \brief Logs message to default logger with given level.
    /// \headerfile floppy/logging.h
    /// \ingroup logging
    /// \tparam Args Format arguments.
    /// \param level Log level.
    /// \param fmt Format string.
    /// \param args Format arguments.
    /// \see write, write_to
    template <typename... Args>
    void operator()(spdlog::level::level_enum lvl, fmt::format_string<Args...> const& fmt, Args&&... args) const {
      this->write(lvl, fmt, std::forward<Args>(args)...);
    }

   private:
    option<source_location> loc_;
  };

  /// \brief Implementation details of \ref log.
  namespace detail
  {
    template <auto L>
    requires std::is_same_v<decltype(L), spdlog::level::level_enum>
    class basic_log_helper
    {
     public:
       constexpr explicit basic_log_helper(option<source_location> const& loc = source_location::current())
         : loc_(loc)
       {}

       template <typename... Args>
       auto write_to(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) const -> void {
         log(this->loc_)(L, logger, fmt, std::forward<Args>(args)...);
       }

       template <typename... Args>
       auto write(fmt::format_string<Args...> const& fmt, Args&&... args) const -> void {
         log(this->loc_)(L, fmt, std::forward<Args>(args)...);
       }

       template <typename... Args>
       auto operator()(spdlog::logger& logger, fmt::format_string<Args...> const& fmt, Args&&... args) const -> void {
         this->write_to(logger, fmt, std::forward<Args>(args)...);
       }

       template <typename... Args>
       auto operator()(fmt::format_string<Args...> const& fmt, Args&&... args) const -> void {
         this->write(fmt, std::forward<Args>(args)...);
       }

     protected:
      option<source_location> loc_;
    };
  } // namespace detail

  // \brief Logs message with <b>trace</b> level.
  // \headerfile floppy/logging.h
  // \ingroup logging
  // \details
  // This class is a wrapper around spdlog::logger.
  // Provides source location information in log messages.
  // Example usage syntax:
  // \code {.cpp}
  // floppy::log::trace()("Message"); // logs message at info level to default logger
  // floppy::log::trace()("Message {}", 1); // logs message at info level to default logger with 1 as argument
  // floppy::log::trace()(*spdlog::default_logger(), "Message {} and {}", 1, 2); // logs message at info level to selected logger with 1 and 2 as arguments
  // \endcode
  // If you want to log message without source location, pass <code>none</code> to first pair of brackets:
  // \code {.cpp}
  // floppy::log::trace(none)("Message {} and {}", 1, 2);
  // \endcode
  // \see source_location
  // \see log
  // \see debug, info, warn, error, critical

  /// \brief Logging levels.
  namespace level {
    using namespace spdlog::level;
  } // namespace level

  using trace = detail::basic_log_helper<level::trace>;
  using debug = detail::basic_log_helper<level::debug>;
  using info = detail::basic_log_helper<level::info>;
  using warn = detail::basic_log_helper<level::warn>;
  using error = detail::basic_log_helper<level::err>;
  using critical = detail::basic_log_helper<level::critical>;
} // namespace floppy::log

/// \defgroup logging Logging
/// \ingroup foundation
/// \brief Logging-related functions.
/// \details Provides convenient wrapper over **spdlog**.