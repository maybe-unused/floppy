#pragma once

#include <floppy/backtrace/trace_resolver.h>
#include <floppy/backtrace/win/stack_trace.h>

namespace floppy::stacktrace::impl
{
  struct module_data
  {
    std::string image_name;
    std::string module_name;
    void* base_address;
    DWORD load_size;
  };

  class get_mod_info
  {
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
      ::SymLoadModule64(process, 0, &img[0], &mod[0], (DWORD64)ret.base_address,
                      ret.load_size);
      return ret;
    }
  };

  template <>
  class trace_resolver_impl<system_tag::windows> : public trace_resolver_impl_base
  {
   public:
    trace_resolver_impl() {
      HANDLE process = ::GetCurrentProcess();
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

    auto resolve(resolved_trace t) -> resolved_trace override {
      HANDLE process = GetCurrentProcess();

      char name[256];

      memset(&sym, 0, sizeof(sym));
      sym.sym.SizeOfStruct = sizeof(SYMBOL_INFO);
      sym.sym.MaxNameLen = max_sym_len;

      if(not ::SymFromAddr(process, (ULONG64)t.addr, &displacement, &sym.sym)) {
        char* lpMsgBuf;
        DWORD dw = ::GetLastError();
        if(::FormatMessageA(
              FORMAT_MESSAGE_ALLOCATE_BUFFER
                | FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS,
              nullptr,
              dw,
              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
              (char*)&lpMsgBuf, 0, NULL)
          ) {
          std::fprintf(stderr, "%s\n", lpMsgBuf);
          ::LocalFree(lpMsgBuf);
        }
      }
      ::UnDecorateSymbolName(sym.sym.Name, (PSTR)name, 256, UNDNAME_COMPLETE);

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

} // namespace floppy::stacktrace::impl