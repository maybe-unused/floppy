#pragma once

#include <cstdint>
#include <ranges>
#include <fmt/format.h>
#include <floppy/detail/platform.h>
#include <floppy/detail/concepts.h>
#include <floppy/detail/formatters.h>

#if defined(__cpp_lib_source_location)
# include <source_location>
#endif // __cpp_lib_source_location

namespace floppy
{
  /// \brief <tt>std::ranges</tt> backports namespace.
  namespace ranges
  {
    using namespace std::ranges;

    namespace detail
    {
      template <typename C>
      struct to_helper {};

      template <typename C, range R>
        requires std::convertible_to<range_value_t<R>, typename C::value_type>
#ifndef DOXYGEN_GENERATING_OUTPUT
      auto operator|(R&& r, to_helper<C>) -> C
#else
      C operator|(R&& r, to_helper<C>)
#endif
      {
        return C { r.begin(), r.end() };
      }
    } // namespace detail

    /**
     * \brief The overloads of the range conversion function construct a new non-view object from a source range.
     * \details This is an alias to <tt>std::ranges::to</tt>.
     * \sa https://en.cppreference.com/w/cpp/ranges/to
     * \sa to
     */
    template <range C> requires (not view<C>)
    auto collect() { return detail::to_helper<C> {}; }

    /**
     * \brief The overloads of the range conversion function construct a new non-view object from a source range.
     * \sa https://en.cppreference.com/w/cpp/ranges/to
     * \sa collect
     */
    template <range C> requires (not view<C>)
    auto to() { return detail::to_helper<C> {}; }
  } // namespace ranges

  /**
   * \brief Invokes undefined behavior.
   * \details https://en.cppreference.com/w/cpp/utility/unreachable
   */
#ifndef DOXYGEN_GENERATING_OUTPUT
  [[noreturn]] inline auto unreachable() -> void {
#else
  inline void unreachable() {
#endif
    #if defined(FLOPPY_COMPILER_MSVC) && !defined(FLOPPY_COMPILER_CLANG)
        __assume(false);
    #else
        __builtin_unreachable();
    #endif
  }

  /**
   * \brief Converts enum type to its underlying type.
   * \details This is backport of C++20 (23) std::to_underlying.
   * \tparam T Enum type. Must be an enum type.
   * \param t Enum value
   * \return Underlying value of the enum
   * \sa https://en.cppreference.com/w/cpp/utility/to_underlying
   */
  template <concepts::enum_ T>
#ifndef DOXYGEN_GENERATING_OUTPUT
  constexpr inline auto to_underlying(T t) noexcept -> std::underlying_type_t<T> {
#else
  constexpr inline std::underlying_type_t<T> to_underlying(T t) noexcept
#endif
    return static_cast<std::underlying_type_t<T>>(t);
  }

  /**
   * \brief Provides information about the location of the current source file.
   * \details Class provides information about the location of the current source file.
   * This class is a port of C++20 std::source_location.
   * Example usage:
   * ```cpp
   * int main() {
       llog::debug("I am called from {}, {}:{} in file {}!",
         lf::source_location::current().function_name(),
         lf::source_location::current().line(),
         lf::source_location::current().column(),
         lf::source_location::current().file_name()
       );
       llog::debug("I am called from {}", lf::source_location::current());
     }
   * ```
   *
   * Output:
   * ```sh
     I am called from main, 5:7 in file main.c++!
     I am called from main.c++:main 6:7
   * ```
   * \note This implementation can be aliased to `std::source_location`, if the compiler supports it.
   * Otherwise, it is a self-contained port of C++20 std::source_location.
   */
  #if defined(__cpp_lib_source_location)
    using source_location = std::source_location;
  #else // __cpp_lib_source_location
    struct source_location
    {
     private:
      constexpr static auto UNKNOWN = "(unknown)";

     public:
  #if not defined(__apple_build_version__) and defined(__clang__) and (__clang_major__ >= 9)
      static consteval auto current(
        char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION(),
        uint_least32_t line = __builtin_LINE(),
        uint_least32_t column = __builtin_COLUMN()
          ) noexcept -> source_location
  #elif defined(__GNUC__) and (__GNUC__ > 4 or (__GNUC__ == 4 and __GNUC_MINOR__ >= 8))
      static consteval auto current(
        char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION(),
        uint_least32_t line = __builtin_LINE(),
        uint_least32_t column = 0
        ) noexcept -> source_location
  #else // compiler
      static consteval auto current(
        char const* file = UNKNOWN,
        char const* function = UNKNOWN,
        uint_least32_t line = 0,
        uint_least32_t column = 0
        ) noexcept -> source_location
  #endif // compiler
      {
        return { file, line, function, column };
      }

     private:
      const char* m_file;
      const char* m_function;
      uint_least32_t m_line;
      uint_least32_t m_column;

     public:

      /// \brief Creates an empty invalid source_location.
      constexpr source_location() noexcept
        : m_file(UNKNOWN)
        , m_function(UNKNOWN)
        , m_line(0)
        , m_column(0)
      {}

      /**
       * \brief Creates a source_location from given parameters.
       * \param file File name
       * \param line Number of the line in the file. 0 if NULL
       * \param function Function name
       * \param column Column number in the function. Defaults to 0
       */
      constexpr source_location(
        char const* file,
        uint_least32_t const line,
        char const* function,
        uint_least32_t const column = 0
      ) noexcept
        : m_file(file)
        , m_function(function)
        , m_line(line)
        , m_column(column)
      {}

      /// \brief Returns the file name.
#ifndef DOXYGEN_GENERATING_OUTPUT
      [[nodiscard]] constexpr auto file_name() const noexcept -> char const* { return this->m_file; }
#else
      [[nodiscard]] constexpr char const* file_name() const noexcept { return this->m_file; }
#endif

      /// \brief Returns the function name. NULL if unknown or not available on compiler
#ifndef DOXYGEN_GENERATING_OUTPUT
      [[nodiscard]] constexpr auto function_name() const noexcept -> char const* { return this->m_function; }
#else
      [[nodiscard]] constexpr char const* function_name() const noexcept { return this->m_function; }
#endif

      /// \brief Returns the line number. 0 if unknown or not available on compiler
#ifndef DOXYGEN_GENERATING_OUTPUT
      [[nodiscard]] constexpr auto line() const noexcept -> uint_least32_t { return this->m_line; }
#else
      [[nodiscard]] constexpr uint_least32_t line() const noexcept { return this->m_line; }
#endif

      /// \brief Returns the column number. 0 if unknown or not available on compiler
#ifndef DOXYGEN_GENERATING_OUTPUT
      [[nodiscard]] constexpr auto column() const noexcept -> uint_least32_t { return this->m_column; }
#else
      [[nodiscard]] constexpr uint_least32_t column() const noexcept { return this->m_column; }
#endif
    };

    /// \brief Stream adaptor for source_location
    template <class E, class T>
#ifndef DOXYGEN_GENERATING_OUTPUT
    auto operator<<(std::basic_ostream<E, T>& os, source_location const& loc) -> std::basic_ostream<E, T>&
#else
    std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& os, source_location const& loc)
#endif
    {
      os.width(0);
      if(loc.line() == 0)
        os << "(unknown)";
      else {
        os << fmt::format("{}:{}", loc.file_name(), loc.line());
        if(loc.column())
          os << fmt::format(":{}", loc.column());
        os << fmt::format(": in fn {} ", loc.function_name());
      }
      return os;
    }
  #endif // __cpp_lib_source_location
} // namespace floppy

template <> struct fmt::formatter<floppy::source_location> : floppy::ostream_formatter<char> {};