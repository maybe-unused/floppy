#include <floppy/directories.h>

#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <floppy/detail/predefs.h>
#include <floppy/detail/platform.h>

#if defined(_WIN32)
# include <windows.h>
# if defined(_MSC_VER)
#   if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#     define _AMD64_
#   elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(_M_IX86)
#     define _X86_
#   elif defined(__arm__) || defined(_M_ARM) || defined(_M_ARMT)
#     define _ARM_
#   endif // __amd64__
# endif // _MSC_VER
# include <Shlobj.h>
#endif // _WIN32

using std::string;
using std::string_view;
using std::vector;
namespace fs = std::filesystem;

namespace
{
#if defined(_WIN32)
  [[nodiscard]] auto known_folder_path(::KNOWNFOLDERID id) -> fs::path {
    auto* buf = ::PWSTR();
    auto const result = ::SHGetKnownFolderPath(id, 0, nullptr, &buf);
    if(result != S_OK) {
      ::CoTaskMemFree(buf);
      throw std::system_error(std::make_error_code(static_cast<std::errc>(result)));
    }
    auto const path = fs::path(buf);
    ::CoTaskMemFree(buf);
    return path;
  }
#endif // _WIN32

  auto split(const string& input) -> vector<string> {
    auto buffer = std::istringstream(input);
    return {std::istream_iterator<string>(buffer), std::istream_iterator<string>()};
  }

  auto to_lower(const string_view name) -> string {
    auto str = string();
    str.reserve(name.length());
    for(auto const c : name)
      str.push_back(static_cast<char>(::tolower(c)));
    return str;
  }

  auto trim(const string_view name, const string_view repl) -> string {
    auto str = string();
    str.reserve(name.length());
    auto parts = split(string(name));
    auto current_part = parts.begin();
    auto const replace = not repl.empty();
    while(current_part != parts.end()) {
      auto const value = to_lower(*current_part);
      str.append(value);
      ++current_part;
      if(replace && current_part != parts.end())
        str.append(repl);
    }
    return str;
  }
} // namespace

namespace floppy
{
  application_dirs::application_dirs(meta::project_meta const& meta) noexcept(false)
    : application_dirs(meta.domain(), meta.organization(), meta.name())
  {}

  application_dirs::application_dirs(
    string_view qualifier,
    string_view organization,
    string_view application
  ) noexcept(false)
  {
    if constexpr(current_platform.operating_system == platform::operating_system::linux_) {
      auto const path = fs::path(::trim(application, ""));
      auto* const home_c = std::getenv("HOME");
      if(not home_c)
        throw std::runtime_error("no valid home directory could be retrieved from the operating system");
      auto const home = fs::path(home_c);
      auto* const xdg_runtime_dir = std::getenv("XDG_RUNTIME_DIR");
      this->project_path_ = path;
      this->cache_dir_ = home / ".cache" / path;
      this->config_dir_ = home / ".config" / path;
      this->config_local_dir_ = this->config_dir_;
      this->data_dir_ = home / ".local" / "share" / path;
      this->data_local_dir_ = this->data_dir_;
      this->preference_dir_ = this->config_dir_;
      this->runtime_dir_ = xdg_runtime_dir ? option<fs::path>(fs::path(xdg_runtime_dir)) : none;
      this->state_dir_ = home / ".local" / "state" / path;
    } else if constexpr(current_platform.operating_system == platform::operating_system::windows) {
      auto const path = fs::path(organization) / application;
      #if defined(FLOPPY_OS_WINDOWS)
        auto const appdata_roaming = ::known_folder_path(FOLDERID_RoamingAppData);
        auto const appdata_local = ::known_folder_path(FOLDERID_LocalAppData);
        this->project_path_ = path;
        this->cache_dir_ = appdata_local / path / "cache";
        this->config_dir_ = appdata_roaming / path / "config";
        this->config_local_dir_ = appdata_local / path / "config";
        this->data_dir_ = appdata_roaming / path / "data";
        this->data_local_dir_ = appdata_local / path / "data";
        this->preference_dir_ = this->config_dir_;
        this->runtime_dir_ = none;
        this->state_dir_ = none;
      #endif
    } else if constexpr(current_platform.operating_system == platform::operating_system::macos) {
      auto replaced = [](string_view const str, char const what, char const with) {
        auto owned_str = string(str);
        std::replace(owned_str.begin(), owned_str.end(), what, with);
        return owned_str;
      };
      auto const org = replaced(organization, ' ', '-');
      auto const path = fmt::format("{}{}{}{}{}",
        qualifier,
        qualifier.empty() ? "" : ".",
        org,
        org.empty() ? "" : ".",
        application
      );
      auto* const home_c = std::getenv("HOME");
      if(not home_c)
        throw std::runtime_error("no valid home directory could be retrieved from the operating system");
      auto const home = fs::path(home_c);
      this->project_path_ = path;
      this->cache_dir_ = home / "Library" / "Caches" / path;
      this->config_dir_ = home / "Library" / "Application Support" / path;
      this->config_local_dir_ = this->config_dir_;
      this->data_dir_ = this->config_dir_;
      this->data_local_dir_ = this->config_dir_;
      this->preference_dir_ = this->config_dir_;
      this->runtime_dir_ = none;
      this->state_dir_ = none;
    } else {
      throw std::runtime_error("unsupported operating system");
    }
  }

  auto application_dirs::create() const -> void {
    for(auto i = 0_ZU; i <= static_cast<usize>(dir::preferences); ++i) {
      if(auto const dir = this->get(static_cast<enum dir>(i)); not fs::exists(dir))
        fs::create_directories(dir);
    }
    if(this->runtime_dir_ and not fs::exists(*this->runtime_dir_))
      fs::create_directory(*this->runtime_dir_);
    if(this->state_dir_ and not fs::exists(*this->state_dir_))
      fs::create_directory(*this->state_dir_);
  }

  auto application_dirs::remove() const -> void {
    for(auto i = 0_ZU; i <= static_cast<usize>(dir::preferences); ++i) {
      if(auto const dir = this->get(static_cast<enum dir>(i)); fs::exists(dir))
        fs::remove_all(dir);
    }
    if(this->runtime_dir_ and fs::exists(*this->runtime_dir_))
      fs::remove_all(*this->runtime_dir_);
    if(this->state_dir_ and fs::exists(*this->state_dir_))
      fs::remove_all(*this->state_dir_);
  }

  auto application_dirs::project_path() const -> fs::path const& { return this->project_path_; }
  auto application_dirs::cache_dir() const -> fs::path const& { return this->cache_dir_; }
  auto application_dirs::config_dir() const -> fs::path const& { return this->config_dir_; }
  auto application_dirs::config_local_dir() const -> fs::path const& { return this->config_local_dir_; }
  auto application_dirs::data_dir() const -> fs::path const& { return this->data_dir_; }
  auto application_dirs::data_local_dir() const -> fs::path const& { return this->data_local_dir_; }
  auto application_dirs::preference_dir() const -> fs::path const& { return this->preference_dir_; }
  auto application_dirs::runtime_dir() const -> option<fs::path> const& { return this->runtime_dir_; }
  auto application_dirs::state_dir() const -> option<fs::path> const& { return this->state_dir_; }

  auto application_dirs::get(application_dirs::dir directory_type) const noexcept(false) -> fs::path
  {
    switch(directory_type) {
      case dir::cache: return this->cache_dir_;
      case dir::config: return this->config_dir_;
      case dir::config_local: return this->config_local_dir_;
      case dir::data: return this->data_dir_;
      case dir::data_local: return this->data_local_dir_;
      case dir::preferences: return this->preference_dir_;
      case dir::runtime:
        if(this->runtime_dir_)
          return *this->runtime_dir_;
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
      case dir::state:
        if(this->state_dir_)
          return *this->state_dir_;
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
      default: throw std::system_error(std::make_error_code(std::errc::invalid_argument));
    }
  }

  auto application_dirs::operator[](application_dirs::dir directory_type) const noexcept(false) -> fs::path {
    return this->get(directory_type);
  }
} // namespace floppy