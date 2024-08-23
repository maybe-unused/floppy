#pragma once

#if defined(_WIN32)
# if defined(FLOPPY_LIBRARY)
#   define FL_API __declspec(dllexport)
# elif defined(FLOPPY_STATIC_LIBRARY)
#   define FL_API
# else // defined(FLOPPY_LIBRARY)
#   define FL_API __declspec(dllimport)
# endif // defined(FLOPPY_LIBRARY)
#else // defined(_WIN32)
# define FL_API
#endif // defined(_WIN32)

/**
 * @brief Main namespace for the <b>floppy</b> library.
 */
namespace fl { // NOLINT(*-concat-nested-namespaces)
  /**
   * @brief Version switch namespace for the <b>floppy</b> library.
   * @note This namespace is not part of the public API.
   */
  inline namespace v2 {} // namespace v2
} // namespace fl