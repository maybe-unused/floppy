#pragma once

// NOLINTBEGIN(*-macro-to-enum, *-macro-usage)
#ifndef FL_VERSION_STRING
/**
 * @ingroup macros
 * @brief Major version number of this library build.
 * @note Consider using fl::meta::library_metadata class to access this value.
 */
# define FL_VERSION_MAJOR 2
# define FL_VERSION_MINOR 0
# define FL_VERSION_PATCH 0

# define FL_STRINGIFY2(x) #x
# define FL_STRINGIFY(x) FL_STRINGIFY2(x)

# define FL_VERSION_STRING FL_STRINGIFY(FL_VERSION_MAJOR) "." FL_STRINGIFY(FL_VERSION_MINOR) "." FL_STRINGIFY(FL_VERSION_PATCH)

# undef FL_STRINGIFY2
# undef FL_STRINGIFY
#endif // FL_VERSION_STRING
// NOLINTEND(*-macro-to-enum, *-macro-usage)