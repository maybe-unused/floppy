#pragma once

// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#if defined(_WIN32)
# if defined(FLOPPY_LIBRARY)
#   define ___fl_api___ __declspec(dllexport)
# elif defined(FLOPPY_STATIC_LIBRARY)
#   define ___fl_api___
# else // defined(FLOPPY_LIBRARY)
#   define ___fl_api___ __declspec(dllimport)
# endif // defined(FLOPPY_LIBRARY)
#else // defined(_WIN32)
# define ___fl_api___
#endif // defined(_WIN32)
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)

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