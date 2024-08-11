#pragma once

#include <iostream>
#include <type_traits>
#include <floppy/floppy.h>

namespace floppy
{
  template <typename T>
  class propagate_const : public T
  {
   public:
    using T::operator=;

    propagate_const() = default;

    template <typename... Args>
    inline propagate_const(Args&&... args)
      : T(std::forward<Args>(args)...)
    {}

    [[nodiscard]] inline auto operator*() -> typename T::element_type& {
      return T::operator*();
    }

    [[nodiscard]] inline auto operator*() const -> typename T::element_type const& {
      return T::operator*();
    }

    [[nodiscard]] inline auto operator->() -> typename T::element_type* {
      return T::operator->();
    }

    [[nodiscard]] inline auto operator->() const -> typename T::element_type const* {
      return T::operator->();
    }

    [[nodiscard]] inline auto get() -> typename T::element_type* {
      return T::get();
    }

    [[nodiscard]] inline auto get() const -> typename T::element_type const* {
      return T::get();
    }

    [[nodiscard]] inline operator bool() const noexcept {
      return T::operator bool();
    }
  };

  /// \brief Disallow copy operations for a type
  /// \headerfile floppy/traits.h 
  /// \ingroup traits
  /// \details Example declaration:
  /// \code {.cpp}
  /// class Test : fl::non_copyable {}; // copy is not allowed for Test
  /// \endcode
  /// \see non_movable
  /// \see pin
  struct non_copyable // NOLINT(*-special-member-functions)
  {
    non_copyable() = default;
    ~non_copyable() = default;
    non_copyable(non_copyable const&) = delete;
    auto operator=(non_copyable const&) -> non_copyable& = delete;
  };

  /// \brief Disallow move operations for a type
  /// \headerfile floppy/traits.h 
  /// \ingroup traits
  /// \details Example declaration:
  /// \code {.cpp}
  /// class Test : fl::non_movable<Test> {}; // move is not allowed for Test
  /// \endcode
  /// \see non_copyable
  /// \see pin
  struct non_movable // NOLINT(*-special-member-functions)
  {
    non_movable() = default;
    ~non_movable() = default;
    non_movable(non_movable&&) = delete;
    auto operator=(non_movable&&) noexcept -> non_movable& = delete;
  };

  /// \brief Disallow copy and move operations for a type
  /// \headerfile floppy/traits.h 
  /// \ingroup traits
  /// \details Example declaration:
  /// \code {.cpp}
  /// class Test : fl::pin<Test> {}; // copy and move is not allowed for Test
  /// \endcode
  /// \see non_movable
  /// \see non_copyable
  struct [[maybe_unused]] pin : public non_copyable, public non_movable {};

  /// \brief Singleton pattern trait.
  /// \headerfile floppy/traits.h 
  /// \ingroup traits
  /// \details Allows to use <b>Singleton</b> pattern in custom types and classes.
  /// Example declaration:
  /// \code {.cpp}
  /// class TestSingleton : public fl::singleton<TestSingleton>
  /// {
  ///   friend struct fl::singleton<TestSingleton>;
  ///
  ///  public:
  ///   ~TestSingleton() = default;
  ///
  ///   [[nodiscard]] auto add(int a, int b) const -> int { return a + b; }
  ///
  ///  private:
  ///   TestSingleton() = default;
  /// };
  /// \endcode
  /// Example usage:
  /// \code {.cpp}
  /// TestSingleton::ref().add(1, 2);
  /// TestSingleton::ref_mut().add(1, 2);
  /// TestSingleton::ptr()->add(1, 2);
  /// TestSingleton::ptr_mut()->add(1, 2);
  /// \endcode
  /// \note Produced singleton is thread-safe.
  /// \tparam T Class type.
  template <typename T>
  struct singleton : pin
  {
    /// \brief Returns <i>mutable pointer</i> to the singleton instance.
    /// \return Mutable pointer to the singleton instance.
    /// \see ptr
    [[nodiscard]] static auto ptr_mut() noexcept -> T* {
      static T instance;
      return &instance;
    }

    /// \brief Returns <i>pointer</i> to the singleton instance.
    /// \return Pointer to the singleton instance.
    /// \see ptr_mut
    [[nodiscard]] static auto ptr() noexcept -> T const* { return ptr_mut(); }

    /// \brief Returns <i>constant reference</i> to the singleton instance.
    /// \return Constant reference to the singleton instance.
    /// \see ref_mut
    [[nodiscard]] static auto ref() noexcept -> T const& { return *ptr(); }

    /// \brief Returns <i>mutable reference</i> to the singleton instance.
    /// \return Mutable reference to the singleton instance.
    /// \see ref
    [[nodiscard]] static auto ref_mut() noexcept -> T& { return *ptr_mut(); }

    singleton() = default;
  };

  /// \brief Pointer-to-implementation pattern trait.
  /// \headerfile floppy/traits.h 
  /// \ingroup traits
  /// \details Allows to use <b>Pimpl</b> pattern in custom types and classes.
  /// Example declaration:
  /// \code {.cpp}
  /// class TestPimpl : fl::pin
  /// {
  ///  public:
  ///   TestPimpl() = default;
  ///
  ///   [[nodiscard]] auto add(int a, int b) const -> int;
  ///
  ///  private:
  ///   struct Impl;
  ///   fl::pimpl<struct Impl> impl;
  /// };
  ///
  ///  struct TestPimpl::Impl {
  ///   auto add(int a, int b) const -> int { return a + b; }
  /// };
  ///
  ///  auto TestPimpl::add(int a, int b) const -> int {
  ///   return this->impl->add(a, b);
  /// }
  /// \endcode
  /// \tparam T Underlying type.
  template <typename T>
  using pimpl = propagate_const<box<T>>;

  /// \brief Implementation details of traits.
  namespace detail
  {
    /// \brief Interface, providing <tt>to_string</tt> method.
    /// \tparam C Character type.
    template <typename C>
    struct formattable_base
    {
      /// \brief Returns string representation of the object.
      /// \return String representation of the object.
      [[nodiscard]] virtual auto to_string() const -> std::basic_string<C> = 0;

      /// \brief Returns string representation of the object.
      /// \return String representation of the object.
      /// \see to_string
      [[nodiscard]] explicit operator std::basic_string<C>() const { return this->to_string(); }
    };

    #if defined(FL_CXX20) || defined(FL_DOC)
    /// \brief Check if T is derived from formattable_base<C>.
    /// \tparam T Type to check.
    /// \tparam C Character type.
    /// \invariant Available in C++20 or above.
    template <typename T, typename C>
    concept derived_from_formattable = std::is_base_of_v<formattable_base<C>, T>;
    #endif // C++20
  } // namespace detail

  /// \brief Allows type to be converted to string and printed via standard streams or <b>fmt</b>.
  /// \headerfile floppy/traits.h 
  /// \ingroup traits
  /// \tparam T Type to convert.
  /// \tparam C Character type.
  /// \see detail::formattable_base
  /// \see detail::derived_from_formattable
  template <typename T, typename C>
  struct formattable : public detail::formattable_base<C>
  {
    /// \brief Writes string representation of the object to the stream.
    friend auto operator<<(std::basic_ostream<C>& os, T const& t) -> std::basic_ostream<C>& {
      return os << t.to_string();
    }
  };
} // namespace floppy

namespace floppy
{
  namespace detail
  {
    /// \brief Casts types which derives from <tt>formattable_base<C></tt> to string.
    /// \tparam C Character type.
    /// \tparam T Type to cast.
    /// \see floppy::traits::formattable
    template <typename C, __wrap_concept__(floppy::detail::derived_from_formattable<C>) T>
    [[nodiscard]] auto str_cast(T const& t) noexcept -> std::string {
      return static_cast<std::string>(t);
    }
  } // namespace detail

  /// \brief Casts type to string.
  /// \details If the type has method <tt>to_string</tt>, calls it. Otherwise, calls <tt>std::to_string</tt>.
  /// \tparam T Type to cast.
  /// \see detail::str_cast
  /// \todo C++17 support
  template <typename T>
  auto string_cast(T const& t) -> std::string
  requires std::is_same_v<decltype(std::declval<T const&>().to_string()), std::string> {
    return t.to_string();
  }

  /// \brief Casts type to string.
  /// \details Specialization for method <tt>std::to_string</tt>.
  /// \tparam T Type to cast.
  /// \see detail::str_cast
  /// \todo C++17 support
  template <typename T>
  auto string_cast(T const& t) -> std::string
  requires std::is_same_v<decltype(std::to_string(std::declval<T&>())), std::string> {
    return std::to_string(t);
  }
} // namespace floppy

/// \struct fmt::formatter
/// \tparam T Type to format. Must satisfy <code>std::convertible_to<std::string>></code>.
/// \brief Formatter for types which can be easily converted to string.
/// \details Removed reference implementation:
/// \code {.cpp}
/// template <std::convertible_to<std::string> T>
/// struct [[maybe_unused]] fmt::formatter<T>
/// {
///   /// \brief Required by <tt>fmt</tt>.
///   template <typename ParseContext>
///   constexpr auto parse(ParseContext& ctx) const { return ctx.begin();}
///
///   /// \brief Required by <tt>fmt</tt>.
///   template <typename FormatContext>
///   constexpr auto format(T const& c, FormatContext& ctx) const {
///     return format_to(ctx.out(), "{}", static_cast<std::string>(c));
///   }
/// };
/// \endcode
/// \headerfile floppy/traits.h
/// \deprecated Removed due to ambiguity with <tt>fmt</tt> impaired with <tt>spdlog</tt> library.

/// \brief Formatter for types which derives from <tt>formattable_base<char></tt>
/// \todo C++17 support.
template <__wrap_concept__(floppy::detail::derived_from_formattable<char>) T>
struct [[maybe_unused]] fmt::formatter<T>
{
  /// \brief Required by <tt>fmt</tt>.
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) const { return ctx.begin(); }

  /// \brief Required by <tt>fmt</tt>.
  template <typename FormatContext>
  constexpr auto format(T const& c, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}", c.to_string());
  }
};

/// \defgroup traits Traits
/// \ingroup foundation
/// \brief Traits for your classes and types.
/// \todo Add detailed description.