#include <fl/contracts.h>

#include <iostream>
#include <utility>
#include "extern/termcolor.hh"

namespace
{
  using namespace fl::contracts;
  contract_violation_handler global_violation_handler = default_contract_violation_handler; // NOLINT(*-avoid-non-const-global-variables)
} // namespace

namespace fl::contracts
{
  void default_contract_violation_handler(const contract_violation& violation) {
    auto type = [&violation]() -> std::string_view {
      switch(violation.type) {
        case contract_type::precondition: return "precondition";
        case contract_type::postcondition: return "postcondition";
        case contract_type::invariant: return "invariant";
        default: return "unknown";
      }
    };
    std::cerr << termcolor::red << termcolor::bold << std::format("Contract violation ({}):\n", type()) << termcolor::reset << termcolor::red;
    std::cerr << std::format("  {}\n", violation.message);
    std::cerr << std::format("  in function \'{}\'\n", violation.location.function_name());
    std::cerr << std::format("  in file \'{}\'\n", violation.location.file_name());
    std::cerr << std::format("  at line {}:{}\n", violation.location.line(), violation.location.column());
    std::cerr << termcolor::reset;
    std::flush(std::cerr);
    std::terminate();
  }

  contract_violation_handler& violation_handler() {
    return ::global_violation_handler;
  }

  contract_violation_handler set_violation_handler(contract_violation_handler handler) {
    auto old = ::global_violation_handler;
    ::global_violation_handler = std::move(handler);
    return old;
  }

  contract_violation detail::make_contract_violation(
    contract_type type,
    std::string_view message,
    std::source_location location
  ) {
    return {
      .type = type,
      .message = std::string(message),
      .location = location
    };
  }

  void detail::violate(
    contract_type type,
    std::string_view message,
    std::source_location location
  ) {
    ::violation_handler()(make_contract_violation(type, message, location));
  }
} // namespace fl::contracts