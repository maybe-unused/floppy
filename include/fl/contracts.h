#pragma once

#include <source_location>
#include <string>
#include <string_view>
#include <format>
#include <functional>
#include "global/definitions.h"
#include "global/export.h"

/**
 * @brief Contract-programming related functions and classes.
 * @details Mimics the canceled C++20 contracts proposal.
 */
namespace fl::contracts
{
  /**
   * @brief Type of contract.
   */
  enum class contract_type : signed char
  {
    precondition,  ///< Precondition.
    postcondition, ///< Postcondition.
    invariant      ///< Invariant.
  };

  /**
   * @brief Contract violation data holder.
   * @details Contains information about a contract violation.
   * @see contract_violation_handler
   */
  struct contract_violation
  {
    contract_type type;            ///< Violated contract type.
    std::string message;           ///< Violation message.
    std::source_location location; ///< Violation location in source code.
  };

  /**
   * @brief Contract violation handler type.
   * @note Handler function must be <code>[[noreturn]]</code>. The behavior is undefined if it does not terminate the program.
   */
  using contract_violation_handler = std::function<void(contract_violation const&)>;

  /**
   * @brief Default contract violation handler.
   * @details Default implementation of the contract violation handler.
   *
   * Prints the violation to the standard error stream and terminates the program.
   * If it is possible, colors the violation message in red.
   *
   * Example output:
   *
   * @code
   * Contract violation (invariant):
   *    Hello, world!
   *    in function 'int __cdecl main(int,char **)'
   *    in file 'D:/dev/my/floppy-dev/bin/main.cc'
   *    at line 7:18
   *
   * Process finished with exit code 3
   * @endcode
   * @param violation Contract violation data.
   */
  [[noreturn]]
  #ifndef FL_DOC
  ___fl_api___
  #endif // FL_DOC
  void default_contract_violation_handler(contract_violation const& violation);

  /**
   * @brief Returns the current global contract violation handler.
   * @return Mutable reference to the global contract violation handler.
   * @see default_contract_violation_handler
   * @see set_violation_handler
   */
  [[nodiscard]]
  #ifndef FL_DOC
  ___fl_api___
  #endif // FL_DOC
  contract_violation_handler& violation_handler();

  /**
   * @brief Sets the global contract violation handler and returns the old one.
   * @note Handler function must be <code>[[noreturn]]</code>. The behavior is undefined if it does not terminate the program.
   * @param handler Contract violation handler.
   * @return Old global contract violation handler.
   * @see violation_handler
   * @see default_contract_violation_handler
   */
  #ifndef FL_DOC
  ___fl_api___
  #endif // FL_DOC
  contract_violation_handler set_violation_handler(contract_violation_handler handler);

  /**
   * @brief Contracts implementation details.
   */
  namespace detail
  {
    /**
     * @brief Creates a contract violation data structure from current location in the source code.
     * @param type Type of violated contract.
     * @param message Violation message.
     * @param location Violation location in source code. Defaults to current location.
     * @return Contract violation data structure.
     * @see violate
     */
    [[nodiscard]]
    #ifndef FL_DOC
    ___fl_api___
    #endif // FL_DOC
    contract_violation make_contract_violation(
      contract_type type,
      std::string_view message,
      std::source_location location = std::source_location::current()
    );

    /**
     * @brief Invokes the global contract violation handler (see @ref violation_handler) with the violation data from current location in the source code.
     * @param type Type of violated contract.
     * @param message Violation message.
     * @param location Violation location in source code. Defaults to current location.
     * @see make_contract_violation
     */
    [[noreturn]]
    #ifndef FL_DOC
    ___fl_api___
    #endif // FL_DOC
    void violate(
      contract_type type,
      std::string_view message,
      std::source_location location = std::source_location::current()
    );
  } // namespace detail

  /**
   * @brief Checks if a given <i>invariant</i> expression is true and throws a contract violation if it is not.
   * @details If the expression is false, the contract violation is thrown with the given message and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
   * @param expression Invariant expression.
   * @param message Violation message. Defaults to <code>"Invariant violated"</code>.
   * @see broken_invariant
   * @see precondition
   * @see postcondition
   */
  #ifndef FL_DOC
  ___inline___
  ___fl_release_consteval___
  #endif // FL_DOC
  void invariant(
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    bool expression,
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    std::string_view message = "Invariant violated",
    #ifndef FL_DOC
    ___fl_release_unused___ std::source_location location = std::source_location::current()
    #endif // FL_DOC
  ) {
    ___fl_debug_only___(if(not expression) detail::violate(contract_type::invariant, message, location));
  }

  /**
   * @brief Checks if a given <i>precondition</i> expression is true and throws a contract violation if it is not.
   * @details If the expression is false, the contract violation is thrown with the given message and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
   * @param expression Precondition expression.
   * @param message Violation message. Defaults to <code>"Precondition violated"</code>.
   * @param location Violation location in source code. Defaults to current location.
   * @see broken_precondition
   * @see invariant
   * @see postcondition
   */
  #ifndef FL_DOC
  ___inline___
  ___fl_release_consteval___
  #endif // FL_DOC
  void precondition(
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    bool expression,
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    std::string_view message = "Precondition violated",
    #ifndef FL_DOC
    ___fl_release_unused___ std::source_location location = std::source_location::current()
    #endif // FL_DOC
  ) {
    ___fl_debug_only___(if(not expression) detail::violate(contract_type::precondition, message, location));
  }

  /**
   * @brief Checks if a given <i>postcondition</i> expression is true and throws a contract violation if it is not.
   * @details If the expression is false, the contract violation is thrown with the given message and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
   * @param expression Postcondition expression.
   * @param message Violation message. Defaults to <code>"Postcondition violated"</code>.
   * @see broken_postcondition
   * @see invariant
   * @see precondition
   */
  #ifndef FL_DOC
  ___inline___
  ___fl_release_consteval___
  #endif // FL_DOC
  void postcondition(
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    bool expression,
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    std::string_view message = "Postcondition violated",
    #ifndef FL_DOC
    ___fl_release_unused___ std::source_location location = std::source_location::current()
    #endif // FL_DOC
  ) {
    ___fl_debug_only___(if(not expression) detail::violate(contract_type::postcondition, message, location));
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken invariant violation.
   * @details If the expression is false, the contract violation is thrown with the given message and location.
   * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
   * @param message Violation message. Defaults to <code>"Broken invariant"</code>.
   * @see invariant
   */
  #ifndef FL_DOC
  ___inline___
  ___fl_release_consteval___
  #endif // FL_DOC
  void broken_invariant(
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    std::string_view message = "Broken invariant",
    #ifndef FL_DOC
    ___fl_release_unused___ std::source_location location = std::source_location::current()
    #endif // FL_DOC
  ) {
    ___fl_debug_only___(detail::violate(contract_type::invariant, message, location));
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken precondition violation.
   * @param message Violation message. Defaults to <code>"Broken precondition"</code>.
   * @see precondition
   */
  #ifndef FL_DOC
  ___inline___
  ___fl_release_consteval___
  #endif // FL_DOC
  void broken_precondition(
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    std::string_view message = "Broken precondition",
    #ifndef FL_DOC
    ___fl_release_unused___
   std::source_location location = std::source_location::current()
    #endif // FL_DOC
  ) {
    ___fl_debug_only___(detail::violate(contract_type::precondition, message, location));
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken postcondition violation.
   * @param message Violation message. Defaults to <code>"Broken postcondition"</code>.
   * @see postcondition
   */
  #ifndef FL_DOC
  ___inline___
  ___fl_release_consteval___
  #endif // FL_DOC
  void broken_postcondition(
    #ifndef FL_DOC
    ___fl_release_unused___
    #endif // FL_DOC
    std::string_view message = "Broken postcondition",
    #ifndef FL_DOC
    ___fl_release_unused___ std::source_location location = std::source_location::current()
    #endif // FL_DOC
  ) {
    ___fl_debug_only___(detail::violate(contract_type::postcondition, message, location));
  }

  /**
   * @brief Invokes the global contract violation handler (see @ref violation_handler) with a broken invariant violation.
   * @details Useful for marking a function as <i>unimplemented</i>.
   * @note The expression is only evaluated in debug mode. In release mode this function is a no-op.
   * @see invariant
   */
  #ifndef FL_DOC
  ___inline___
  ___fl_release_consteval___
  #endif // FL_DOC
  void not_implemented(
    #ifndef FL_DOC
    ___fl_release_unused___ std::source_location location = std::source_location::current()
    #endif // FL_DOC
  ) {
    ___fl_debug_only___(detail::violate(contract_type::invariant, "Reached unimplemented code", location));
  }
} // namespace fl::contracts
