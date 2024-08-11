#pragma once

#include <floppy/backtrace/trace_resolver.h>
#include <floppy/backtrace/unix/stack_trace.h>

namespace floppy::stacktrace::impl
{
#ifdef FLOPPY_OS_LINUX
  class trace_resolver_linux_base : public trace_resolver_impl_base {
   public:
    trace_resolver_linux_base()
      : argv0_(get_argv0()), exec_path_(read_symlink("/proc/self/exe")) {}
    std::string resolve_exec_path(Dl_info &symbol_info) const {
      if(symbol_info.dli_fname == argv0_) {
        symbol_info.dli_fname = "/proc/self/exe";
        return exec_path_;
      } else
        return symbol_info.dli_fname;
    }

   private:
    std::string argv0_;
    std::string exec_path_;

    static std::string get_argv0() {
      std::string argv0;
      std::ifstream ifs("/proc/self/cmdline");
      std::getline(ifs, argv0, '\0');
      return argv0;
    }

    static std::string read_symlink(std::string const &symlink_path) {
      std::string path;
      path.resize(100);
      while (true) {
        isize len = ::readlink(symlink_path.c_str(), &*path.begin(), path.size());
        if(len < 0)
          return "";
        if (static_cast<size_t>(len) == path.size()) {
          path.resize(path.size() * 2);
        } else {
          path.resize(static_cast<std::string::size_type>(len));
          break;
        }
      }

      return path;
    }
  };

  template <typename STACKTRACE_TAG> class trace_resolver_linux_impl;

  template <>
  class trace_resolver_linux_impl<trace_resolver_tag::libdw>
    : public trace_resolver_linux_base {
   public:
    trace_resolver_linux_impl() : _dwfl_handle_initialized(false) {}

    resolved_trace resolve(resolved_trace trace) override {
      using namespace details;
      Dwarf_Addr trace_addr = reinterpret_cast<Dwarf_Addr>(trace.addr);
      if (!_dwfl_handle_initialized) {
        _dwfl_cb.reset(new Dwfl_Callbacks);
        _dwfl_cb->find_elf = &dwfl_linux_proc_find_elf;
        _dwfl_cb->find_debuginfo = &dwfl_standard_find_debuginfo;
        _dwfl_cb->debuginfo_path = 0;
        _dwfl_handle.reset(dwfl_begin(_dwfl_cb.get()));
        _dwfl_handle_initialized = true;

        if(not _dwfl_handle)
          return trace;

        dwfl_report_begin(_dwfl_handle.get());
        int r = dwfl_linux_proc_report(_dwfl_handle.get(), getpid());
        dwfl_report_end(_dwfl_handle.get(), NULL, NULL);
        if(r < 0)
          return trace;
      }

      if(not _dwfl_handle)
        return trace;
      Dwfl_Module *mod = dwfl_addrmodule(_dwfl_handle.get(), trace_addr);
      if(mod) {
        const char *module_name = dwfl_module_info(mod, 0, 0, 0, 0, 0, 0, 0);
        if(module_name)
          trace.object_filename = module_name;

        const char *sym_name = dwfl_module_addrname(mod, trace_addr);
        if(sym_name)
          trace.object_function = demangle(sym_name);
      }

      Dwarf_Addr mod_bias = 0;
      Dwarf_Die *cudie = dwfl_module_addrdie(mod, trace_addr, &mod_bias);
      if(not cudie) {
        while((cudie = dwfl_module_nextcu(mod, cudie, &mod_bias))) {
          Dwarf_Die die_mem;
          Dwarf_Die *fundie = find_fundie_by_pc(cudie, trace_addr - mod_bias, &die_mem);
          if(fundie)
            break;
        }
      }

      if(not cudie)
        return trace; // this time we lost the game :/

      // Now that we have a compilation unit DIE, this function will be able
      // to load the corresponding section in .debug_line (if not already
      // loaded) and hopefully find the source location mapped to our
      // address.
      Dwarf_Line *srcloc = dwarf_getsrc_die(cudie, trace_addr - mod_bias);

      if (srcloc) {
        const char *srcfile = dwarf_linesrc(srcloc, 0, 0);
        if (srcfile) {
          trace.source.file_name_mut() = srcfile;
        }
        int line = 0, col = 0;
        dwarf_lineno(srcloc, &line);
        dwarf_linecol(srcloc, &col);
        trace.source.line_mut() = static_cast<unsigned>(line);
        trace.source.column_mut() = static_cast<unsigned>(col);
      }
      deep_first_search_by_pc(cudie, trace_addr - mod_bias, inliners_search_cb(trace));
      if(trace.source.function_name().empty())
        trace.source.function_name_mut() = trace.object_function;
      return trace;
    }

   private:
    typedef details::handle<Dwfl *, deleter<void, Dwfl *, &dwfl_end> >
      dwfl_handle_t;
    details::handle<Dwfl_Callbacks *, default_delete<Dwfl_Callbacks *> >
      _dwfl_cb;
    dwfl_handle_t _dwfl_handle;
    bool _dwfl_handle_initialized;

    struct inliners_search_cb {
      void operator()(Dwarf_Die *die) {
        switch (dwarf_tag(die)) {
          const char *name;
          case DW_TAG_subprogram:
            if((name = dwarf_diename(die)))
              trace.source.function_name_mut() = name;
            break;

          case DW_TAG_inlined_subroutine:
            resolved_trace::source_loc_t sloc;
            Dwarf_Attribute attr_mem;

            if((name = dwarf_diename(die)))
              sloc.function_name_mut() = name;
            if((name = die_call_file(die)))
              sloc.file_name_mut() = name;

            Dwarf_Word line = 0, col = 0;
            dwarf_formudata(dwarf_attr(die, DW_AT_call_line, &attr_mem), &line);
            dwarf_formudata(dwarf_attr(die, DW_AT_call_column, &attr_mem), &col);
            sloc.line_mut() = static_cast<unsigned>(line);
            sloc.column_mut() = static_cast<unsigned>(col);
            trace.inliners.push_back(sloc);
            break;
        };
      }
      resolved_trace &trace;
      inliners_search_cb(resolved_trace &t) : trace(t) {}
    };

    static bool die_has_pc(Dwarf_Die *die, Dwarf_Addr pc) {
      Dwarf_Addr low, high;

      // continuous range
      if (dwarf_hasattr(die, DW_AT_low_pc) && dwarf_hasattr(die, DW_AT_high_pc)) {
        if(dwarf_lowpc(die, &low) != 0)
          return false;
        if(dwarf_highpc(die, &high) != 0) {
          Dwarf_Attribute attr_mem;
          Dwarf_Attribute *attr = dwarf_attr(die, DW_AT_high_pc, &attr_mem);
          Dwarf_Word value;
          if(dwarf_formudata(attr, &value) != 0)
            return false;
          high = low + value;
        }
        return pc >= low && pc < high;
      }

      // non-continuous range.
      Dwarf_Addr base;
      ptrdiff_t offset = 0;
      while ((offset = dwarf_ranges(die, offset, &base, &low, &high)) > 0) {
        if(pc >= low && pc < high)
          return true;
      }
      return false;
    }

    static Dwarf_Die *find_fundie_by_pc(Dwarf_Die *parent_die, Dwarf_Addr pc,
                                        Dwarf_Die *result) {
      if(dwarf_child(parent_die, result) != 0)
        return 0;
      Dwarf_Die *die = result;
      do {
        switch (dwarf_tag(die)) {
          case DW_TAG_subprogram:
          case DW_TAG_inlined_subroutine:
            if(die_has_pc(die, pc))
              return result;
        };
        bool declaration = false;
        Dwarf_Attribute attr_mem;
        dwarf_formflag(dwarf_attr(die, DW_AT_declaration, &attr_mem),
                       &declaration);
        if(not declaration) {
          // let's be curious and look deeper in the tree,
          // function are not necessarily at the first level, but
          // might be nested inside a namespace, structure etc.
          Dwarf_Die die_mem;
          Dwarf_Die *indie = find_fundie_by_pc(die, pc, &die_mem);
          if(indie) {
            *result = die_mem;
            return result;
          }
        }
      } while(dwarf_siblingof(die, result) == 0);
      return 0;
    }

    template <typename CB>
    static bool deep_first_search_by_pc(Dwarf_Die *parent_die, Dwarf_Addr pc,
                                        CB cb) {
      Dwarf_Die die_mem;
      if(dwarf_child(parent_die, &die_mem) != 0)
        return false;

      bool branch_has_pc = false;
      Dwarf_Die *die = &die_mem;
      do {
        bool declaration = false;
        Dwarf_Attribute attr_mem;
        dwarf_formflag(dwarf_attr(die, DW_AT_declaration, &attr_mem),
                       &declaration);
        if (!declaration) {
          // let's be curious and look deeper in the tree, function are
          // not necessarily at the first level, but might be nested
          // inside a namespace, structure, a function, an inlined
          // function etc.
          branch_has_pc = deep_first_search_by_pc(die, pc, cb);
        }
        if(not branch_has_pc)
          branch_has_pc = die_has_pc(die, pc);
        if(branch_has_pc)
          cb(die);
      } while(dwarf_siblingof(die, &die_mem) == 0);
      return branch_has_pc;
    }

    static const char* die_call_file(Dwarf_Die *die) {
      Dwarf_Attribute attr_mem;
      Dwarf_Word file_idx = 0;
      dwarf_formudata(dwarf_attr(die, DW_AT_call_file, &attr_mem), &file_idx);
      if(file_idx == 0)
        return 0;
      Dwarf_Die die_mem;
      Dwarf_Die *cudie = dwarf_diecu(die, &die_mem, 0, 0);
      if(not cudie)
        return 0;
      Dwarf_Files *files = 0;
      size_t nfiles;
      dwarf_getsrcfiles(cudie, &files, &nfiles);
      if(not files)
        return 0;
      return dwarf_filesrc(files, file_idx, 0, 0);
    }
  };

  template <>
  class trace_resolver_impl<system_tag::linux_>
    : public trace_resolver_linux_impl<trace_resolver_tag::current> {};

#endif // FLOPPY_OS_LINUX

#ifdef FLOPPY_OS_DARWIN

  template <typename STACKTRACE_TAG> class trace_resolver_darwin_impl;

  template <>
  class trace_resolver_darwin_impl<trace_resolver_tag::backtrace_symbol>
    : public trace_resolver_impl_base {
   public:
    void load_addresses(void *const*addresses, int address_count) override {
      if(address_count == 0)
        return;
      _symbols.reset(backtrace_symbols(addresses, address_count));
    }

    resolved_trace resolve(resolved_trace trace) override {
      // parse:
      // <n>  <file>  <addr>  <mangled-name> + <offset>
      char *filename = _symbols[trace.idx];

      // skip "<n>  "
      while(*filename && *filename != ' ')
        filename++;
      while(*filename == ' ')
        filename++;

      // find start of <mangled-name> from end (<file> may contain a space)
      char *p = filename + strlen(filename) - 1;
      // skip to start of " + <offset>"
      while(p > filename && *p != ' ')
        p--;
      while(p > filename && *p == ' ')
        p--;
      while(p > filename && *p != ' ')
        p--;
      while(p > filename && *p == ' ')
        p--;
      char *funcname_end = p + 1;

      // skip to start of "<manged-name>"
      while(p > filename && *p != ' ')
        p--;
      char *funcname = p + 1;

      // skip to start of "  <addr>  "
      while(p > filename && *p == ' ')
        p--;
      while(p > filename && *p != ' ')
        p--;
      while(p > filename && *p == ' ')
        p--;

      // skip "<file>", handling the case where it contains a
      char *filename_end = p + 1;
      if(p == filename) {
        // something went wrong, give up
        filename_end = filename + strlen(filename);
        funcname = filename_end;
      }
      trace.object_filename.assign(
        filename, filename_end); // ok even if filename_end is the ending \0
                                 // (then we assign entire string)

      if(*funcname) { // if it's not end of string
        *funcname_end = '\0';

        trace.object_function = this->demangle(funcname);
        trace.object_function += " ";
        trace.object_function += (funcname_end + 1);
        trace.source.function = trace.object_function; // we cannot do better.
      }
      return trace;
    }

   private:
    details::handle<char **> _symbols;
  };

  template <>
  class trace_resolver_impl<system_tag::darwin> : public trace_resolver_darwin_impl<trace_resolver_tag::current> {};

#endif // FLOPPY_OS_DARWIN
} // namespace floppy::stacktrace::impl