#pragma once

#include <stdexcept>
#include <source_location>
#include <floppy/detail/formatters.h>
#include <floppy/detail/export.h>

#ifdef FL_DEBUG
# define __floppy_internal_remove_in_release__(...) __VA_ARGS__ // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __constant_evaluated__  // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __release_only_maybe_unused__ // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#else // FL_NO_DEBUG
# define __floppy_internal_remove_in_release__(...) // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __constant_evaluated__ consteval // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __release_only_maybe_unused__ [[maybe_unused]] // NOLINT(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#endif // FL_DEBUG/FL_NO_DEBUG

// \brief Namespace for contract-programming related functions and classes.
namespace floppy::contracts
{
  /// \brief Base class for all contract violations.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \details All contract violations inherit from this class.
  /// Example output:
  /// \code
  /// contract violation at file 'main.cpp' line 42 in function 'foo()': message
  /// \endcode
  class contract_violation : public std::invalid_argument
  {
   public:
    using std::invalid_argument::what;

    /// \brief Creates contract violation object.
    /// \param message Error message.
    /// \param prefix_text Prefix text. Can be empty (default).
    /// \param location Source location of the violation (default is current).
    explicit contract_violation(std::string_view message, std::string_view prefix_text = "", std::source_location location = std::source_location::current())
      : std::invalid_argument(fmt::format(R"({}contract violation at file '{}' line {}:{} in function '{}': {})",
          prefix_text.empty() ? "" : std::string(prefix_text) + " ",
          location.file_name(),
          location.line(),
          location.column(),
          location.function_name(),
          message
        ))
    {}
  };

  /// \brief Contract violation for preconditions.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \details Example output:
  /// \code
  /// precondition contract violation at file 'main.cpp' line 42 in function 'foo()': message
  /// \endcode
  /// \see contract_violation
  /// \see expects
  class precondition_contract_violation final : public contract_violation
  {
   public:
    using contract_violation::what;

    /// \brief Creates precondition contract violation object.
    /// \param message Error message.
    /// \param location Source location of the violation (default is current).
    explicit precondition_contract_violation(
      std::string_view message,
      std::source_location location = std::source_location::current()
    ) : contract_violation(message, "precondition", location)
    {}
  };

  /// \brief Contract violation for postconditions.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \details Example output:
  /// \code
  /// postcondition contract violation at file 'main.cpp' line 42 in function 'foo()': message
  /// \endcode
  /// \see contract_violation
  /// \see ensures
  class postcondition_contract_violation final : public contract_violation
  {
   public:
    using contract_violation::what;

    /// \brief Creates postcondition contract violation object.
    /// \param message Error message.
    /// \param location Source location of the violation (default is current).
    explicit postcondition_contract_violation(
      std::string_view message,
      std::source_location location = std::source_location::current()
    ) : contract_violation(message, "postcondition", location)
    {}
  };

  /// \brief Contract violation for invariants.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \details Example output:
  /// \code
  /// invariant contract violation at file 'main.cpp' line 42 in function 'foo()': message
  /// \endcode
  /// \see contract_violation
  /// \see invariant
  class invariant_contract_violation final : public contract_violation
  {
   public:
    using contract_violation::what;

    /// \brief Creates invariant contract violation object.
    /// \param message Error message.
    /// \param location Source location of the violation (default is current).
    explicit invariant_contract_violation(
      std::string_view message,
      std::source_location location = std::source_location::current()
    ) : contract_violation(message, "invariant", location)
    {}
  };

  /// \brief Contract violation for assertions.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \details Example output:
  /// \code
  /// assertion contract violation at file 'main.cpp' line 42 in function 'foo()': message
  /// \endcode
  /// \see contract_violation
  /// \see asserts
  class assertion_contract_violation final : public contract_violation
  {
   public:
    using contract_violation::what;

    /// \brief Creates assertion contract violation object.
    /// \param message Error message.
    /// \param location Source location of the violation (default is current).
    explicit assertion_contract_violation(
      std::string_view message,
      std::source_location location = std::source_location::current()
    ) : contract_violation(message, "assertion", location)
    {}
  };

  /// \brief Asserts that the given condition is true.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \tparam T Contract violation type. Default is assertion_contract_violation.
  /// \param condition Condition to be checked.
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>if(not condition) throw T(message, location);</code> in the debug mode and to empty
  /// consteval function in the release mode.
  /// \throws T if the condition is false in the debug mode.
  /// \see broken_assert
  /// \see expects
  /// \see ensures
  /// \see invariant
  template <std::derived_from<contract_violation> T = assertion_contract_violation>
  __forceinline__
  __constant_evaluated__
  void asserts(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::string_view message = "failed assertion",
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(
      if(not condition)
        throw T(message, location);
    )
  }

  /// \brief Raises an assertion violation.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>asserts(false, message, location);</code> in the debug mode and to empty
  /// consteval function in the release mode.
  /// \throws assertion_contract_violation in the debug mode.
  /// \see asserts
  /// \see expects
  /// \see ensures
  /// \see invariant
  __forceinline__
  __constant_evaluated__
  void broken_assert(
    __release_only_maybe_unused__ std::string_view message = "failed assertion",
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(asserts(false, message, location);)
  }

  /// \brief Asserts that the given precondition is true.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param condition Precondition to be checked.
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>if(not condition) throw precondition_contract_violation(message, location);</code>
  /// in the debug mode and to empty consteval function in the release mode.
  /// \throws precondition_contract_violation if the condition is false in the debug mode.
  /// \see broken_precondition
  /// \see asserts
  /// \see ensures
  /// \see invariant
  __forceinline__
  __constant_evaluated__
  void expects(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(asserts<precondition_contract_violation>(condition, "failed precondition contract check", location);)
  }

  /// \brief Raises a precondition violation.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>expects(false, message, location);</code> in the debug mode and to empty
  /// consteval function in the release mode.
  /// \throws precondition_contract_violation in the debug mode.
  /// \see expects
  /// \see asserts
  /// \see ensures
  /// \see invariant
  __forceinline__
  __constant_evaluated__
  void broken_precondition(
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(expects(false, location);)
  }

  /// \brief Asserts that the given postcondition is true.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param condition Postcondition to be checked.
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>if(not condition) throw postcondition_contract_violation(message, location);</code>
  /// in the debug mode and to empty consteval function in the release mode.
  /// \throws postcondition_contract_violation if the condition is false in the debug mode.
  /// \see broken_postcondition
  /// \see asserts
  /// \see expects
  /// \see invariant
  __forceinline__
  __constant_evaluated__
  void ensures(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(asserts<postcondition_contract_violation>(condition, "failed postcondition contract check", location);)
  }

  /// \brief Raises a postcondition violation.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>ensures(false, message, location);</code> in the debug mode and to empty
  /// consteval function in the release mode.
  /// \throws postcondition_contract_violation in the debug mode.
  /// \see ensures
  /// \see asserts
  /// \see expects
  /// \see invariant
  __forceinline__
  __constant_evaluated__
  void broken_postcondition(
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(ensures(false, location);)
  }

  /// \brief Asserts that the given invariant is true.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param condition Invariant to be checked.
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>if(not condition) throw invariant_contract_violation(message, location);</code>
  /// in the debug mode and to empty consteval function in the release mode.
  /// \throws invariant_contract_violation if the condition is false in the debug mode.
  /// \see broken_invariant
  /// \see asserts
  /// \see expects
  /// \see ensures
  __forceinline__
  __constant_evaluated__
  void invariant(
    __release_only_maybe_unused__ bool condition,
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(asserts<invariant_contract_violation>(condition, "failed invariant contract check", location);)
  }

  /// \brief Raises an invariant violation.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param message Error message.
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>invariant(false, message, location);</code> in the debug mode and to empty
  /// consteval function in the release mode.
  /// \throws invariant_contract_violation in the debug mode.
  /// \see invariant
  /// \see asserts
  /// \see expects
  /// \see ensures
  __forceinline__
  __constant_evaluated__
  void broken_invariant(
    __release_only_maybe_unused__ std::source_location location = std::source_location::current()
  ) {
    __floppy_internal_remove_in_release__(invariant(false, location);)
  }

  /// \brief Raises an unimplemented contract violation.
  /// \ingroup contracts
  /// \headerfile floppy/floppy.h
  /// \param location Source location of the violation (default is current).
  /// \details Expands to <code>throw invariant_contract_violation("not implemented", location);</code> in the debug mode and to empty
  /// consteval function in the release mode.
  /// \throws invariant_contract_violation in the debug mode.
  /// \see invariant
  /// \see asserts
  /// \see expects
  /// \see ensures
  [[maybe_unused]]
  __forceinline__
  __constant_evaluated__
  void not_implemented(__release_only_maybe_unused__ std::source_location location = std::source_location::current()) {
    __floppy_internal_remove_in_release__(asserts<invariant_contract_violation>(false, "not implemented", location);)
  }
} // namespace floppy::contracts

#undef __constant_evaluated__
#undef __floppy_internal_remove_in_release__