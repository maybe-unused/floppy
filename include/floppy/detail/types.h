#pragma once

#include <optional>
#include <type_traits>
#include <fmt/format.h>
#include <tl/expected.hpp>
#include <floppy/detail/cxx_ver.h>
#include <floppy/detail/stdint.h>

namespace floppy // NOLINT(*-concat-nested-namespaces)
{
  /// \brief Inline namespace for types and type aliases.
  /// \headerfile floppy/floppy.h
  /// \ingroup aliases
  inline namespace types
  {
    /// \brief Safe alias for <tt>std::optional</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \details Defaults to <tt>std::optional</tt> if available, otherwise <tt>std::experimental::optional</tt>.
    /// Example usage with helper functions:
    /// \code {.cpp}
    ///  template <typename T>
    ///  auto first(std::vector<T> const& vec) -> option<T> {
    ///    if(vec.empty())
    ///     return none;
    ///    return some(vec.front());
    ///  }
    /// \endcode
    /// \see some
    /// \see none
    /// \sa https://en.cppreference.com/w/cpp/utility/optional
    template <__wrap_concept__(std::destructible) T>
    using option = std::optional<T>;

    /// \brief Types implementation details.
    namespace detail_
    {
      using tl::expected;
      using tl::unexpected;
    } // namespace detail_

    /// \brief Expected type with default error type (<tt>std::string</tt>).
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \details Provides result type, which defaults to <tt>std::expected</tt>, if
    /// available, or <tt>tl::expected</tt>/<tt>std::experimental::expected</tt> otherwise.
    ///
    /// Example usage with void return type:
    /// \code {.cpp}
    /// using namespace fl::types;
    ///
    /// auto returns_void(int x) -> result<> {
    ///   if(x > 0)
    ///     return error("x is positive, should be negative ({})", x);
    ///   return Ok();
    /// }
    /// \endcode
    ///
    /// Example usage with non-void return type:
    /// \code {.cpp}
    /// using namespace fl::types;
    /// auto divide_by_then_sqrt(float a, float x) -> result<f32> {
    ///   if(x == 0)
    ///     return error("division by zero");
    ///   return ok(a / x).map(std::sqrt);
    /// }
    /// \endcode
    template <typename T = void>
    using result = detail_::expected<T, std::string>;

    static_assert(std::is_same_v<result<>, detail_::expected<void, std::string>>);

    /// \brief Helper function for <tt>result</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    template<typename... Args>
    [[nodiscard]] auto error(std::string_view format, Args&&... args) -> detail_::unexpected<std::decay_t<std::string>> {
      return detail_::unexpected<std::decay_t<std::string>>(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
    }

    /// \brief Helper function for <tt>result</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    template <class T>
    [[nodiscard]] auto ok(T&& t) -> detail_::expected<std::decay_t<T>, std::string> {
      return detail_::expected<std::decay_t<T>, std::string>(std::forward<T>(t));
    }

    /// \brief Helper function for <tt>result</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    [[nodiscard]] inline auto ok() -> result<> { return {}; }

    /// \brief Helper function for \ref option.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \see none
    template <std::destructible T>
    auto some(T&& t) -> option<std::decay_t<T>> {
      return option<std::decay_t<T>>(std::forward<T>(t));
    }

    /// \brief Helper function for \ref option.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \see some
    inline constexpr auto none = std::nullopt;

    /// \brief Shortcut for <tt>std::reference_wrapper</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \deprecated Use <code>std::ref</code> or <code>std::cref</code> instead.
    template <typename T>
    using ref = std::reference_wrapper<T>;
  } // namespace types
} // namespace floppy

/// \defgroup aliases Aliases
/// \brief Aliases for types.