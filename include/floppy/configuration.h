#pragma once

#include <filesystem>
#include <utility>
#include <fstream>
#include <floppy/floppy.h>
#include <floppy/serialization.h>

namespace floppy
{
  /// \brief Determines the policy used for autosaving configuration files.
  enum class saving_policy : u8
  {
    autosave,  ///< Saves configuration files automatically on closing.
    explicit_  ///< Saves configuration files only explicitly.
  };

  template <auto F, typename T>
  requires serialization::serializable_and_deserializable<F, T, char>
  class configuration_file
  {
   public:
    explicit configuration_file(std::filesystem::path path, saving_policy policy);
    explicit configuration_file(std::string_view filename, std::filesystem::path const& folder, saving_policy policy);
    configuration_file(configuration_file const&) = default;
    configuration_file(configuration_file&&) = default;
    virtual ~configuration_file() noexcept;

    [[nodiscard]] auto valid() const -> bool;
    [[nodiscard]] auto path() const -> std::filesystem::path const&;
    [[nodiscard]] auto saving_policy() const -> saving_policy;
    [[nodiscard]] auto values() const -> T const&;
    [[nodiscard]] auto values_mut() -> T&;
    [[nodiscard]] auto default_values() const -> T const&;
    [[nodiscard]] auto default_values_mut() -> T&;

    auto load() noexcept(false) -> void;
    auto save() const noexcept(false) -> void;
    auto revert_to_default() noexcept(false) -> void;

    [[nodiscard]] explicit operator bool() const noexcept;
    [[nodiscard]] auto operator()() const -> T const&;
    [[nodiscard]] auto operator()() -> T&;
    auto operator=(configuration_file const&) -> configuration_file& = default;
    auto operator=(configuration_file&&) -> configuration_file& = default;

   protected:
    auto read_from_file() const noexcept(false) -> std::string;
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