#pragma once

// NOLINTBEGIN(*-macro-to-enum, *-macro-usage)
#ifndef FL_VERSION_STRING
/**
 * @ingroup macros
 * @brief Major version number of this library build.
 * @note Consider using fl::meta::library_metadata class to access this value.
 */
# define FL_VERSION_MAJOR 2

/**
 * @ingroup macros
 * @brief Minor version number of this library build.
 * @note Consider using fl::meta::library_metadata class to access this value.
 */
# define FL_VERSION_MINOR 0

/**
 * @ingroup macros
 * @brief Patch version number of this library build.
 * @note Consider using fl::meta::library_metadata class to access this value.
 */
# define FL_VERSION_PATCH 0

/**
 * @internal
 */
# define FL_STRINGIFY2(x) #x

/**
 * @internal
 */
# define FL_STRINGIFY(x) FL_STRINGIFY2(x)

/**
 * @ingroup macros
 * @brief String representation of this library version number.
 * @note Consider using fl::meta::library_metadata class to access this value.
 * @see FL_VERSION_MAJOR, FL_VERSION_MINOR, FL_VERSION_PATCH
 */
# define FL_VERSION_STRING FL_STRINGIFY(FL_VERSION_MAJOR) "." FL_STRINGIFY(FL_VERSION_MINOR) "." FL_STRINGIFY(FL_VERSION_PATCH)

# undef FL_STRINGIFY2
# undef FL_STRINGIFY
#endif // FL_VERSION_STRING
// NOLINTEND(*-macro-to-enum, *-macro-usage)