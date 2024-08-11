#pragma once

#include <vector>
#include <utility>
#include <string>
#include <string_view>
#include <fstream>
#include <algorithm>
#include <floppy/backtrace/demangle.h>
#include <floppy/backtrace/handle.h>

namespace floppy::stacktrace
{
  class source_file final
  {
   public:
    using lines_t = std::vector<std::pair<unsigned int, std::string>>;

    source_file() = default;
    explicit source_file(std::string_view path) {
      auto const& prefixes = get_paths_from_env_variable();
      for(auto const& prefix : prefixes) {
        auto new_path = prefix + '/' + std::string(path);
        this->_file.reset(new std::ifstream(new_path.c_str())); // NOLINT(*-owning-memory)
        if(this->is_open())
          break;
      }
      if(not this->_file or not this->is_open())
        this->_file.reset(new std::ifstream(path.data())); // NOLINT(*-owning-memory)
    }

    source_file(source_file const&) = delete;
    auto operator=(const source_file &) -> source_file& = delete;

    source_file(source_file&& from) noexcept
      : _file(nullptr) {
      this->swap(from);
    }

    auto operator=(source_file&& from) noexcept -> source_file& {
      this->swap(from);
      return *this;
    }

    ~source_file() = default;

    [[nodiscard]] auto is_open() const -> bool { return this->_file->is_open(); }

    [[nodiscard]] auto get_lines(unsigned line_start, unsigned line_count, lines_t &lines) -> lines_t& {
      _file->clear();
      _file->seekg(0);
      auto line = std::string();
      auto line_idx = static_cast<unsigned int>(0);

      for(line_idx = 1; line_idx < line_start; ++line_idx) {
        std::getline(*this->_file, line);
        if(not *this->_file)
          return lines;
      }

      bool started = false;
      for(; line_idx < line_start + line_count; ++line_idx) {
        std::getline(*this->_file, line);
        if(not *this->_file)
          return lines;
        if(not started) {
          if(std::find_if_not(line.begin(), line.end(), [&](char c){ return std::isspace(c); }) == line.end())
            continue;
          started = true;
        }
        lines.emplace_back(line_idx, line);
      }

      lines.erase(
        std::find_if_not(
          lines.rbegin(),
          lines.rend(),
          [](lines_t::value_type const& p){
            return std::find_if_not(p.second.begin(), p.second.end(), [&](char c){
              return std::isspace(c);
            }) == p.second.end();
          }).base(),
        lines.end());
      return lines;
    }

    [[nodiscard]] auto get_lines(unsigned line_start, unsigned line_count) -> lines_t {
      auto lines = lines_t();
      return this->get_lines(line_start, line_count, lines);
    }

    auto swap(source_file& b) noexcept -> void { this->_file.swap(b._file); }

    [[maybe_unused]] static auto add_paths_to_env_variable_impl(std::string const& to_add) -> void {
      source_file::get_mutable_paths_from_env_variable().push_back(to_add);
    }

   private:
    details::handle<std::ifstream*, default_delete<std::ifstream*>> _file;

    [[nodiscard]] static auto get_paths_from_env_variable_impl() -> std::vector<std::string> {
      auto paths = std::vector<std::string>();
      auto const* prefixes_str = std::getenv("BACKWARD_CXX_SOURCE_PREFIXES");
      if(prefixes_str and prefixes_str[0]) // NOLINT(*-pro-bounds-pointer-arithmetic)
        paths = details::split_source_prefixes(prefixes_str);
      return paths;
    }

    [[nodiscard]] static auto get_mutable_paths_from_env_variable() -> std::vector<std::string>& {
      static volatile auto paths = source_file::get_paths_from_env_variable_impl();
      return const_cast<std::vector<std::string>&>(paths); // NOLINT(*-pro-type-const-cast)
    }

    [[nodiscard]] static auto get_paths_from_env_variable() -> std::vector<std::string> const& {
      return source_file::get_mutable_paths_from_env_variable();
    }
  };
} // namespace floppy::stacktrace