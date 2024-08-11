#pragma once

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <new>
#include <sstream>
#include <streambuf>
#include <exception>
#include <iterator>
#include <unordered_map>
#include <floppy/backtrace/trace_resolver_tag.h>
#include <floppy/backtrace/handle.h>
#include <floppy/backtrace/demangle.h>
#include <floppy/backtrace/trace.h>
#include <floppy/backtrace/resolved_trace.h>
#include <floppy/floppy.h>
#if defined(FLOPPY_OS_LINUX)
# include <cxxabi.h>
# include <fcntl.h>
# ifdef __ANDROID__
#   define _Unwind_Ptr _Unwind_Ptr_Custom
#   include <link.h>
#   undef _Unwind_Ptr
# else // __ANDROID__
#   include <link.h>
# endif // __ANDROID__
# if defined(__ppc__) || defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__)
#   include <asm/ptrace.h>
# endif // defined(__ppc__) || defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__)
# include <signal.h>
# include <sys/stat.h>
# include <syscall.h>
# include <unistd.h>
# ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#   include <dlfcn.h>
#   undef _GNU_SOURCE
# else // _GNU_SOURCE
#   include <dlfcn.h>
# endif // _GNU_SOURCE
# include <dwarf.h>
# include <elfutils/libdw.h>
# include <elfutils/libdwfl.h>
# include <execinfo.h>
#endif // defined(FLOPPY_OS_LINUX)
#if defined(FLOPPY_OS_DARWIN)
# include <cxxabi.h>
# include <fcntl.h>
# include <pthread.h>
# include <signal.h>
# include <sys/stat.h>
# include <unistd.h>
# include <execinfo.h>
#endif // defined(FLOPPY_OS_DARWIN)
#if defined(FLOPPY_OS_WINDOWS)
# include <condition_variable>
# include <mutex>
# include <thread>
# include <basetsd.h>
# ifndef NOMINMAX
#   define NOMINMAX
# endif // NOMINMAX
# include <windows.h>
# include <winnt.h>
# include <psapi.h>
# include <csignal>
# ifdef _MSC_VER
#   pragma comment(lib, "psapi.lib")
#   pragma comment(lib, "dbghelp.lib")
# endif // _MSC_VER
# pragma pack(push, before_imagehlp, 8)
#   include <imagehlp.h>
# pragma pack(pop, before_imagehlp)
#endif // defined(FLOPPY_OS_WINDOWS)

/// \brief Stacktrace namespace.
/// \details Currently implemented using reworked <a href="https://github.com/bombela/backward-cpp">backward-cpp</a> library.
/// \ingroup foundation
/// \sa https://github.com/bombela/backward-cpp
namespace floppy::stacktrace {

/*************** STACK TRACE ***************/

template <typename TAG> class StackTraceImpl {
public:
  size_t size() const { return 0; }
  trace operator[](size_t) const { return trace(); }
  size_t load_here(size_t = 0) { return 0; }
  size_t load_from(void *, size_t = 0, void * = nullptr, void * = nullptr) {
    return 0;
  }
  size_t thread_id() const { return 0; }
  void skip_n_firsts(size_t) {}
  void *const *begin() const { return nullptr; }
};

class StackTraceImplBase {
public:
  StackTraceImplBase()
      : _thread_id(0), _skip(0), _context(nullptr), _error_addr(nullptr) {}

  size_t thread_id() const { return _thread_id; }

  void skip_n_firsts(size_t n) { _skip = n; }

protected:
  void load_thread_info() {
#ifdef FLOPPY_OS_LINUX
#ifndef __ANDROID__
    _thread_id = static_cast<size_t>(syscall(SYS_gettid));
#else
    _thread_id = static_cast<size_t>(gettid());
#endif
    if (_thread_id == static_cast<size_t>(getpid())) {
      // If the thread is the main one, let's hide that.
      // I like to keep little secret sometimes.
      _thread_id = 0;
    }
#elif defined(FLOPPY_OS_DARWIN)
    _thread_id = reinterpret_cast<size_t>(pthread_self());
    if (pthread_main_np() == 1) {
      // If the thread is the main one, let's hide that.
      _thread_id = 0;
    }
#endif
  }

  void set_context(void *context) { _context = context; }
  void *context() const { return _context; }

  void set_error_addr(void *error_addr) { _error_addr = error_addr; }
  void *error_addr() const { return _error_addr; }

  size_t skip_n_firsts() const { return _skip; }

private:
  size_t _thread_id;
  size_t _skip;
  void *_context;
  void *_error_addr;
};

class StackTraceImplHolder : public StackTraceImplBase {
public:
  size_t size() const {
    return (_stacktrace.size() >= skip_n_firsts())
               ? _stacktrace.size() - skip_n_firsts()
               : 0;
  }
  trace operator[](size_t idx) const {
    if (idx >= size()) {
      return trace();
    }
    return trace(_stacktrace[idx + skip_n_firsts()], idx);
  }
  void *const *begin() const {
    if (size()) {
      return &_stacktrace[skip_n_firsts()];
    }
    return nullptr;
  }

protected:
  std::vector<void *> _stacktrace;
};

#if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
template <>
class StackTraceImpl<system_tag::current> : public StackTraceImplHolder {
public:
  __noinline__
  size_t load_here(size_t depth = 32, void *context = nullptr,
                   void *error_addr = nullptr) {
    set_context(context);
    set_error_addr(error_addr);
    load_thread_info();
    if (depth == 0) {
      return 0;
    }
    _stacktrace.resize(depth + 1);
    size_t trace_cnt = backtrace(&_stacktrace[0], _stacktrace.size());
    _stacktrace.resize(trace_cnt);
    skip_n_firsts(1);
    return size();
  }

  size_t load_from(void *addr, size_t depth = 32, void *context = nullptr,
                   void *error_addr = nullptr) {
    load_here(depth + 8, context, error_addr);

    for (size_t i = 0; i < _stacktrace.size(); ++i) {
      if (_stacktrace[i] == addr) {
        skip_n_firsts(i);
        _stacktrace[i] = (void *)((uintptr_t)_stacktrace[i] + 1);
        break;
      }
    }

    _stacktrace.resize(std::min(_stacktrace.size(), skip_n_firsts() + depth));
    return size();
  }
};

#elif defined(FLOPPY_OS_WINDOWS)

template <>
class StackTraceImpl<system_tag::current> : public StackTraceImplHolder {
public:
  // We have to load the machine type from the image info
  // So we first initialize the resolver, and it tells us this info
  void set_machine_type(DWORD machine_type) { machine_type_ = machine_type; }
  void set_context(CONTEXT *ctx) { ctx_ = ctx; }
  void set_thread_handle(HANDLE handle) { thd_ = handle; }

  __noinline__
  size_t load_here(size_t depth = 32, void *context = nullptr,
                   void *error_addr = nullptr) {
    set_context(static_cast<CONTEXT*>(context));
    set_error_addr(error_addr);
    CONTEXT localCtx; // used when no context is provided

    if (depth == 0) {
      return 0;
    }

    if (!ctx_) {
      ctx_ = &localCtx;
      RtlCaptureContext(ctx_);
    }

    if (!thd_) {
      thd_ = GetCurrentThread();
    }

    HANDLE process = GetCurrentProcess();

    STACKFRAME64 s;
    memset(&s, 0, sizeof(STACKFRAME64));

    // TODO: 32 bit context capture
    s.AddrStack.Mode = AddrModeFlat;
    s.AddrFrame.Mode = AddrModeFlat;
    s.AddrPC.Mode = AddrModeFlat;
#ifdef _M_X64
    s.AddrPC.Offset = ctx_->Rip;
    s.AddrStack.Offset = ctx_->Rsp;
    s.AddrFrame.Offset = ctx_->Rbp;
#else
    s.AddrPC.Offset = ctx_->Eip;
    s.AddrStack.Offset = ctx_->Esp;
    s.AddrFrame.Offset = ctx_->Ebp;
#endif

    if (!machine_type_) {
#ifdef _M_X64
      machine_type_ = IMAGE_FILE_MACHINE_AMD64;
#else
      machine_type_ = IMAGE_FILE_MACHINE_I386;
#endif
    }

    for(;;) {
      // NOTE: this only works if PDBs are already loaded!
      SetLastError(0);
      if (!StackWalk64(machine_type_, process, thd_, &s, ctx_, NULL,
                       SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        break;

      if (s.AddrReturn.Offset == 0)
        break;

      _stacktrace.push_back(reinterpret_cast<void *>(s.AddrPC.Offset));

      if (size() >= depth)
        break;
    }

    return size();
  }

  size_t load_from(void *addr, size_t depth = 32, void *context = nullptr,
                   void *error_addr = nullptr) {
    load_here(depth + 8, context, error_addr);

    for (size_t i = 0; i < _stacktrace.size(); ++i) {
      if (_stacktrace[i] == addr) {
        skip_n_firsts(i);
        break;
      }
    }

    _stacktrace.resize(std::min(_stacktrace.size(), skip_n_firsts() + depth));
    return size();
  }

private:
  DWORD machine_type_ = 0;
  HANDLE thd_ = 0;
  CONTEXT *ctx_ = nullptr;
};

#endif

class stack_trace : public StackTraceImpl<system_tag::current> {};

/*************** TRACE RESOLVER ***************/

class TraceResolverImplBase {
public:
  virtual ~TraceResolverImplBase() {}

  virtual void load_addresses(void *const*addresses, int address_count) {
    (void)addresses;
    (void)address_count;
  }

  template <class ST> void load_stacktrace(ST &st) {
    load_addresses(st.begin(), static_cast<int>(st.size()));
  }

  virtual resolved_trace resolve(resolved_trace t) { return t; }

protected:
  std::string demangle(const char *funcname) {
    return _demangler.demangle(funcname);
  }

private:
  details::demangler _demangler;
};

template <typename TAG> class TraceResolverImpl;

#ifdef FLOPPY_OS_UNKNOWN

template <> class TraceResolverImpl<system_tag::unknown>
    : public TraceResolverImplBase {};

#endif

#ifdef FLOPPY_OS_LINUX

class TraceResolverLinuxBase : public TraceResolverImplBase {
public:
  TraceResolverLinuxBase()
      : argv0_(get_argv0()), exec_path_(read_symlink("/proc/self/exe")) {}
  std::string resolve_exec_path(Dl_info &symbol_info) const {
    // mutates symbol_info.dli_fname to be filename to open and returns filename
    // to display
    if (symbol_info.dli_fname == argv0_) {
      // dladdr returns argv[0] in dli_fname for symbols contained in
      // the main executable, which is not a valid path if the
      // executable was found by a search of the PATH environment
      // variable; In that case, we actually open /proc/self/exe, which
      // is always the actual executable (even if it was deleted/replaced!)
      // but display the path that /proc/self/exe links to.
      // However, this right away reduces probability of successful symbol
      // resolution, because libbfd may try to find *.debug files in the
      // same dir, in case symbols are stripped. As a result, it may try
      // to find a file /proc/self/<exe_name>.debug, which obviously does
      // not exist. /proc/self/exe is a last resort. First load attempt
      // should go for the original executable file path.
      symbol_info.dli_fname = "/proc/self/exe";
      return exec_path_;
    } else {
      return symbol_info.dli_fname;
    }
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
      isize len =
          ::readlink(symlink_path.c_str(), &*path.begin(), path.size());
      if (len < 0) {
        return "";
      }
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

template <typename STACKTRACE_TAG> class TraceResolverLinuxImpl;

template <>
class TraceResolverLinuxImpl<trace_resolver_tag::libdw>
    : public TraceResolverLinuxBase {
public:
  TraceResolverLinuxImpl() : _dwfl_handle_initialized(false) {}

  resolved_trace resolve(resolved_trace trace) override {
    using namespace details;

    Dwarf_Addr trace_addr = reinterpret_cast<Dwarf_Addr>(trace.addr);

    if (!_dwfl_handle_initialized) {
      // initialize dwfl...
      _dwfl_cb.reset(new Dwfl_Callbacks);
      _dwfl_cb->find_elf = &dwfl_linux_proc_find_elf;
      _dwfl_cb->find_debuginfo = &dwfl_standard_find_debuginfo;
      _dwfl_cb->debuginfo_path = 0;

      _dwfl_handle.reset(dwfl_begin(_dwfl_cb.get()));
      _dwfl_handle_initialized = true;

      if (!_dwfl_handle) {
        return trace;
      }

      // ...from the current process.
      dwfl_report_begin(_dwfl_handle.get());
      int r = dwfl_linux_proc_report(_dwfl_handle.get(), getpid());
      dwfl_report_end(_dwfl_handle.get(), NULL, NULL);
      if (r < 0) {
        return trace;
      }
    }

    if (!_dwfl_handle) {
      return trace;
    }

    // find the module (binary object) that contains the trace's address.
    // This is not using any debug information, but the addresses ranges of
    // all the currently loaded binary object.
    Dwfl_Module *mod = dwfl_addrmodule(_dwfl_handle.get(), trace_addr);
    if (mod) {
      // now that we found it, lets get the name of it, this will be the
      // full path to the running binary or one of the loaded library.
      const char *module_name = dwfl_module_info(mod, 0, 0, 0, 0, 0, 0, 0);
      if (module_name) {
        trace.object_filename = module_name;
      }
      // We also look after the name of the symbol, equal or before this
      // address. This is found by walking the symtab. We should get the
      // symbol corresponding to the function (mangled) containing the
      // address. If the code corresponding to the address was inlined,
      // this is the name of the out-most inliner function.
      const char *sym_name = dwfl_module_addrname(mod, trace_addr);
      if (sym_name) {
        trace.object_function = demangle(sym_name);
      }
    }

    // now let's get serious, and find out the source location (file and
    // line number) of the address.

    // This function will look in .debug_aranges for the address and map it
    // to the location of the compilation unit DIE in .debug_info and
    // return it.
    Dwarf_Addr mod_bias = 0;
    Dwarf_Die *cudie = dwfl_module_addrdie(mod, trace_addr, &mod_bias);

    if (!cudie) {
      // Sadly clang does not generate the section .debug_aranges, thus
      // dwfl_module_addrdie will fail early. Clang doesn't either set
      // the lowpc/highpc/range info for every compilation unit.
      //
      // So in order to save the world:
      // for every compilation unit, we will iterate over every single
      // DIEs. Normally functions should have a lowpc/highpc/range, which
      // we will use to infer the compilation unit.

      // note that this is probably badly inefficient.
      while ((cudie = dwfl_module_nextcu(mod, cudie, &mod_bias))) {
        Dwarf_Die die_mem;
        Dwarf_Die *fundie =
            find_fundie_by_pc(cudie, trace_addr - mod_bias, &die_mem);
        if (fundie) {
          break;
        }
      }
    }

//#define BACKWARD_I_DO_NOT_RECOMMEND_TO_ENABLE_THIS_HORRIBLE_PIECE_OF_CODE
#ifdef BACKWARD_I_DO_NOT_RECOMMEND_TO_ENABLE_THIS_HORRIBLE_PIECE_OF_CODE
    if (!cudie) {
      // If it's still not enough, lets dive deeper in the shit, and try
      // to save the world again: for every compilation unit, we will
      // load the corresponding .debug_line section, and see if we can
      // find our address in it.

      Dwarf_Addr cfi_bias;
      Dwarf_CFI *cfi_cache = dwfl_module_eh_cfi(mod, &cfi_bias);

      Dwarf_Addr bias;
      while ((cudie = dwfl_module_nextcu(mod, cudie, &bias))) {
        if (dwarf_getsrc_die(cudie, trace_addr - bias)) {

          // ...but if we get a match, it might be a false positive
          // because our (address - bias) might as well be valid in a
          // different compilation unit. So we throw our last card on
          // the table and lookup for the address into the .eh_frame
          // section.

          handle<Dwarf_Frame *> frame;
          dwarf_cfi_addrframe(cfi_cache, trace_addr - cfi_bias, &frame);
          if (frame) {
            break;
          }
        }
      }
    }
#endif

    if (!cudie) {
      return trace; // this time we lost the game :/
    }

    // Now that we have a compilation unit DIE, this function will be able
    // to load the corresponding section in .debug_line (if not already
    // loaded) and hopefully find the source location mapped to our
    // address.
    Dwarf_Line *srcloc = dwarf_getsrc_die(cudie, trace_addr - mod_bias);

    if (srcloc) {
      const char *srcfile = dwarf_linesrc(srcloc, 0, 0);
      if (srcfile) {
        trace.source.filename = srcfile;
      }
      int line = 0, col = 0;
      dwarf_lineno(srcloc, &line);
      dwarf_linecol(srcloc, &col);
      trace.source.line = static_cast<unsigned>(line);
      trace.source.col = static_cast<unsigned>(col);
    }

    deep_first_search_by_pc(cudie, trace_addr - mod_bias,
                            inliners_search_cb(trace));
    if (trace.source.function.size() == 0) {
      // fallback.
      trace.source.function = trace.object_function;
    }

    return trace;
  }

private:
  typedef details::handle<Dwfl *, details::deleter<void, Dwfl *, &dwfl_end> >
      dwfl_handle_t;
  details::handle<Dwfl_Callbacks *, details::default_delete<Dwfl_Callbacks *> >
      _dwfl_cb;
  dwfl_handle_t _dwfl_handle;
  bool _dwfl_handle_initialized;

  // defined here because in C++98, template function cannot take locally
  // defined types... grrr.
  struct inliners_search_cb {
    void operator()(Dwarf_Die *die) {
      switch (dwarf_tag(die)) {
        const char *name;
      case DW_TAG_subprogram:
        if ((name = dwarf_diename(die))) {
          trace.source.function = name;
        }
        break;

      case DW_TAG_inlined_subroutine:
        resolved_trace::SourceLoc sloc;
        Dwarf_Attribute attr_mem;

        if ((name = dwarf_diename(die))) {
          sloc.function = name;
        }
        if ((name = die_call_file(die))) {
          sloc.filename = name;
        }

        Dwarf_Word line = 0, col = 0;
        dwarf_formudata(dwarf_attr(die, DW_AT_call_line, &attr_mem), &line);
        dwarf_formudata(dwarf_attr(die, DW_AT_call_column, &attr_mem), &col);
        sloc.line = static_cast<unsigned>(line);
        sloc.col = static_cast<unsigned>(col);

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
      if (dwarf_lowpc(die, &low) != 0) {
        return false;
      }
      if (dwarf_highpc(die, &high) != 0) {
        Dwarf_Attribute attr_mem;
        Dwarf_Attribute *attr = dwarf_attr(die, DW_AT_high_pc, &attr_mem);
        Dwarf_Word value;
        if (dwarf_formudata(attr, &value) != 0) {
          return false;
        }
        high = low + value;
      }
      return pc >= low && pc < high;
    }

    // non-continuous range.
    Dwarf_Addr base;
    ptrdiff_t offset = 0;
    while ((offset = dwarf_ranges(die, offset, &base, &low, &high)) > 0) {
      if (pc >= low && pc < high) {
        return true;
      }
    }
    return false;
  }

  static Dwarf_Die *find_fundie_by_pc(Dwarf_Die *parent_die, Dwarf_Addr pc,
                                      Dwarf_Die *result) {
    if (dwarf_child(parent_die, result) != 0) {
      return 0;
    }

    Dwarf_Die *die = result;
    do {
      switch (dwarf_tag(die)) {
      case DW_TAG_subprogram:
      case DW_TAG_inlined_subroutine:
        if (die_has_pc(die, pc)) {
          return result;
        }
      };
      bool declaration = false;
      Dwarf_Attribute attr_mem;
      dwarf_formflag(dwarf_attr(die, DW_AT_declaration, &attr_mem),
                     &declaration);
      if (!declaration) {
        // let's be curious and look deeper in the tree,
        // function are not necessarily at the first level, but
        // might be nested inside a namespace, structure etc.
        Dwarf_Die die_mem;
        Dwarf_Die *indie = find_fundie_by_pc(die, pc, &die_mem);
        if (indie) {
          *result = die_mem;
          return result;
        }
      }
    } while (dwarf_siblingof(die, result) == 0);
    return 0;
  }

  template <typename CB>
  static bool deep_first_search_by_pc(Dwarf_Die *parent_die, Dwarf_Addr pc,
                                      CB cb) {
    Dwarf_Die die_mem;
    if (dwarf_child(parent_die, &die_mem) != 0) {
      return false;
    }

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
      if (!branch_has_pc) {
        branch_has_pc = die_has_pc(die, pc);
      }
      if (branch_has_pc) {
        cb(die);
      }
    } while (dwarf_siblingof(die, &die_mem) == 0);
    return branch_has_pc;
  }

  static const char *die_call_file(Dwarf_Die *die) {
    Dwarf_Attribute attr_mem;
    Dwarf_Word file_idx = 0;

    dwarf_formudata(dwarf_attr(die, DW_AT_call_file, &attr_mem), &file_idx);

    if (file_idx == 0) {
      return 0;
    }

    Dwarf_Die die_mem;
    Dwarf_Die *cudie = dwarf_diecu(die, &die_mem, 0, 0);
    if (!cudie) {
      return 0;
    }

    Dwarf_Files *files = 0;
    size_t nfiles;
    dwarf_getsrcfiles(cudie, &files, &nfiles);
    if (!files) {
      return 0;
    }

    return dwarf_filesrc(files, file_idx, 0, 0);
  }
};

template <>
class TraceResolverImpl<system_tag::linux_>
    : public TraceResolverLinuxImpl<trace_resolver_tag::current> {};

#endif // FLOPPY_OS_LINUX

#ifdef FLOPPY_OS_DARWIN

template <typename STACKTRACE_TAG> class TraceResolverDarwinImpl;

template <>
class TraceResolverDarwinImpl<trace_resolver_tag::backtrace_symbol>
    : public TraceResolverImplBase {
public:
  void load_addresses(void *const*addresses, int address_count) override {
    if (address_count == 0) {
      return;
    }
    _symbols.reset(backtrace_symbols(addresses, address_count));
  }

  resolved_trace resolve(resolved_trace trace) override {
    // parse:
    // <n>  <file>  <addr>  <mangled-name> + <offset>
    char *filename = _symbols[trace.idx];

    // skip "<n>  "
    while (*filename && *filename != ' ')
      filename++;
    while (*filename == ' ')
      filename++;

    // find start of <mangled-name> from end (<file> may contain a space)
    char *p = filename + strlen(filename) - 1;
    // skip to start of " + <offset>"
    while (p > filename && *p != ' ')
      p--;
    while (p > filename && *p == ' ')
      p--;
    while (p > filename && *p != ' ')
      p--;
    while (p > filename && *p == ' ')
      p--;
    char *funcname_end = p + 1;

    // skip to start of "<manged-name>"
    while (p > filename && *p != ' ')
      p--;
    char *funcname = p + 1;

    // skip to start of "  <addr>  "
    while (p > filename && *p == ' ')
      p--;
    while (p > filename && *p != ' ')
      p--;
    while (p > filename && *p == ' ')
      p--;

    // skip "<file>", handling the case where it contains a
    char *filename_end = p + 1;
    if (p == filename) {
      // something went wrong, give up
      filename_end = filename + strlen(filename);
      funcname = filename_end;
    }
    trace.object_filename.assign(
        filename, filename_end); // ok even if filename_end is the ending \0
                                 // (then we assign entire string)

    if (*funcname) { // if it's not end of string
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
class TraceResolverImpl<system_tag::darwin>
    : public TraceResolverDarwinImpl<trace_resolver_tag::current> {};

#endif // FLOPPY_OS_DARWIN

#ifdef FLOPPY_OS_WINDOWS

// Load all symbol info
// Based on:
// https://stackoverflow.com/questions/6205981/windows-c-stack-trace-from-a-running-app/28276227#28276227

struct module_data {
  std::string image_name;
  std::string module_name;
  void *base_address;
  DWORD load_size;
};

class get_mod_info {
  HANDLE process;
  static const int buffer_length = 4096;

public:
  get_mod_info(HANDLE h) : process(h) {}

  module_data operator()(HMODULE module) {
    module_data ret;
    char temp[buffer_length];
    MODULEINFO mi;

    GetModuleInformation(process, module, &mi, sizeof(mi));
    ret.base_address = mi.lpBaseOfDll;
    ret.load_size = mi.SizeOfImage;

    GetModuleFileNameExA(process, module, temp, sizeof(temp));
    ret.image_name = temp;
    GetModuleBaseNameA(process, module, temp, sizeof(temp));
    ret.module_name = temp;
    std::vector<char> img(ret.image_name.begin(), ret.image_name.end());
    std::vector<char> mod(ret.module_name.begin(), ret.module_name.end());
    SymLoadModule64(process, 0, &img[0], &mod[0], (DWORD64)ret.base_address,
                    ret.load_size);
    return ret;
  }
};

template <> class TraceResolverImpl<system_tag::windows>
    : public TraceResolverImplBase {
public:
  TraceResolverImpl() {

    HANDLE process = GetCurrentProcess();

    std::vector<module_data> modules;
    DWORD cbNeeded;
    std::vector<HMODULE> module_handles(1);
    SymInitialize(process, NULL, false);
    DWORD symOptions = SymGetOptions();
    symOptions |= SYMOPT_LOAD_LINES | SYMOPT_UNDNAME;
    SymSetOptions(symOptions);
    EnumProcessModules(process, &module_handles[0],
                       static_cast<DWORD>(module_handles.size() * sizeof(HMODULE)),
		       &cbNeeded);
    module_handles.resize(cbNeeded / sizeof(HMODULE));
    EnumProcessModules(process, &module_handles[0],
                       static_cast<DWORD>(module_handles.size() * sizeof(HMODULE)),
		       &cbNeeded);
    std::transform(module_handles.begin(), module_handles.end(),
                   std::back_inserter(modules), get_mod_info(process));
    void *base = modules[0].base_address;
    IMAGE_NT_HEADERS *h = ImageNtHeader(base);
    image_type = h->FileHeader.Machine;
  }

  static const int max_sym_len = 255;
  struct symbol_t {
    SYMBOL_INFO sym;
    char buffer[max_sym_len];
  } sym;

  DWORD64 displacement;

  resolved_trace resolve(resolved_trace t) override {
    HANDLE process = GetCurrentProcess();

    char name[256];

    memset(&sym, 0, sizeof(sym));
    sym.sym.SizeOfStruct = sizeof(SYMBOL_INFO);
    sym.sym.MaxNameLen = max_sym_len;

    if (!SymFromAddr(process, (ULONG64)t.addr, &displacement, &sym.sym)) {
      // TODO:  error handling everywhere
      char* lpMsgBuf;
      DWORD dw = GetLastError();

      if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                             FORMAT_MESSAGE_FROM_SYSTEM |
                             FORMAT_MESSAGE_IGNORE_INSERTS,
                         NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                         (char*)&lpMsgBuf, 0, NULL)) {
        std::fprintf(stderr, "%s\n", lpMsgBuf);
        LocalFree(lpMsgBuf);
      }

      // abort();
    }
    UnDecorateSymbolName(sym.sym.Name, (PSTR)name, 256, UNDNAME_COMPLETE);

    DWORD offset = 0;
    IMAGEHLP_LINE line;
    if(SymGetLineFromAddr(process, (ULONG64)t.addr, &offset, &line)) {
      t.object_filename = line.FileName;
      t.source.file_name_mut() = line.FileName;
      t.source.line_mut() = line.LineNumber;
      t.source.column_mut() = offset;
    }
    t.source.function_name_mut() = name;
    t.object_filename = "";
    t.object_function = name;

    return t;
  }

  DWORD machine_type() const { return image_type; }

private:
  DWORD image_type;
};

#endif

class trace_resolver : public TraceResolverImpl<system_tag::current> {};

/*************** CODE SNIPPET ***************/

class source_file {
public:
  typedef std::vector<std::pair<unsigned, std::string> > lines_t;

  source_file() {}
  source_file(const std::string &path) {
    // 1. If BACKWARD_CXX_SOURCE_PREFIXES is set then assume it contains
    //    a colon-separated list of path prefixes.  Try prepending each
    //    to the given path until a valid file is found.
    const std::vector<std::string> &prefixes = get_paths_from_env_variable();
    for (size_t i = 0; i < prefixes.size(); ++i) {
      // Double slashes (//) should not be a problem.
      std::string new_path = prefixes[i] + '/' + path;
      _file.reset(new std::ifstream(new_path.c_str()));
      if (is_open())
        break;
    }
    // 2. If no valid file found then fallback to opening the path as-is.
    if (not _file || !is_open()) {
      _file.reset(new std::ifstream(path.c_str()));
    }
  }
  bool is_open() const { return _file->is_open(); }

  lines_t &get_lines(unsigned line_start, unsigned line_count, lines_t &lines) {
    using namespace std;
    // This function make uses of the dumbest algo ever:
    //	1) seek(0)
    //	2) read lines one by one and discard until line_start
    //	3) read line one by one until line_start + line_count
    //
    // If you are getting snippets many times from the same file, it is
    // somewhat a waste of CPU, feel free to benchmark and propose a
    // better solution ;)

    _file->clear();
    _file->seekg(0);
    string line;
    unsigned line_idx;

    for (line_idx = 1; line_idx < line_start; ++line_idx) {
      std::getline(*_file, line);
      if (!*_file) {
        return lines;
      }
    }

    // think of it like a lambda in C++98 ;)
    // but look, I will reuse it two times!
    // What a good boy am I.
    struct isspace {
      bool operator()(char c) { return std::isspace(c); }
    };

    bool started = false;
    for (; line_idx < line_start + line_count; ++line_idx) {
      getline(*_file, line);
      if (!*_file) {
        return lines;
      }
      if (!started) {
        if (std::find_if(line.begin(), line.end(), not_isspace()) == line.end())
          continue;
        started = true;
      }
      lines.push_back(make_pair(line_idx, line));
    }

    lines.erase(
        std::find_if(lines.rbegin(), lines.rend(), not_isempty()).base(),
        lines.end());
    return lines;
  }

  lines_t get_lines(unsigned line_start, unsigned line_count) {
    lines_t lines;
    return get_lines(line_start, line_count, lines);
  }

  // there is no find_if_not in C++98, lets do something crappy to
  // workaround.
  struct not_isspace {
    bool operator()(char c) { return !std::isspace(c); }
  };
  // and define this one here because C++98 is not happy with local defined
  // struct passed to template functions, fuuuu.
  struct not_isempty {
    bool operator()(const lines_t::value_type &p) {
      return !(std::find_if(p.second.begin(), p.second.end(), not_isspace()) ==
               p.second.end());
    }
  };

  void swap(source_file &b) { _file.swap(b._file); }

  source_file(source_file &&from) : _file(nullptr) { swap(from); }
  source_file &operator=(source_file &&from) {
    swap(from);
    return *this;
  }

  // Allow adding to paths gotten from BACKWARD_CXX_SOURCE_PREFIXES after loading the
  // library; this can be useful when the library is loaded when the locations are unknown
  // Warning: Because this edits the static paths variable, it is *not* intrinsiclly thread safe
  static void add_paths_to_env_variable_impl(const std::string & to_add) {
    get_mutable_paths_from_env_variable().push_back(to_add);
  }

private:
  details::handle<std::ifstream *, default_delete<std::ifstream *> >
      _file;

  static std::vector<std::string> get_paths_from_env_variable_impl() {
    std::vector<std::string> paths;
    const char *prefixes_str = std::getenv("BACKWARD_CXX_SOURCE_PREFIXES");
    if (prefixes_str && prefixes_str[0]) {
      paths = details::split_source_prefixes(prefixes_str);
    }
    return paths;
  }

  static std::vector<std::string> &get_mutable_paths_from_env_variable() {
    static volatile std::vector<std::string> paths = get_paths_from_env_variable_impl();
    return const_cast<std::vector<std::string>&>(paths);
  }

  static const std::vector<std::string> &get_paths_from_env_variable() {
    return get_mutable_paths_from_env_variable();
  }

  source_file(const source_file &) = delete;
  source_file &operator=(const source_file &) = delete;
};

class snippet_factory {
public:
  typedef source_file::lines_t lines_t;

  lines_t get_snippet(const std::string &filename, unsigned line_start,
                      unsigned context_size) {

    source_file &src_file = get_src_file(filename);
    unsigned start = line_start - context_size / 2;
    return src_file.get_lines(start, context_size);
  }

  lines_t get_combined_snippet(const std::string &filename_a, unsigned line_a,
                               const std::string &filename_b, unsigned line_b,
                               unsigned context_size) {
    source_file &src_file_a = get_src_file(filename_a);
    source_file &src_file_b = get_src_file(filename_b);

    lines_t lines =
        src_file_a.get_lines(line_a - context_size / 4, context_size / 2);
    src_file_b.get_lines(line_b - context_size / 4, context_size / 2, lines);
    return lines;
  }

  lines_t get_coalesced_snippet(const std::string &filename, unsigned line_a,
                                unsigned line_b, unsigned context_size) {
    source_file &src_file = get_src_file(filename);

    using std::max;
    using std::min;
    unsigned a = min(line_a, line_b);
    unsigned b = max(line_a, line_b);

    if ((b - a) < (context_size / 3)) {
      return src_file.get_lines((a + b - context_size + 1) / 2, context_size);
    }

    lines_t lines = src_file.get_lines(a - context_size / 4, context_size / 2);
    src_file.get_lines(b - context_size / 4, context_size / 2, lines);
    return lines;
  }

private:
  using src_files_t = std::unordered_map<std::string, source_file>;
  src_files_t _src_files;

  source_file &get_src_file(const std::string &filename) {
    src_files_t::iterator it = _src_files.find(filename);
    if (it != _src_files.end()) {
      return it->second;
    }
    source_file &new_src_file = _src_files[filename];
    new_src_file = source_file(filename);
    return new_src_file;
  }
};

class printer {
public:
  bool snippet;
  bool address;
  bool object;
  int inliner_context_size;
  int trace_context_size;
  bool reverse;

  printer()
      : snippet(true), address(false),
        object(false), inliner_context_size(5), trace_context_size(7),
        reverse(true) {}

  template <typename ST> FILE *print(ST &st, FILE *fp = stderr) {
    print_stacktrace(st, fp);
    return fp;
  }

  template <typename IT>
  FILE *print(IT begin, IT end, FILE *fp = stderr, size_t thread_id = 0) {
    print_stacktrace(begin, end, fp, thread_id);
    return fp;
  }

  trace_resolver const &resolver() const { return _resolver; }

private:
  trace_resolver _resolver;
  snippet_factory _snippets;

  template <typename ST>
  void print_stacktrace(ST &st, std::FILE* out) {
    printer::print_header(out, st.thread_id());
    _resolver.load_stacktrace(st);
    if(reverse)
      for(size_t trace_idx = st.size(); trace_idx > 0; --trace_idx)
        this->print_trace(out, _resolver.resolve(st[trace_idx - 1]));
    else
      for(size_t trace_idx = 0; trace_idx < st.size(); ++trace_idx)
        this->print_trace(out, _resolver.resolve(st[trace_idx]));
  }

  template <typename IT>
  auto print_stacktrace(IT begin, IT end, std::FILE* out, size_t thread_id) -> void {
    printer::print_header(out, thread_id);
    for (; begin != end; ++begin) {
      this->print_trace(out, *begin);
    }
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

  static auto indent(std::FILE* out, size_t idx) -> void {
    fmt::print(out, fmt::emphasis::bold | fg(fmt::terminal_color::red), "#{:<2}: ", idx);
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

  auto print_snippet(
    std::FILE* out,
    char const* indent,
    resolved_trace::source_loc_t const& source_loc,
    int context_size
  ) -> void {
    using namespace std;
    typedef snippet_factory::lines_t lines_t;

    lines_t lines = this->_snippets.get_snippet(
      std::string(source_loc.file_name()),
      source_loc.line(),
      static_cast<unsigned>(context_size)
    );

    for(lines_t::const_iterator it = lines.begin(); it != lines.end(); ++it) {
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
};

#if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)

class signal_handler {
public:
  static std::vector<int> make_default_signals() {
    const int posix_signals[] = {
      // Signals for which the default action is "Core".
      SIGABRT, // Abort signal from abort(3)
      SIGBUS,  // Bus error (bad memory access)
      SIGFPE,  // Floating point exception
      SIGILL,  // Illegal Instruction
      SIGIOT,  // IOT trap. A synonym for SIGABRT
      SIGQUIT, // Quit from keyboard
      SIGSEGV, // Invalid memory reference
      SIGSYS,  // Bad argument to routine (SVr4)
      SIGTRAP, // Trace/breakpoint trap
      SIGXCPU, // CPU time limit exceeded (4.2BSD)
      SIGXFSZ, // File size limit exceeded (4.2BSD)
#if defined(FLOPPY_OS_DARWIN)
      SIGEMT, // emulation instruction executed
#endif
    };
    return std::vector<int>(posix_signals,
                            posix_signals +
                                sizeof posix_signals / sizeof posix_signals[0]);
  }

  signal_handler(const std::vector<int> &posix_signals = make_default_signals())
      : _loaded(false) {
    bool success = true;

    const size_t stack_size = 1024 * 1024 * 8;
    _stack_content.reset(static_cast<char *>(malloc(stack_size)));
    if (_stack_content) {
      stack_t ss;
      ss.ss_sp = _stack_content.get();
      ss.ss_size = stack_size;
      ss.ss_flags = 0;
      if (sigaltstack(&ss, nullptr) < 0) {
        success = false;
      }
    } else {
      success = false;
    }

    for (size_t i = 0; i < posix_signals.size(); ++i) {
      struct sigaction action;
      memset(&action, 0, sizeof action);
      action.sa_flags =
          static_cast<int>(SA_SIGINFO | SA_ONSTACK | SA_NODEFER | SA_RESETHAND);
      sigfillset(&action.sa_mask);
      sigdelset(&action.sa_mask, posix_signals[i]);
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
      action.sa_sigaction = &sig_handler;
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

      int r = sigaction(posix_signals[i], &action, nullptr);
      if (r < 0)
        success = false;
    }

    _loaded = success;
  }

  bool loaded() const { return _loaded; }

  static void handleSignal(int, siginfo_t *info, void *_ctx) {
    ucontext_t *uctx = static_cast<ucontext_t *>(_ctx);

    stack_trace st;
    void *error_addr = nullptr;
#ifdef REG_RIP // x86_64
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext.gregs[REG_RIP]);
#elif defined(REG_EIP) // x86_32
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext.gregs[REG_EIP]);
#elif defined(__arm__)
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext.arm_pc);
#elif defined(__aarch64__)
    #if defined(__APPLE__)
      error_addr = reinterpret_cast<void *>(uctx->uc_mcontext->__ss.__pc);
    #else
      error_addr = reinterpret_cast<void *>(uctx->uc_mcontext.pc);
    #endif
#elif defined(__mips__)
    error_addr = reinterpret_cast<void *>(
        reinterpret_cast<struct sigcontext *>(&uctx->uc_mcontext)->sc_pc);
#elif defined(__ppc__) || defined(__powerpc) || defined(__powerpc__) ||        \
    defined(__POWERPC__)
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext.regs->nip);
#elif defined(__riscv)
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext.__gregs[REG_PC]);
#elif defined(__s390x__)
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext.psw.addr);
#elif defined(__APPLE__) && defined(__x86_64__)
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext->__ss.__rip);
#elif defined(__APPLE__)
    error_addr = reinterpret_cast<void *>(uctx->uc_mcontext->__ss.__eip);
#else
#warning ":/ sorry, ain't know no nothing none not of your architecture!"
#endif
    if (error_addr) {
      st.load_from(error_addr, 32, reinterpret_cast<void *>(uctx),
                   info->si_addr);
    } else {
      st.load_here(32, reinterpret_cast<void *>(uctx), info->si_addr);
    }

    printer printer_;
    printer_.address = true;
    printer_.print(st, stderr);

#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)
    psiginfo(info, nullptr);
#else
    (void)info;
#endif
  }

private:
  details::handle<char *> _stack_content;
  bool _loaded;

#ifdef __GNUC__
  __attribute__((noreturn))
#endif
  static void
  sig_handler(int signo, siginfo_t *info, void *_ctx) {
    handleSignal(signo, info, _ctx);

    // try to forward the signal.
    raise(info->si_signo);

    // terminate the process immediately.
    puts("watf? exit");
    _exit(EXIT_FAILURE);
  }
};

#endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
#ifdef FLOPPY_OS_WINDOWS
class signal_handler {
public:
 explicit(false) signal_handler(std::vector<int> const& = std::vector<int>())
      : reporter_thread_([]() {
          {
            std::unique_lock<std::mutex> lk(mtx());
            cv().wait(lk, [] { return crashed() != crash_status::running; });
          }
          if(crashed() == crash_status::crashed)
            signal_handler::handle_stacktrace(skip_recs());
          {
            std::unique_lock<std::mutex> const lk(mtx());
            signal_handler::crashed() = crash_status::ending;
          }
          cv().notify_one();
        }) {
    ::SetUnhandledExceptionFilter(signal_handler::crash_handler);

    signal(SIGABRT, signal_handler_);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

    std::set_terminate(&terminator);
    _set_purecall_handler(&terminator);
    _set_invalid_parameter_handler(&invalid_parameter_handler);
  }
  bool loaded() const { return true; }

  ~signal_handler() {
    {
      std::unique_lock<std::mutex> const lk(mtx());
      crashed() = crash_status::normal_exit;
    }
    cv().notify_one();
    reporter_thread_.join();
  }

private:
  static auto ctx() -> CONTEXT* {
    static CONTEXT data;
    return &data;
  }

  enum class crash_status : int
  {
    running,
    crashed,
    normal_exit,
    ending
  };

  static auto crashed() -> crash_status& {
    static crash_status data;
    return data;
  }

  static auto mtx() -> std::mutex& {
    static std::mutex data;
    return data;
  }

  static auto cv() -> std::condition_variable& {
    static std::condition_variable data;
    return data;
  }

  static HANDLE &thread_handle() {
    static HANDLE handle;
    return handle;
  }

  std::thread reporter_thread_;

  static const constexpr auto signal_skip_recs =
#ifdef __clang__
      4
#else
      3
#endif
      ;

  static auto skip_recs() -> int& {
    static int data;
    return data;
  }

  static inline auto terminator() -> void {
    signal_handler::crash_handler(signal_handler::signal_skip_recs);
    std::abort();
  }

  static inline auto signal_handler_([[maybe_unused]] int unused) -> void {
    signal_handler::crash_handler(signal_handler::signal_skip_recs);
    std::abort();
  }

  static inline auto __cdecl invalid_parameter_handler(
    [[maybe_unused]] wchar_t const* _unused1,
    [[maybe_unused]] wchar_t const* _unused2,
    [[maybe_unused]] wchar_t const* _unused3,
    [[maybe_unused]] unsigned int _unused4,
    [[maybe_unused]] uintptr_t _unused5
  ) -> void {
    signal_handler::crash_handler(signal_handler::signal_skip_recs);
    std::abort();
  }

  __noinline__ static auto WINAPI crash_handler(EXCEPTION_POINTERS* info) -> long {
    signal_handler::crash_handler(0, info->ContextRecord);
    return EXCEPTION_CONTINUE_SEARCH;
  }

  __noinline__ static auto crash_handler(int skip, CONTEXT* ct = nullptr) -> void {
    if(ct == nullptr)
      ::RtlCaptureContext(ctx());
    else
      std::memcpy(ctx(), ct, sizeof(CONTEXT));
    ::DuplicateHandle(
      ::GetCurrentProcess(),
      ::GetCurrentThread(),
      ::GetCurrentProcess(),
      &signal_handler::thread_handle(),
      0,
      FALSE,
      DUPLICATE_SAME_ACCESS
    );
    skip_recs() = skip;
    {
      std::unique_lock<std::mutex> const lk(mtx());
      crashed() = crash_status::crashed;
    }
    cv().notify_one();
    {
      std::unique_lock<std::mutex> lk(mtx());
      cv().wait(lk, [] { return crashed() != crash_status::crashed; });
    }
  }

  static auto handle_stacktrace(int skip_frames = 0) -> void {
    auto p = printer();
    auto st = stack_trace();
    st.set_machine_type(p.resolver().machine_type());
    st.set_thread_handle(signal_handler::thread_handle());
    st.load_here(32 + skip_frames, ctx());
    st.skip_n_firsts(skip_frames);
    p.address = true;
    p.print(st, stderr);
  }
};

#endif // FLOPPY_OS_WINDOWS
#ifdef FLOPPY_OS_UNKNOWN
class signal_handler
{
  public:
   signal_handler(std::vector<int> const& = std::vector<int>()) {}
   [[nodiscard]] auto init() -> bool { return false; }
   [[nodiscard]] auto loaded() -> bool { return false; }
};
#endif // FLOPPY_OS_UNKNOWN
} // namespace floppy::stacktrace
