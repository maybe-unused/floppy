#pragma once

#include <cstdio>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <floppy/detail/print_helpers.h>
#include <floppy/backtrace/trace_resolver_concrete.h>
#include <floppy/backtrace/snippet_factory.h>

namespace floppy::stacktrace::impl
{
  class printer final
  {
   public:
    bool snippet;
    bool address;
    bool object;
    int inliner_context_size;
    int trace_context_size;
    bool reverse;

    printer()
      : snippet(true)
      , address(false)
      , object(false)
      , inliner_context_size(5)
      , trace_context_size(7)
      , reverse(true)
    {}

    template <typename St>
    auto print(St &st, FILE *fp = stderr) -> std::FILE* {
      this->print_stacktrace(st, fp);
      return fp;
    }

    template <typename St>
    [[maybe_unused]] auto print(St &st, std::filesystem::path const& path) -> void {
      auto os = std::ofstream(path);
      this->print_stacktrace(st, os);
    }

    template <typename It>
    auto print(It begin, It end, std::FILE* fp = stderr, size_t thread_id = 0) -> std::FILE* {
      this->print_stacktrace(begin, end, fp, thread_id);
      return fp;
    }

    template <typename It>
    [[maybe_unused]] auto print(It begin, It end, std::filesystem::path const& path, size_t thread_id = 0) -> void {
      auto os = std::ofstream(path);
      this->print_stacktrace(begin, end, os, thread_id);
    }

    [[nodiscard]] auto resolver() const -> trace_resolver const& { return this->_resolver; }

   private:
    trace_resolver _resolver; // NOLINT(*-identifier-naming)
    snippet_factory _snippets; // NOLINT(*-identifier-naming)

    template <typename St>
    auto print_stacktrace(St& st, std::FILE* out) -> void {
      printer::print_header(out, st.thread_id());
      _resolver.load_stacktrace(st);
      if(reverse)
        for(size_t trace_idx = st.size(); trace_idx > 0; --trace_idx)
          this->print_trace(out, _resolver.resolve(st[trace_idx - 1]));
      else
        for(size_t trace_idx = 0; trace_idx < st.size(); ++trace_idx)
          this->print_trace(out, _resolver.resolve(st[trace_idx]));
    }

    template <typename St>
    [[maybe_unused]] auto print_stacktrace(St& st, std::ofstream& out) -> void {
      printer::print_header(out, st.thread_id());
      _resolver.load_stacktrace(st);
      if(reverse)
        for(size_t trace_idx = st.size(); trace_idx > 0; --trace_idx)
          this->print_trace(out, _resolver.resolve(st[trace_idx - 1]));
      else
        for(size_t trace_idx = 0; trace_idx < st.size(); ++trace_idx)
          this->print_trace(out, _resolver.resolve(st[trace_idx]));
    }

    template <typename It>
    auto print_stacktrace(It begin, It end, std::FILE* out, size_t thread_id) -> void {
      printer::print_header(out, thread_id);
      for(; begin != end; ++begin)
        this->print_trace(out, *begin);
    }

    template <typename It>
    [[maybe_unused]] auto print_stacktrace(It begin, It end, std::ofstream& out, size_t thread_id) -> void {
      printer::print_header(out, thread_id);
      for(; begin != end; ++begin)
        this->print_trace(out, *begin);
    }

    static auto print_header(std::FILE* out, size_t thread_id) -> void {
      fmt::print(
        out,
        fmt::emphasis::bold | fg(fmt::terminal_color::red),
        "Stack trace (most recent call last){}:\n",
        thread_id
          ? " in thread " + std::to_string(thread_id)
          : ""
      );
    }

    [[maybe_unused]] static auto print_header(std::ofstream& out, size_t thread_id) -> void {
      out << fmt::format(
        "Stack trace (most recent call last){}:\n",
        thread_id
          ? " in thread " + std::to_string(thread_id)
          : ""
      );
    }

    static auto indent(std::FILE* out, size_t idx) -> void {
      fmt::print(out, fmt::emphasis::bold | fg(fmt::terminal_color::red), "#{:<2}: ", idx);
    }

    [[maybe_unused]] static auto indent(std::ofstream& out, size_t idx) -> void {
      out << fmt::format("#{:<2}: ", idx);
    }

    auto print_trace(std::FILE* out, resolved_trace const& trace) -> void {
      printer::indent(out, trace.idx);
      auto already_indented = true;
      if(trace.source.file_name().empty() or this->object) {
        fmt::print(out, R"(   Object '{}' at '{}' in '{}')",
          fmt::styled(trace.object_filename, fmt::emphasis::bold | fg(fmt::terminal_color::white)),
          fmt::styled(trace.addr, fmt::emphasis::faint | fg(fmt::terminal_color::white)),
          fmt::styled(trace.object_function, fg(fmt::terminal_color::yellow))
        );
        fmt::print(out, "\n");
        already_indented = false;
      }

      for(size_t inliner_idx = trace.inliners.size(); inliner_idx > 0; --inliner_idx) {
        if(not already_indented)
          fmt::print(out, "    ");
        auto const& inliner_loc = trace.inliners[inliner_idx - 1];
        this->print_source_loc(out, " | ", inliner_loc);
        if(snippet)
          this->print_snippet(out, "    | ", inliner_loc, inliner_context_size);
        already_indented = false;
      }

      if(not trace.source.file_name().empty()) {
        if(not already_indented)
          fmt::print(out, "    ");
        this->print_source_loc(out, "   ", trace.source, trace.addr);
        if(snippet)
          this->print_snippet(out, "      ", trace.source, trace_context_size);
      }
    }

    [[maybe_unused]] auto print_trace(std::ofstream& out, resolved_trace const& trace) -> void {
      printer::indent(out, trace.idx);
      auto already_indented = true;
      if(trace.source.file_name().empty() or this->object) {
        out << fmt::format(R"(   Object '{}' at '{}' in '{}')",
          trace.object_filename,
          trace.addr,
          trace.object_function
        );
        out << "\n";
        already_indented = false;
      }

      for(size_t inliner_idx = trace.inliners.size(); inliner_idx > 0; --inliner_idx) {
        if(not already_indented)
          out << "    ";
        auto const& inliner_loc = trace.inliners[inliner_idx - 1];
        this->print_source_loc(out, " | ", inliner_loc);
        if(snippet)
          this->print_snippet(out, "    | ", inliner_loc, inliner_context_size);
        already_indented = false;
      }

      if(not trace.source.file_name().empty()) {
        if(not already_indented)
          out << "    ";
        this->print_source_loc(out, "   ", trace.source, trace.addr);
        if(snippet)
          this->print_snippet(out, "      ", trace.source, trace_context_size);
      }
    }

    auto print_snippet(
      std::FILE* out,
      char const* indent,
      resolved_trace::source_loc_t const& source_loc,
      int context_size
    ) -> void {
      typedef impl::snippet_factory::lines_t lines_t;

      auto lines = this->_snippets.get_snippet(
        std::string(source_loc.file_name()),
        source_loc.line(),
        static_cast<unsigned>(context_size)
      );

      for(auto it = lines.begin(); it != lines.end(); ++it) {
        auto highlight = false;
        if(it->first == source_loc.line()) {
          fmt::print(out, fmt::emphasis::bold | fg(fmt::terminal_color::bright_yellow), "{}>", indent);
          highlight = true;
        } else
          fmt::print(out, fg(fmt::terminal_color::bright_magenta), "{} ", indent);
        if(highlight)
          fmt::print(out, fmt::emphasis::bold | fg(fmt::terminal_color::bright_yellow), "{:>4}: {}\n", it->first, it->second);
        else
          fmt::print(out, fg(fmt::terminal_color::bright_magenta), "{:>4}: {}\n", it->first, it->second);
      }
    }

    [[maybe_unused]] auto print_snippet(
      std::ofstream& out,
      char const* indent,
      resolved_trace::source_loc_t const& source_loc,
      int context_size
      ) -> void {
      typedef impl::snippet_factory::lines_t lines_t;

      auto lines = this->_snippets.get_snippet(
        std::string(source_loc.file_name()),
        source_loc.line(),
        static_cast<unsigned>(context_size)
      );

      for(auto it = lines.begin(); it != lines.end(); ++it) {
        if(it->first == source_loc.line())
          out << fmt::format("{}>", indent);
        else
          out << fmt::format("{} ", indent);
        out << fmt::format("{:>4}: {}\n", it->first, it->second);
      }
    }

    auto print_source_loc(
      std::FILE* out,
      char const* indent,
      resolved_trace::source_loc_t const& source_loc,
      void* addr = nullptr
    ) const -> void {
      fmt::print(out, "{}Source \'{}\', line {} in {} {}\n",
        indent,
        fmt::styled(print_helpers::truncate(source_loc.file_name(), 45, direction::reverse), fg(fmt::terminal_color::white)),
        fmt::styled(source_loc.line(), fg(fmt::terminal_color::white)),
        fmt::styled(source_loc.function_name(), fg(fmt::terminal_color::yellow)),
        fmt::styled(address and addr != nullptr ? fmt::format("[0x{:p}]", addr) : "", fmt::emphasis::faint | fg(fmt::terminal_color::white))
      );
    }

    [[maybe_unused]] auto print_source_loc(
      std::ofstream& out,
      char const* indent,
      resolved_trace::source_loc_t const& source_loc,
      void* addr = nullptr
    ) const -> void {
      out << fmt::format("{}Source \'{}\', line {} in {} {}\n",
        indent,
        print_helpers::truncate(source_loc.file_name(), 45, direction::reverse),
        source_loc.line(),
        source_loc.function_name(),
        address and addr != nullptr ? fmt::format("[0x{:p}]", addr) : ""
      );
    }
  };
} // namespace floppy::stacktrace::impl