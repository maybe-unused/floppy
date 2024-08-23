#pragma once

#include <cstdio>
#include <string>
#include <string_view>
#include <iostream>
#include <format>
#include <stdexcept>
#include "version_definition.h"

namespace fl::inline v2
{
  /**
   * @brief Version number structure.
   * @details Simple class representing an immutable semantic version (<i>major.minor.patch</i>).
   * @ingroup helpers
   * @see meta::library_metadata
   */
  class version
  {
   public:
    /**
     * @brief Creates a new version object with zero-initialized values.
     */
    constexpr version() noexcept
      : major_(0U)
      , minor_(0U)
      , patch_(0U)
    {}

    /**
     * @brief Creates a new version object from major, minor and patch values.
     * @param major Version major number.
     * @param minor Version minor number.
     * @param patch Version patch number.
     */
    constexpr version(unsigned major, unsigned minor, unsigned patch) noexcept
      : major_(major)
      , minor_(minor)
      , patch_(patch)
    {}

    /**
     * @brief Creates a new version object from a string representation.
     * @param version String representation of the version. Must be valid according to semantic version format <i>major.minor.patch</i>.
     * @throws std::invalid_argument if the string does not represent a valid version.
     */
    explicit version(std::string_view version) {
      if(std::sscanf(
        version.data(),
        "%u.%u.%u",
        &this->major_,
        &this->minor_,
        &this->patch_
      ) != 3)
        throw std::invalid_argument("Invalid version string");
    }

    /**
     * @brief Version major number.
     */
    [[nodiscard]] constexpr unsigned major() const noexcept { return this->major_; }

    /**
     * @brief Version minor number.
     */
    [[nodiscard]] constexpr unsigned minor() const noexcept { return this->minor_; }

    /**
     * @brief Version patch number.
     */
    [[nodiscard]] constexpr unsigned patch() const noexcept { return this->patch_; }

    /**
     * @brief Returns a string representation of the version.
     * @return Owning string object.
     */
    [[nodiscard]] std::string to_string() const {
      return std::format("{}.{}.{}", this->major_, this->minor_, this->patch_);
    }

    /**
     * @brief Returns a wide string representation of the version.
     * @return Owning wide string object.
     */
    [[nodiscard]] std::wstring to_wstring() const {
      return std::format(L"{}.{}.{}", this->major_, this->minor_, this->patch_);
    }

    /**
     * @brief Compares two versions.
     */
    friend auto operator<=>(version const&, version const&) = default;

    /**
     * @brief Increments the patch number by one.
     */
    version& operator++() {
      ++this->patch_;
      return *this;
    }

    /**
     * @brief Increments the patch number by one and returns the old value.
     */
    version operator++(int) {
      version tmp = *this;
      ++*this;
      return tmp;
    }

    /**
     * @brief Decrements the patch number by one.
     */
    version& operator--() {
      --this->patch_;
      return *this;
    }

    /**
     * @brief Decrements the patch number by one and returns the old value.
     */
    version operator--(int) {
      version tmp = *this;
      --*this;
      return tmp;
    }

    /**
     * @brief Adds two versions.
     */
    friend version operator+(version const& v1, version const& v2) {
      return version(v1.major_ + v2.major_, v1.minor_ + v2.minor_, v1.patch_ + v2.patch_);
    }

    /**
     * @brief Subtracts two versions.
     * @warning Version subtraction can result in overflow of unsigned integer type. Use with care.
     */
    friend version operator-(version const& v1, version const& v2) {
      return version(v1.major_ - v2.major_, v1.minor_ - v2.minor_, v1.patch_ - v2.patch_);
    }

    /**
     * @brief Prints the version string representation to an standard output stream.
     * @param os Standard output stream
     * @param v Version
     * @return Standard output stream
     */
    friend std::ostream& operator<<(std::ostream& os, version const& v) {
      return os << v.to_string();
    }

    /**
     * @brief Prints the version wide string representation to an standard output stream.
     * @param os Standard output stream
     * @param v Version
     * @return Standard output stream
     */
    friend std::wostream& operator<<(std::wostream& os, version const& v) {
      return os << v.to_wstring();
    }

   private:
    unsigned major_;
    unsigned minor_;
    unsigned patch_;
  };

  /**
   * @brief Library meta information.
   */
  namespace meta
  {
    /**
     * @brief Immutable constant expression structure, holding project meta-information.
     * @see version
     */
    class [[maybe_unused]] project_meta
    {
     public:
      /**
       * @brief Creates project meta-information object at compile time.
       * @param version Project version.
       * @param name Project name.
       * @param domain Project domain.
       * @param organization Project organization.
       */
      consteval project_meta(
        class version version,
        std::string_view name,
        std::string_view domain,
        std::string_view organization
      ) noexcept
        : version_(version)
        , name_(name)
        , domain_(domain)
        , organization_(organization)
      {}

      /**
       * @brief Version of the project.
       */
      [[nodiscard]] constexpr class version version() const noexcept { return this->version_; }

      /**
       * @brief Name of the project.
       */
      [[nodiscard]] constexpr std::string_view name() const noexcept { return this->name_; }

      /**
       * @brief Project domain.
       */
      [[nodiscard]] constexpr std::string_view domain() const noexcept { return this->domain_; }

      /**
       * @brief Project organization.
       */
      [[nodiscard]] constexpr std::string_view organization() const noexcept { return this->organization_; }

     private:
      class version version_;
      std::string_view name_;
      std::string_view domain_;
      std::string_view organization_;
    };

    /**
     * @brief Constant expression, containing library meta-information.
     * @details This constant expression is evaluated at compile time.
     * Example content of this variable:
     * @code
     * version:
     *   major: 2
     *   minor: 0
     *   patch: 0
     * name: 'floppy'
     * domain: 'github.com/whs31/floppy'
     * organization: 'maybe-unused'
     * @endcode
     * @see project_meta, version
     */
    [[maybe_unused]] constexpr inline auto library_metadata = project_meta(
      version(
        FLOPPY_PROJECT_VERSION_MAJOR,
        FLOPPY_PROJECT_VERSION_MINOR,
        FLOPPY_PROJECT_VERSION_PATCH
      ),
      "floppy",
      "github.com/whs31/floppy",
      "maybe-unused"
    );
  } // namespace meta
} // namespace fl::inline v2

/**
 * @brief Specialization of std::formatter for the version class.
 */
template <>
struct [[maybe_unused]] std::formatter<fl::version, char>
{
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(fl::version const& v, FormatContext& ctx) {
    return std::format_to(ctx.out(), "{}", v.to_string());
  }
};

/**
 * @brief Specialization of std::formatter for the version class (wide version).
 */
template <>
struct [[maybe_unused]] std::formatter<fl::version, wchar_t>
{
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(fl::version const& v, FormatContext& ctx) {
    return std::format_to(ctx.out(), L"{}", v.to_wstring());
  }
};

static_assert(fl::meta::library_metadata.version().major() == FLOPPY_PROJECT_VERSION_MAJOR);
static_assert(fl::meta::library_metadata.version().minor() == FLOPPY_PROJECT_VERSION_MINOR);
static_assert(fl::meta::library_metadata.version().patch() == FLOPPY_PROJECT_VERSION_PATCH);