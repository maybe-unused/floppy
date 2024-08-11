#pragma once

#include <unordered_map>
#include <iterator>
#include <floppy/backtrace/source_file.h>

namespace floppy::stacktrace::impl
{
  class snippet_factory final
  {
   public:
    using lines_t = source_file::lines_t;

    [[nodiscard]] auto get_snippet(std::string const& filename, unsigned line_start, unsigned context_size) -> lines_t {
      auto& src_file = this->get_src_file(filename);
      unsigned const start = line_start - context_size / 2;
      return src_file.get_lines(start, context_size);
    }

    [[maybe_unused]] auto get_combined_snippet(
      std::string const& filename_a,
      unsigned line_a,
      std::string const& filename_b,
      unsigned line_b,
      unsigned context_size
    ) -> lines_t {
      auto& src_file_a = this->get_src_file(filename_a);
      auto& src_file_b = this->get_src_file(filename_b);

      auto lines = src_file_a.get_lines(line_a - context_size / 4, context_size / 2);
      std::ignore = src_file_b.get_lines(line_b - context_size / 4, context_size / 2, lines);
      return lines;
    }

    [[maybe_unused]] auto get_coalesced_snippet(
      std::string const& filename,
      unsigned line_a,
      unsigned line_b,
      unsigned context_size
    ) -> lines_t {
      auto& src_file = this->get_src_file(filename);
      unsigned const a = std::min(line_a, line_b);
      unsigned const b = std::max(line_a, line_b);

      if((b - a) < (context_size / 3))
        return src_file.get_lines((a + b - context_size + 1) / 2, context_size);

      auto lines = src_file.get_lines(a - context_size / 4, context_size / 2);
      std::ignore = src_file.get_lines(b - context_size / 4, context_size / 2, lines);
      return lines;
    }

   private:
    std::unordered_map<std::string, source_file> _src_files;

    [[nodiscard]] auto get_src_file(std::string const& filename) -> source_file& {
      auto it = this->_src_files.find(filename);
      if(it != this->_src_files.end())
        return it->second;
      auto& new_src_file = this->_src_files[filename];
      new_src_file = source_file(filename);
      return new_src_file;
    }
  };
} // namespace floppy::stacktrace::impl