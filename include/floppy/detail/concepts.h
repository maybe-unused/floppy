#pragma once

#include <concepts>
#include <floppy/detail/type_traits.h>
#include <floppy/detail/types.h>

/// \brief Concepts namespace.
namespace floppy::concepts
{
  /// \brief Satisfied if T and any of given U are the same type.
  /// \tparam T Type to check
  /// \tparam U Types to check against
  template <typename T, typename... U>
  concept any_of = (std::same_as<T, U> or ...);

  /// \brief Number concept.
  /// \details Number types are integral or floating point.
  /// \note This concept will be true for references or pointers to numbers as well.
  /// \tparam T Type to check
  template <typename T>
  concept num = std::integral<plain_type_t<T>> or std::floating_point<plain_type_t<T>>;

  /// \brief Array concept.
  /// \tparam T Type to check
  /// \see array
  template <typename T>
  concept array = std::is_array_v<T>;

  /// \brief Object concept.
  /// \tparam T Type to check
  /// \details Object is an any possibly cv-qualified type other than function, reference or void types.
  template <typename T>
  concept object = std::is_object_v<T>;

  /// \brief Pointer concept.
  /// \details Will be true for any type that is a pointer or reference. Note that this concept will
  /// not be true for function pointers, <tt>std::nullptr_t</tt> or smart pointers.
  /// \tparam T Type to check
  template <typename T>
  concept ptr = std::is_pointer_v<T>;

  /// \brief Reference concept.
  /// \tparam T Type to check
  /// \see ref
  template <typename T>
  concept ref = std::is_reference_v<T>;

  /// \brief Enum concept.
  /// \note This concept will be true for references or pointers to enums as well.
  /// \tparam T Type to check
  template <typename T>
  concept enum_ = std::is_enum_v<plain_type_t<T>>;

  /// \brief Formattable concept.
  /// \details Will be true for any type that has method <tt>to_string()</tt> which return a string or convertible to it.
  /// \tparam T Type to check
  /// \tparam C Character type
  template <typename T, typename C>
  concept formattable = requires(T t) {
    { t.to_string() } -> std::convertible_to<std::basic_string<C>>;
  };
} // namespace floppy::concepts