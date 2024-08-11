#pragma once

#include <string>
#include <filesystem>
#include <floppy/detail/types.h>
#include <floppy/detail/export.h>

namespace floppy
{
  /// \brief Class for getting location of system directories for a specific application.
  /// \headerfile floppy/directories.h 
  /// \ingroup foundation
  /// \details ProjectDirs computes the location of cache, config or data directories for a specific application,
  /// which are derived from the standard directories and the name of the project/organization.
  ///
  /// For example if user named <i>Alice</i>, the following code:
  /// \code {.cpp}
  /// using floppy::filesystem::application_dirs;
  /// auto dirs = application_dirs("com", "Foo Corp", "Bar App");
  /// std::cout << dirs.config_dir() << std::endl;
  /// \endcode
  /// will produce the following output:
  /// \code {.sh}
  /// Linux:   '/home/alice/.config/barapp'
  /// Windows: 'C:\Users\Alice\AppData\Roaming\Foo Corp\Bar App'
  /// MacOS:   '/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App'
  /// \endcode
  class application_dirs
  {
   public:
      /// \brief Supported directory types.
      enum class dir : u8
      {
        cache,        ///< Cache directory
        config,       ///< Config directory
        config_local, ///< Local config directory
        data,         ///< Data directory
        data_local,   ///< Local data directory
        preferences,  ///< Preferences directory
        runtime,      ///< Runtime directory. Can be unavailable on some platforms
        state         ///< State directory. Can be unavailable on some platforms
      };

     /// \brief Creates an application_dirs class from values describing the project.
     /// \note Constuctor can fail if no valid home directory could be retrieved from the operating system.
     /// \note Directories will be created if they do not exist.
     /// \param qualifier The reverse domain name notation of the application, excluding
     /// the organization or application name itself.
     ///
     /// Example values:
     /// <ul>
     /// <li><tt>"com.example"</tt></li>
     /// <li><tt>"org"</tt></li>
     /// <li><tt>"uk.co"</tt></li>
     /// <li><tt>"io"</tt></li>
     /// <li><tt>""</tt></li>
     /// </ul>
     /// \param organization The name of the organization that develops this application, or for which the application is developed.
     ///
     /// Example values:
     /// <ul>
     /// <li><tt>"Foo Corp"</tt></li>
     /// <li><tt>"Bar Ltd"</tt></li>
     /// <li><tt>"Example Inc"</tt></li>
     /// </ul>
     /// \param application The name of the application itself.
     ///
     /// Example values:
     /// <ul>
     /// <li><tt>"Bar App"</tt></li>
     /// <li><tt>"Foo App"</tt></li>
     /// </ul>
     /// \throws std::runtime_error if no valid home directory could be retrieved from the operating system.
     FLOPPY_EXPORT explicit application_dirs(
       std::string_view qualifier,
       std::string_view organization,
       std::string_view application
     ) noexcept(false);

     /// \brief Creates an application_dirs class from values describing the project.
     /// \note Constuctor can fail if no valid home directory could be retrieved from the operating system.
     /// \note Directories will be created if they do not exist.
     /// \param meta The project meta data.
     /// \throws std::runtime_error if no valid home directory could be retrieved from the operating system.
     /// \see floppy::meta::project_meta
     FLOPPY_EXPORT explicit application_dirs(meta::project_meta const& meta) noexcept(false);

     ~application_dirs() = default;
     application_dirs(application_dirs const&) = default;
     auto operator=(application_dirs const&) -> application_dirs& = default;
     application_dirs(application_dirs&&) = default;
     auto operator=(application_dirs&&) -> application_dirs& = default;

     /// \brief Returns the path to the directory.
     /// \param directory_type The directory type.
     /// \return The path to the directory.
     /// \throws std::system_error If the directory does not exist on this platform.
     /// \note Use explicit functions such as \ref config_dir() to avoid this exception.
     /// \see operator[]
     [[nodiscard]] FLOPPY_EXPORT auto get(dir directory_type) const noexcept(false) -> std::filesystem::path;

     /// \brief Creates the directories if they do not exist.
     FLOPPY_EXPORT auto create() const -> void;

     /// \brief Removes the directories from the filesystem.
     FLOPPY_EXPORT auto remove() const -> void;

     /// \brief Returns the project path fragment used to compute the project's cache/config/data directories.
     /// \details The value is derived from the ProjectDirs::from call and is platform-dependent.
     /// \return The project path.
     [[nodiscard]] FLOPPY_EXPORT auto project_path() const -> std::filesystem::path const&;

     /// \brief Returns the path to the project's cache directory.
     /// \details Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>$XDG_CACHE_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.cache/<tt>project_path</tt>, for example: <tt>/home/alice/.cache/barapp</tt></li>
     /// <li><b>MacOS</b>: <tt>$HOME/Library/Caches</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Caches/com.Foo-Corp.Bar-App</tt></li>
     /// <li><b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\cache, for example: <tt>C:\\Users\\Alice\\AppData\\Local\\Foo Corp\\Bar App\\cache</tt></li>
     /// </ul>
     /// \return The path to the project's cache directory.
     [[nodiscard]] FLOPPY_EXPORT auto cache_dir() const -> std::filesystem::path const&;

     /// \brief Returns the path to the project's config directory.
     /// \details Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt></li>
     /// <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li>
     /// <li><b>Windows</b>: <tt>%APPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt></li>
     /// </ul>
     /// \return The path to the project's config directory.
     [[nodiscard]] FLOPPY_EXPORT auto config_dir() const -> std::filesystem::path const&;

     /// \brief Returns the path to the project's config_local directory.
     /// \details Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt></li>
     /// <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li>
     /// <li><b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt></li>
     /// </ul>
     /// \return The path to the project's config_local directory.
     [[nodiscard]] FLOPPY_EXPORT auto config_local_dir() const -> std::filesystem::path const&;

     /// \brief Returns the path to the project's data directory.
     /// \details Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt></li>
     /// <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li>
     /// <li><b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\data</tt></li>
     /// </ul>
     /// \return The path to the project's data directory.
     [[nodiscard]] FLOPPY_EXPORT auto data_dir() const -> std::filesystem::path const&;

     /// \brief Returns the path to the project's data_local directory.
     /// \details Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>$XDG_DATA_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/share/<tt>project_path</tt>, for example: <tt>/home/alice/.local/share/barapp</tt></li>
     /// <li><b>MacOS</b>: <tt>$HOME/Library/Application Support</tt>/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Application Support/com.Foo-Corp.Bar-App</tt></li>
     /// <li><b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\data, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\data</tt></li>
     /// </ul>
     /// \return The path to the project's data_local directory.
     [[nodiscard]] FLOPPY_EXPORT auto data_local_dir() const -> std::filesystem::path const&;

     /// \brief Returns the path to the project's preference directory.
     /// \details Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>$XDG_CONFIG_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.config/<tt>project_path</tt>, for example: <tt>/home/alice/.config/barapp</tt></li>
     /// <li><b>MacOS</b>: <tt>$HOME</tt>/Library/Preferences/<tt>project_path</tt>, for example: <tt>/Users/Alice/Library/Preferences/com.Foo-Corp.Bar-App</tt></li>
     /// <li><b>Windows</b>: <tt>%LOCALAPPDATA%</tt>\\<tt>project_path</tt>\\config, for example: <tt>C:\\Users\\Alice\\AppData\\Roaming\\Foo Corp\\Bar App\\config</tt></li>
     /// </ul>
     /// \return
     [[nodiscard]] FLOPPY_EXPORT auto preference_dir() const -> std::filesystem::path const&;

     /// \brief Returns the path to the project's runtime directory.
     /// \details
     /// The runtime directory contains transient, non-essential data (like sockets or named pipes) that
     /// is expected to be cleared when the user's session ends.<br>
     /// Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>XDG_RUNTIME_DIR</tt>/<tt>project_path</tt>, for example: <tt>/run/user/1001/barapp</tt></li>
     /// <li><b>MacOS</b>: -</li>
     /// <li><b>Windows</b>: -</li>
     /// </ul>
     /// \invariant Only for Linux.
     /// \return The path to the project's runtime directory or <tt>none</tt> if it is not available.
     [[nodiscard]] FLOPPY_EXPORT auto runtime_dir() const -> option<std::filesystem::path> const&;

     /// \brief Returns the path to the project's state directory.
     /// \details
     /// The state directory contains data that should be retained between sessions (unlike the runtime directory),
     /// but may not be important/portable enough to be synchronized across machines (unlike the config/preferences/data directories).<br>
     /// Values on different platforms:
     /// <ul>
     /// <li><b>Linux</b>: <tt>$XDG_STATE_HOME</tt>/<tt>project_path</tt> or <tt>$HOME</tt>/.local/state/<tt>project_path</tt>, for example: <tt>/home/alice/.local/state/barapp</tt></li>
     /// <li><b>MacOS</b>: -</li>
     /// <li><b>Windows</b>: -</li>
     /// </ul>
     /// \invariant Only for Linux.
     /// \return The path to the project's state directory or <tt>none</tt> if it is not available.
     [[nodiscard]] FLOPPY_EXPORT auto state_dir() const -> option<std::filesystem::path> const&;

     /// \brief Shortcut for \ref get() function.
     /// \param directory_type The directory type.
     /// \return The path to the directory.
     /// \throws std::system_error If the directory does not exist on this platform.
     /// \note Use explicit functions such as \ref config_dir() to avoid this exception.
     /// \see get
     [[nodiscard]] FLOPPY_EXPORT auto operator[](dir directory_type) const noexcept(false) -> std::filesystem::path;

    private:
     std::filesystem::path project_path_;
     std::filesystem::path cache_dir_;
     std::filesystem::path config_dir_;
     std::filesystem::path config_local_dir_;
     std::filesystem::path data_dir_;
     std::filesystem::path data_local_dir_;
     std::filesystem::path preference_dir_;
     option<std::filesystem::path> runtime_dir_;
     option<std::filesystem::path> state_dir_;
  };
} // namespace floppy::filesystem
