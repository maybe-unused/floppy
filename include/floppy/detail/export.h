#pragma once

#include <string>
#include <string_view>

#if defined(_WIN32)
# if defined(FLOPPY_LIBRARY)
#   define FLOPPY_EXPORT __declspec(dllexport)
# elif defined(FLOPPY_STATIC_LIBRARY)
#   define FLOPPY_EXPORT
# else
#   define FLOPPY_EXPORT __declspec(dllimport)
# endif
#else
# define FLOPPY_EXPORT
#endif

#define _stringify$(x) #x
#define stringify$(x) _stringify$(x)

/// \brief Main namespace for the library. See \ref fl for shorter alias.
namespace floppy {
  /// \brief Metadata definitions, such as library version or name.
  namespace meta {
    /// \brief Project version constexpr structure. Equals to current cmake/conan version.
    struct [[maybe_unused]] project_version
    {
      int major;  ///< Major version.
      int minor;  ///< Minor version.
      int patch;  ///< Patch version.

      /// \brief Creates version object at compile-time.
      /// \see project_name
      constexpr inline project_version()
        : major(CMAKE_PROJECT_VERSION_MAJOR)
        , minor(CMAKE_PROJECT_VERSION_MINOR)
        , patch(CMAKE_PROJECT_VERSION_PATCH)
      {}

      /// \brief Returns constant expression version string in <tt>XX.YY.ZZ</tt> format.
      [[nodiscard]] static constexpr inline auto as_str() -> std::string_view {
        return {
          stringify$(CMAKE_PROJECT_VERSION_MAJOR) "."
          stringify$(CMAKE_PROJECT_VERSION_MINOR) "."
          stringify$(CMAKE_PROJECT_VERSION_PATCH)
        };
      }
    };

    static_assert(project_version::as_str() == std::string_view(
        stringify$(CMAKE_PROJECT_VERSION_MAJOR) "."
        stringify$(CMAKE_PROJECT_VERSION_MINOR) "."
        stringify$(CMAKE_PROJECT_VERSION_PATCH)
      ), "versions aren't the same"
    );

    /// \brief Project name constant-expression string. Always equals to \c floppy.
    /// \see project_version
    auto constexpr project_name = std::string_view(stringify$(CMAKE_TARGET_NAME));
  }
};

#undef _stringify$
#undef stringify$

/// \brief Alias for the main namespace \ref floppy.
namespace fl = floppy;