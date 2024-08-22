#pragma once

#include <stdexcept>
#include <source_location>
#include <floppy/detail/formatters.h>
#include <floppy/detail/export.h>

#ifdef FL_DEBUG
# define __floppy_internal_remove_in_release__(...) __VA_ARGS__ // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __constant_evaluated__  // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __release_only_maybe_unused__
#else // FL_NO_DEBUG
# define __floppy_internal_remove_in_release__(...) // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __constant_evaluated__ consteval
# define __release_only_maybe_unused__ [[maybe_unused]]
#endif // FL_DEBUG/FL_NO_DEBUG

namespace floppy::contracts
{
  class contract_violation : public std::invalid_argument
  {
   public:
    using std::invalid_argument::what;

    explicit contract_violation(std::string_view message, std::source_location location = std::source_location::current())
      : std::invalid_argument(fmt::format("contract violation at file {} line {}:{} in function {}: {}",
          location.file_name(),
          location.line(),
          location.column(),
          location.function_name(),
          message
        ))
    {}
  };

  [[noreturn]] inline void raise_contract_violation(std::string_view message, std::source_location location = std::source_location::current()) {
    throw contract_violation(message, location);
  }

  __forceinline__
  __constant_evaluated__
  void expects(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::string_view message = "failed precondition contract check",
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(
      if(not condition)
        raise_contract_violation(message, location);
    )
  }

  __forceinline__
  __constant_evaluated__
  void ensures(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::string_view message = "failed postcondition contract check",
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(
      if(not condition)
        raise_contract_violation(message, location);
    )
  }

  __forceinline__
  __constant_evaluated__
  void invariant(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::string_view message = "failed invariant contract check",
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(
      if(not condition)
        raise_contract_violation(message, location);
    )
  }

  __forceinline__
  __constant_evaluated__
  void asserts(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::string_view message = "failed assertion",
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(
      if(not condition)
        raise_contract_violation(message, location);
    )
  }
} // namespace floppy::contracts

#undef __constant_evaluated__
#undef __floppy_internal_remove_in_release__