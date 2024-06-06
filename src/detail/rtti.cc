#include <floppy/detail/corefn.h>

#include <cstdlib>
#include <memory>
#include <floppy/detail/predefs.h>

#if defined(FLOPPY_COMPILER_GNU) || defined(FLOPPY_COMPILER_CLANG)
# include <cxxabi.h>
#else
# include <windows.h>
# include <DbgHelp.h>
# pragma comment(lib, "dbghelp.lib")
#endif

using std::unique_ptr;

namespace floppy::rtti
{
  auto demangle(char const* name) -> std::string
  {
    #if defined(FLOPPY_COMPILER_GNU) || defined(FLOPPY_COMPILER_CLANG)
      auto status = -4;
      const unique_ptr<char, void (*)(void*)> res { ::abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free };
      return status == 0 ? res.get() : name;
    #else
      auto constexpr max_name_length = DWORD(1024);
      char undecorated[max_name_length];
      return ::UnDecorateSymbolName(name, undecorated, max_name_length, UNDNAME_COMPLETE) ? undecorated : name;
    #endif
  }
} // namespace floppy::rtti