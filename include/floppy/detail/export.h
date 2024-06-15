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

/// \brief Main namespace for the library.
/// \note Use <tt>fl</tt> instead of <tt>floppy</tt> if you want shorter name.
namespace floppy { // NOLINT(*-concat-nested-namespaces)
  /// \brief Metadata definitions, such as library version or name.
  namespace meta {
    /// \brief Project version constexpr structure. Equals to current cmake/conan version.
    class [[maybe_unused]] project_meta
    {
     private:
      int major{};                   ///< Major version.
      int minor{};                   ///< Minor version.
      int patch{};                   ///< Patch version.
      std::string_view project_name; ///< Project name.

     public:
      /// \brief Creates version object at compile-time.
      /// \see project_name
      consteval project_meta()
        : major(CMAKE_PROJECT_VERSION_MAJOR)
        , minor(CMAKE_PROJECT_VERSION_MINOR)
        , patch(CMAKE_PROJECT_VERSION_PATCH)
        , project_name(stringify$(CMAKE_TARGET_NAME))
      {}

      /// \brief Project name constant-expression string. Always equals to <b>floppy</b>.
      [[nodiscard]] constexpr auto name() const noexcept -> std::string_view { return this->project_name; }

      /// \brief Returns constant expression version number major as an integer.
      /// \see version_minor
      /// \see version_patch
      /// \see version_string
      [[nodiscard]] constexpr auto version_major() const noexcept -> int { return this->major; }

      /// \brief Returns constant expression version number minor as an integer.
      /// \see version_major
      /// \see version_patch
      /// \see version_string
      [[nodiscard]] constexpr auto version_minor() const noexcept -> int { return this->minor; }

      /// \brief Returns constant expression version number patch as an integer.
      /// \see version_major
      /// \see version_minor
      /// \see version_string
      [[nodiscard]] constexpr auto version_patch() const noexcept -> int { return this->patch; }

      /// \brief Returns constant expression version string in <tt>XX.YY.ZZ</tt> format.
      /// \see version_major
      /// \see version_minor
      /// \see version_patch
      [[nodiscard]] static constexpr auto version_string() noexcept -> std::string_view {
        return {
          stringify$(CMAKE_PROJECT_VERSION_MAJOR) "."
          stringify$(CMAKE_PROJECT_VERSION_MINOR) "."
          stringify$(CMAKE_PROJECT_VERSION_PATCH)
        };
      }
    };

    /// \brief Project metadata object, available at compile-time.
    [[maybe_unused]] constexpr inline auto project_info = project_meta{};

    static_assert(project_meta::version_string() == std::string_view(
        stringify$(CMAKE_PROJECT_VERSION_MAJOR) "."
        stringify$(CMAKE_PROJECT_VERSION_MINOR) "."
        stringify$(CMAKE_PROJECT_VERSION_PATCH)
      ), "versions aren't the same"
    );

    static_assert(project_info.version_major() == CMAKE_PROJECT_VERSION_MAJOR, "major version isn't the same");
    static_assert(project_info.version_minor() == CMAKE_PROJECT_VERSION_MINOR, "minor version isn't the same");
    static_assert(project_info.version_patch() == CMAKE_PROJECT_VERSION_PATCH, "patch version isn't the same");
    static_assert(project_info.name() == std::string_view(stringify$(CMAKE_TARGET_NAME)), "project name isn't the same");
  } // namespace meta
} // namespace floppy

#undef _stringify$
#undef stringify$

/// \brief Alias for the main namespace \ref floppy.
namespace fl = floppy; // NOLINT(*-unused-alias-decls)