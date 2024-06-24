#pragma once

#include <memory>
#include <compare>
#include <stdexcept>
#include <floppy/detail/export.h>
#include <floppy/detail/formatters.h>

namespace floppy
{
  /// \brief Box memory class.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \details The box class behaves very similarly to the unique_ptr, but unlike the
  /// unique_ptr, it is 100% guaranteed to be filled at all times (unless the user
  /// tries to access it after calling std::move does something else that is
  /// clearly bad practice).
  ///
  /// Reference implementation is taken from the <b>reflect-cpp</b> library.
  /// \tparam T Type of data stored in the box.
  /// \sa https://github.com/getml/reflect-cpp/blob/main/include/rfl/Box.hpp
  template <typename T>
  class box
  {
   public:
    /// \brief Constructs new box with given arguments.
    /// \param Args Constructor arguments type.
    /// \param args Constructor arguments.
    template <typename... Args>
    explicit box(Args&&... args) : ptr_(std::make_unique<T>(std::forward<Args>(args)...)) {}

    /// \brief Tries to construct new box from <tt>std::unique_ptr</tt>.
    /// \param ptr <tt>std::unique_ptr</tt> to construct from.
    /// \throws std::invalid_argument if <tt>ptr</tt> is not set.
    explicit box(std::unique_ptr<T> ptr) noexcept(false) {
      if(not ptr)
        throw std::invalid_argument("box::ctor: ptr is not set");
      this->ptr_ = std::move(ptr);
    }

    /// \brief Default constructor.
    box() : ptr_(std::make_unique<T>()) {}

    /// \brief Copy constructor (deleted).
    box(box const& other) = delete;

    /// \brief Move constructor.
    box(box&& other) noexcept = default;

    /// \brief Move constructor.
    /// \details Takes ownership of <tt>other</tt>
    /// \tparam U Underlying type of <tt>other</tt>.
    /// \param other <tt>box</tt> to move from.
    template <typename U>
    box(box<U>&& other) noexcept
      : ptr_(std::forward<std::unique_ptr<U>>(other.ptr()))
    {}

    /// \brief Default destructor.
    ~box() = default;

    /// \brief Returns a pointer to the underlying object.
    /// \returns A pointer to the underlying object.
    [[nodiscard]] auto get() noexcept -> T* { return this->ptr_.get(); }

    /// \brief Copy assignment operator (deleted).
    auto operator=(box<T> const& other) -> box<T>& = delete;

    /// \brief Move assignment operator.
    auto operator=(box<T>&& other) noexcept -> box<T>& = default;

    /// \brief Move assignment operator.
    /// \details Takes ownership of <tt>other</tt>
    /// \tparam U Underlying type of <tt>other</tt>.
    /// \param other <tt>box</tt> to move from.
    /// \returns A reference to <tt>*this</tt>.
    template <typename U>
    auto operator=(box<U>&& other) noexcept
      -> box<T>&
    {
      this->ptr_ = std::forward<std::unique_ptr<U>>(other.ptr());
      return *this;
    }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    [[nodiscard]] auto operator*() noexcept -> T& { return *this->ptr_; }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    [[nodiscard]] auto operator*() const noexcept -> T& { return *this->ptr_; }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    [[nodiscard]] auto operator->() noexcept -> T* { return this->ptr_.get(); }

    /// \brief Returns the underlying object.
    /// \returns The underlying object.
    [[nodiscard]] auto operator->() const noexcept -> T* { return this->ptr_.get(); }

    /// \brief Returns the underlying unique pointer.
    /// \returns The underlying unique pointer.
    [[nodiscard]] auto ptr() noexcept -> std::unique_ptr<T>& { return this->ptr_; }

    /// \brief Returns the constant underlying unique pointer.
    /// \returns The underlying unique pointer.
    [[nodiscard]] auto ptr() const noexcept -> std::unique_ptr<T> const& { return this->ptr_; }

    /// \brief Constructs new box with given arguments.
    /// \param Args Constructor arguments type.
    /// \param args Constructor arguments.
    template <typename... Args>
    [[nodiscard]] static auto make(Args&&... args) -> box<T> {
      return box<T>(std::make_unique<T>(std::forward<Args>(args)...));
    }

    /// \brief Tries to construct new box from <tt>std::unique_ptr</tt>.
    /// \param ptr <tt>std::unique_ptr</tt> to construct from.
    /// \throws std::invalid_argument if <tt>ptr</tt> is not set.
    [[nodiscard]] static auto make(std::unique_ptr<T> ptr) noexcept(false) -> box<T> {
       if(not ptr)
         throw std::invalid_argument("box::make: ptr is not set");
       return box<T>(std::move(ptr));
    }

   private:
    explicit box(std::unique_ptr<T>&& _ptr) noexcept : ptr_(std::move(_ptr)) {}

   private:
    std::unique_ptr<T> ptr_;
  };

  /// \brief Constructs a new box with given arguments.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  template <typename T, typename... Args>
  [[nodiscard]] auto make_box(Args&&... args) -> box<T> {
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
    return a.ptr() <=> b.ptr();
  }

  /// \brief Prints a <tt>box</tt> to an output stream.
  /// \headerfile floppy/floppy.h
  /// \ingroup memory
  /// \param os Output stream.
  /// \param b <tt>box</tt> to print.
  template <typename C, typename Traits, typename T>
  auto operator<<(std::basic_ostream<C, Traits>& os, box<T> const& b) -> std::basic_ostream<C, Traits>& {
    return os << *b;
  }
} // namespace floppy

namespace std
{
  /// \brief Hashes a <tt>box</tt>.
  template <typename T>
  struct [[maybe_unused]] hash<floppy::box<T>>
  {
    [[nodiscard]] auto operator()(floppy::box<T> const& b) const -> std::size_t {
      return std::hash<std::unique_ptr<T>>()(b.ptr());
    }
  };

  /// \brief Swaps two boxes.
  template <typename T>
  [[maybe_unused]] auto swap(floppy::box<T>& a, floppy::box<T>& b) noexcept -> void {
    return std::swap(a.ptr(), b.ptr());
  }
} // namespace std

/// \defgroup memory Memory
/// \ingroup foundation
/// \brief Memory related classes and functions.