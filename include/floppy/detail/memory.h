#pragma once

#include <memory>
#include <compare>
#include <stdexcept>
#include <floppy/detail/export.h>
#include <floppy/detail/formatters.h>
#include <floppy/detail/concepts.h>
#include <floppy/detail/contracts.h>

namespace floppy
{
  template <typename R, typename T, R (*F)(T)>
  struct deleter
  {
    template <typename U>
    void operator()(U &ptr) const {
      (*F)(ptr);
    }
  };

  template <typename T>
  struct default_delete
  {
    void operator()(T &ptr) const {
      delete ptr;
    }
  };

  /// \brief GSL-like owning raw pointer typedef.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \details <code>fl::owner<T></code> is designed as a safety mechanism for code that must deal
  /// directly with raw pointers that own memory.
  /// Ideally such code should be restricted to the implementation of low-level abstractions.
  /// <code>fl::owner<T></code> can also be used
  /// as a stepping point in converting legacy code to use more modern RAII constructs, such as smart pointers.
  /// \tparam T Pointer type.
  /// \sa https://github.com/microsoft/GSL/blob/main/include/gsl/pointers
  template <concepts::ptr T>
  using owner = T;

  /// \brief Box memory class.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \details The box class behaves very similarly to the unique_ptr, but unlike the
  /// unique_ptr, it is 100% guaranteed to be filled at all times (unless the user
  /// tries to access it after calling std::move does something else that is
  /// clearly bad practice).
  ///
  /// Reference implementation is taken from the <b>reflect-cpp</b> library.
  /// API reference matches (almost) the Rust Language implementation.
  /// \tparam T Type of data stored in the box.
  /// \sa https://github.com/getml/reflect-cpp/blob/main/include/rfl/Box.hpp
  /// \sa https://doc.rust-lang.org/std/boxed/struct.Box.html
  template <typename T>
  class box
  {
   public:
    using element_type = T;

    /// \brief Default constructor.
    box() : ptr_(std::make_unique<T>()) {} // NOLINT(*-use-equals-default)

    /// \brief Copy constructor (deleted).
    box(box const& other) = delete;

    /// \brief Move constructor.
    box(box&& other) noexcept = default;

    /// \brief Move constructor.
    /// \details Takes ownership of <tt>other</tt>
    /// \tparam U Underlying type of <tt>other</tt>.
    /// \param other <tt>box</tt> to move from.
    template <typename U>
    box(box<U>&& other) noexcept // NOLINT(*-explicit-constructor, *-rvalue-reference-param-not-moved)
      : ptr_(std::forward<std::unique_ptr<U>>(other.as_unique_ptr()))
    {}

    /// \brief Default destructor.
    ~box() = default;

    /// \brief Returns a mutable pointer to the underlying object.
    /// \returns A mutable pointer to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T* get() {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns a mutable pointer to the underlying object.
    /// \returns A mutable pointer to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T* get() const {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns a mutable pointer to the underlying object.
    /// \returns A mutable pointer to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T* ptr_mut() {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns an immutable pointer to the underlying object.
    /// \returns An immutable pointer to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T const* ptr() const {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns a mutable reference to the underlying object.
    /// \returns A mutable reference to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T& ref_mut() {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return *this->ptr_;
    }

    /// \brief Returns an immutable reference to the underlying object.
    /// \returns An immutable reference to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T const& ref() const {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return *this->ptr_;
    }

    /// \brief Returns a mutable reference to the underlying object.
    /// \returns A mutable reference to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T& operator*() {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return *this->ptr_;
    }

    /// \brief Returns an immutable reference to the underlying object.
    /// \returns An immutable reference to the underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T const& operator*() const {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return *this->ptr_;
    }

    /// \brief Copy assignment operator (deleted).
    box<T>& operator=(box<T> const& other) = delete;

    /// \brief Move assignment operator.
    box<T>& operator=(box<T>&& other) noexcept = default;

    /// \brief Move assignment operator.
    /// \details Takes ownership of <tt>other</tt>
    /// \tparam U Underlying type of <tt>other</tt>.
    /// \param other <tt>box</tt> to move from.
    /// \returns A reference to <tt>*this</tt>.
    template <typename U>
    box<T>& operator=(box<U>&& other) noexcept // NOLINT(*-rvalue-reference-param-not-moved)
    {
      this->ptr_ = std::forward<std::unique_ptr<U>>(other.as_unique_ptr());
      return *this;
    }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T* operator->() {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    [[nodiscard]] T* operator->() const {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return this->ptr_.get();
    }

    /// \brief Returns the underlying unique pointer.
    /// \returns The underlying unique pointer.
    [[nodiscard]] std::unique_ptr<T>& as_unique_ptr() noexcept { return this->ptr_; }

    /// \brief Returns the constant underlying unique pointer.
    /// \returns The underlying unique pointer.
    [[nodiscard]] std::unique_ptr<T> const& as_unique_ptr() const noexcept { return this->ptr_; }

    /// \brief Consumes and leaks the box, returning a mutable pointer to the underlying object.
    /// \details The caller is responsible for deleting the returned pointer.
    /// Calling this function leaves the <tt>box</tt> in an invalid state.
    /// \returns A mutable unmanaged pointer to the underlying object.
    [[nodiscard]] T* leak() noexcept {
      return this->ptr_.release();
    }

    /// \brief Attempt to downcast the box to a concrete type.
    /// \tparam U Type to downcast to.
    /// \returns An mutable reference to the casted underlying object if successful, <tt>none</tt> otherwise.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    template <typename U>
    [[nodiscard]] option<std::reference_wrapper<U>> downcast() {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      try {
        auto& r = dynamic_cast<U&>(this->ref_mut());
        return std::make_optional(std::ref(r));
      } catch(std::bad_cast const&) {
        return none;
      } catch(...) {
        return none;
      }
    }

    /// \brief Casts the underlying pointer to given type.
    /// \tparam U Type to cast to.
    /// \returns An mutable pointer to the casted underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    template <typename U>
    [[nodiscard]] U* as() {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return static_cast<U*>(this->ptr_mut());
    }

    /// \brief Casts the underlying pointer to constant given type.
    /// \tparam U Type to cast to.
    /// \returns An constant pointer to the casted underlying object.
    /// \throws contracts::contract_violation If the box has been moved from or is leaked/consumed.
    /// \warning This function is non-throwing in release mode. Exception is only thrown in debug mode.
    template <typename U>
    [[nodiscard]] U const* as() const {
      contracts::expects(this->ptr_ != nullptr, "use after consume");
      return static_cast<U*>(this->ptr());
    }

    /// \brief Constructs new box with given arguments.
    /// \param Args Constructor arguments type.
    /// \param args Constructor arguments.
    template <typename... Args>
    [[nodiscard]] static box<T> make(Args&&... args) {
      return box<T>(std::make_unique<T>(std::forward<Args>(args)...));
    }

    /// \brief Tries to construct new box from <tt>std::unique_ptr</tt>.
    /// \param ptr <tt>std::unique_ptr</tt> to construct from.
    /// \throws contracts::contract_violation if <tt>ptr</tt> is not set.
    [[nodiscard]] static box<T> make(std::unique_ptr<T> ptr) noexcept(false) {
      contracts::expects(ptr != nullptr, "ptr must not be null");
      return box<T>(std::move(ptr));
    }

   private:
    explicit box(std::unique_ptr<T>&& _ptr) noexcept : ptr_(std::move(_ptr)) {}

    std::unique_ptr<T> ptr_;
  };

  /// \brief Constructs a new box with given arguments.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  template <typename T, typename... Args>
  [[nodiscard]] box<T> make_box(Args&&... args) {
    return box<T>::make(std::forward<Args>(args)...);
  }

  /// \brief Compares two boxes.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \param a First box.
  /// \param b Second box.
  /// \returns The result of the comparison.
  template <typename T1, typename T2>
  auto operator<=>(box<T1> const& a, box<T2> const& b) {
    return a.as_unique_ptr() <=> b.as_unique_ptr();
  }

  /// \brief Prints a <tt>box</tt> to an output stream.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \param os Output stream.
  /// \param b <tt>box</tt> to print.
  template <typename C, typename Traits, typename T>
  std::basic_ostream<C, Traits>& operator<<(std::basic_ostream<C, Traits>& os, box<T> const& b) {
    return os << *b;
  }
} // namespace floppy

namespace std
{
  /// \brief Hashes a <tt>box</tt>.
  template <typename T>
  struct [[maybe_unused]] hash<floppy::box<T>>
  {
    [[nodiscard]] std::size_t operator()(floppy::box<T> const& b) const {
      return std::hash<std::unique_ptr<T>>()(b.as_unique_ptr());
    }
  };

  /// \brief Swaps two boxes.
  template <typename T>
  [[maybe_unused]] void swap(floppy::box<T>& a, floppy::box<T>& b) noexcept {
    return std::swap(a.as_unique_ptr(), b.as_unique_ptr());
  }
} // namespace std

/// \defgroup memory Memory
/// \ingroup foundation
/// \brief Memory related classes and functions.