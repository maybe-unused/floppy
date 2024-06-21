#pragma once

#include <filesystem>
#include <utility>
#include <fstream>
#include <floppy/floppy.h>
#include <floppy/serialization.h>
#include <floppy/logging.h>

namespace floppy
{
  /// \brief Determines the policy used for autosaving configuration files.
  enum class saving_policy : u8
  {
    autosave,  ///< Saves configuration files automatically on closing.
    explicit_  ///< Saves configuration files only explicitly.
  };

  /// \brief Configuration file that can be loaded and saved.
  /// \details This class is used to store configuration values in a configuration file in given format.
  /// Accepts any type that implements \ref floppy::serialization::serializable_and_deserializable with character type char
  /// and given format.
  ///
  /// Example usage:
  /// \code {.cpp}
  /// struct DummyConfiguration
  /// {
  ///   u32 test = 0;
  ///   struct IpAddress
  ///   {
  ///     string ip;
  ///     u16 port;
  ///     struct SockMode
  ///     {
  ///       bool tcp = true;
  ///       bool udp = false;
  ///     } sock_mode{};
  ///   } ip_address = { .ip = "127.0.0.1", .port = 25565 };
  /// };
  /// template <>
  /// auto fl::serialization::serialize<fl::serialization::format::toml>(DummyConfiguration const& value) -> std::basic_string<char>
  /// { /* implementation */ }
  ///
  /// template <>
  /// auto fl::serialization::deserialize<fl::serialization::format::toml>(std::basic_string<char> const& value) -> DummyConfiguration
  /// { /* implementation */ }
  ///
  /// auto main() -> int
  /// {
  ///   using fl::serialization::format;
  ///   using enum fl::saving_policy;
  ///
  ///   auto config = fl::configuration_file<format::toml, DummyConfiguration>(
  ///     "test.toml",
  ///     fs::current_path() / "test-cfg",
  ///     explicit_
  ///   );
  ///   if(not config)
  ///     return 1;
  ///   std::cout << config().ip_address.ip << std::endl;
  ///   try {
  ///     config().ip_address.sock_mode.udp = true;
  ///     config.save();
  ///   } catch(...) {
  ///     return 1;
  ///   }
  ///   return 0;
  /// }
  /// \endcode
  /// \tparam F Format type. Must be value of enum \ref floppy::serialization::format.
  /// \tparam T Type to serialize and deserialize. Must implement \ref floppy::serialization::serializable_and_deserializable with character type char.
  /// \see floppy::serialization::format
  /// \see floppy::serialization::serializable_and_deserializable
  /// \see floppy::saving_policy
  template <auto F, typename T>
  requires serialization::serializable_and_deserializable<F, T, char>
  class configuration_file
  {
   public:
    /// \brief Creates or loads configuration file from given path with saving policy.
    /// \note This constructor will not throw any error, but will return an invalid configuration file.
    /// Don't forget to check if configuration file is valid before using it.
    /// \param path Path to configuration file.
    /// \param policy Saving policy.
    /// \see saving_policy
    /// \see valid
    explicit configuration_file(std::filesystem::path path, saving_policy policy);

    /// \brief Creates or loads configuration file from given path and folder with saving policy.
    /// \note This constructor will not throw any error, but will return an invalid configuration file.
    /// Don't forget to check if configuration file is valid before using it.
    /// \param filename Filename of configuration file.
    /// \param folder Folder where configuration file is located.
    /// \param policy Saving policy.
    /// \see saving_policy
    /// \see valid
    explicit configuration_file(std::string_view filename, std::filesystem::path const& folder, saving_policy policy);
    configuration_file(configuration_file const&) = default;
    configuration_file(configuration_file&&) = default;

    /// \brief Closes configuration file.
    /// \details If saving policy is set to \ref saving_policy::autosave, file will be saved automatically on closing.
    virtual ~configuration_file() noexcept;

    /// \brief Returns whether configuration file is valid or not.
    [[nodiscard]] auto valid() const -> bool;

    /// \brief Returns path to configuration file.
    /// \return Path to configuration file.
    [[nodiscard]] auto path() const -> std::filesystem::path const&;

    /// \brief Returns saving policy.
    /// \return Saving policy.
    /// \see saving_policy
    [[nodiscard]] auto saving_policy() const -> saving_policy;

    /// \brief Returns constant reference to current values of configuration file.
    /// \return Values of configuration file.
    [[nodiscard]] auto values() const -> T const&;

    /// \brief Returns mutable reference to current values of configuration file.
    /// \return Values of configuration file.
    [[nodiscard]] auto values_mut() -> T&;

    /// \brief Returns default values of configuration file.
    /// \return Default values of configuration file.
    [[nodiscard]] auto default_values() const -> T const&;

    /// \brief Returns mutable reference to default values of configuration file.
    /// \return Default values of configuration file.
    [[nodiscard]] auto default_values_mut() -> T&;

    /// \brief Loads configuration file from file.
    /// \throws std::exception if loading fails.
    auto load() noexcept(false) -> void;

    /// \brief Saves configuration file to file.
    /// \throws std::exception if saving fails.
    auto save() const noexcept(false) -> void;

    /// \brief Reverts configuration file to default values.
    /// \throws std::exception if reverting fails.
    auto revert_to_default() noexcept(false) -> void;

    /// \brief Returns whether configuration file is valid or not.
    /// \return Whether configuration file is valid or not.
    [[nodiscard]] explicit operator bool() const noexcept;

    /// \brief Returns constant reference to values of configuration file.
    /// \return Values of configuration file.
    [[nodiscard]] auto operator()() const -> T const&;

    /// \brief Returns mutable reference to values of configuration file.
    /// \return Values of configuration file.
    [[nodiscard]] auto operator()() -> T&;

    /// \brief Default copy assignment.
    auto operator=(configuration_file const&) -> configuration_file& = default;

    /// \brief Default move assignment.
    auto operator=(configuration_file&&) -> configuration_file& = default;

   protected:
    /// \brief Reads content of configuration file from file.
    [[nodiscard]] auto read_from_file() const noexcept(false) -> std::string;

    /// \brief Writes content of configuration file to a file.
    auto write_to_file(std::string_view content) const noexcept(false) -> void;

   private:
    T values_;
    T default_values_;
    std::filesystem::path path_;
    enum saving_policy saving_policy_;
    bool valid_;
  };
} // namespace floppy

#include <floppy/inl/configuration.inl>