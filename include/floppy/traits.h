#pragma once

#include <memory>
#include <experimental/propagate_const>
#include <floppy/detail/export.h>

/// \brief Namespace with traits for custom types and classes.
namespace floppy::traits
{
  /// \brief Disallow copy operations for a type
  /// \details Example declaration:
  /// \code {.cpp}
  /// class Test : public fl::traits::non_copyable<Test> {}; // copy is not allowed for Test
  /// \endcode
  /// \tparam T Class type
  /// \see non_movable
  /// \see pin
  template <typename T>
  struct non_copyable
  {
    non_copyable() = default;
    non_copyable(const T&) = delete;
    T& operator=(const T&) = delete;
  };

  /// \brief Disallow move operations for a type
  /// \details Example declaration:
  /// \code {.cpp}
  /// class Test : public fl::traits::non_movable<Test> {}; // move is not allowed for Test
  /// \endcode
  /// \tparam T Class type
  /// \see non_copyable
  /// \see pin
  template <typename T>
  struct non_movable
  {
    non_movable() = default;
    non_movable(T&&) = delete;
    T& operator=(T&&) = delete;
  };

  /// \brief Disallow copy and move operations for a type
  /// \details Example declaration:
  /// \code {.cpp}
  /// class Test : public fl::traits::pin<Test> {}; // copy and move is not allowed for Test
  /// \endcode
  /// \tparam T Class type
  /// \see non_movable
  /// \see non_copyable
  template <typename T>
  struct pin : public non_copyable<T>, public non_movable<T> {};

  /// \brief Singleton pattern trait.
  /// \details Allows to use <b>Singleton</b> pattern in custom types and classes.
  /// Example declaration:
  /// \code {.cpp}
  /// class TestSingleton : public fl::traits::singleton<TestSingleton>
  /// {
  ///   friend struct fl::traits::singleton<TestSingleton>;
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
  struct singleton : public pin<T>
  {
    /// \brief Returns \i mutable \i pointer to the singleton instance.
    /// \return Mutable pointer to the singleton instance.
    /// \see ptr
    [[nodiscard]] static auto ptr_mut() noexcept -> T* {
      static T instance;
      return &instance;
    }

    /// \brief Returns \i pointer to the singleton instance.
    /// \return Pointer to the singleton instance.
    /// \see ptr_mut
    [[nodiscard]] static inline auto ptr() noexcept -> T const* { return ptr_mut(); }

    /// \brief Returns \i constant \i reference to the singleton instance.
    /// \return Constant reference to the singleton instance.
    /// \see ref_mut
    [[nodiscard]] static inline auto ref() noexcept -> T const& { return *ptr(); }

    /// \brief Returns \i mutable \i reference to the singleton instance.
    /// \return Mutable reference to the singleton instance.
    /// \see ref
    [[nodiscard]] static inline auto ref_mut() noexcept -> T& { return *ptr_mut(); }

    singleton() = default;
  };

  /// \brief Pointer-to-implementation pattern trait.
  /// \details Allows to use <b>Pimpl</b> pattern in custom types and classes.
  /// Example declaration:
  /// \code {.cpp}
  /// class TestPimpl : public traits::pin<TestPimpl>
  /// {
  ///  public:
  ///   TestPimpl() = default;
  ///
  ///   [[nodiscard]] auto add(int a, int b) const -> int;
  ///
  ///  private:
  ///   struct Impl;
  ///   traits::pimpl<struct Impl> impl;
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
  using pimpl = std::experimental::propagate_const<std::unique_ptr<T>>;
} // namespace floppy::traits