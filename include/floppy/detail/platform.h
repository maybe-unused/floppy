#pragma once

#include <limits>
#include <floppy/detail/export.h>
#include <floppy/detail/types.h>
#include <floppy/detail/predefs.h>

namespace floppy
{
  class platform
  {
   public:
    /// \brief Operating systems supported by the library.
    enum class [[maybe_unused]] operating_system
    {
      windows,           ///< Microsoft Windows
      gnu_linux,         ///< GNU/Linux
      darwin,            ///< Apple macOS
      android,           ///< Android
      cygwin,            ///< Cygwin
      freebsd,           ///< FreeBSD
      dragonfly,         ///< DragonFly BSD
      netbsd,            ///< NetBSD
      openbsd,           ///< OpenBSD
      unknown,           ///< Unknown OS - unable to determine
      linux_ = gnu_linux,///< Alias for GNU/Linux
      macos = darwin     ///< Alias for Apple macOS
    };

    /// \brief Architectures supported by the library.
    enum class [[maybe_unused]] arch
    {
      x86_32,       ///< 32-bit x86
      x86_64,       ///< 64-bit x86
      alpha,        ///< Alpha
      arm,          ///< ARM (any)
      bfin,         ///< Blackfin
      convex,       ///< Convex
      e2k,          ///< Elbrus 2k
      ia64,         ///< 64-bit Itanium
      loongarch,    ///< LoongArch
      m68k,         ///< 68k
      mips,         ///< Mips
      hppa,         ///< HP-PA
      powerpc,      ///< PowerPC
      cuda,         ///< CUDA
      pyramid,      ///< Pyramid
      riscv,        ///< RISC-V
      rs6000,       ///< RS6000
      sparc,        ///< Sparc
      superh,       ///< SuperH
      s370,         ///< 370
      s390,         ///< 390
      sysz,         ///< SystemZ
      unknown       ///< Unknown architecture - unable to determine
    };

    /// \brief Compilers supported by the library.
    enum class [[maybe_unused]] compiler
    {
      borland,       ///< Borland C++ Builder
      clang,         ///< Clang
      comeau,        ///< Comeau C++
      compaq,        ///< Compaq C++
      diab,          ///< Diab C++
      digitalmars,   ///< Digital Mars
      dignussystem,  ///< Dignus Systems C++
      edg,           ///< EDG C++
      pathscale,     ///< PathScale C++
      gcc,           ///< GNU GCC
      greenhills,    ///< Greenhills C++
      hpa,           ///< Hewlett-Packard C++
      iar,           ///< IAR C++
      ibm,           ///< IBM C++
      intel,         ///< Intel C++
      kai,           ///< Kai C++
      llvm,          ///< LLVM C++
      metaware,      ///< Metaware C++
      codewarrior,   ///< CodeWarrior C++
      microtec,      ///< Microtec C++
      mpw,           ///< MPW C++
      nvcc,          ///< NVIDIA C++
      palm,          ///< Palm C++
      portlandgroup, ///< Portland Group C++
      mipspro,       ///< MIPSpro C++
      oracle,        ///< Oracle C++ (Sun)
      tendra,        ///< Tendra C++
      msvc,          ///< Microsoft Visual C++
      watcom,        ///< Watcom C++
      unknown        ///< Unknown compiler - unable to determine
    };

    /// \brief Endiannesses enumeration.
    enum class [[maybe_unused]] endianness
    {
      little,     ///< Little-endian
      big,        ///< Big-endian
      unknown     ///< Unknown endianness - unable to determine
    };

    operating_system operating_system;
    arch architecture;
    compiler compiler;
    endianness endianness;
    i16 cxx_standard;
    char path_separator;

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunknown-pragmas"
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "ConstantParameter"
    constexpr inline platform(
      enum operating_system operating_system_,
      enum arch architecture_,
      enum compiler compiler_,
      enum endianness endianness_,
      i16 cxx_standard_,
      char path_separator_
    )
      : operating_system(operating_system_)
      , architecture(architecture_)
      , compiler(compiler_)
      , endianness(endianness_)
      , cxx_standard(cxx_standard_)
      , path_separator(path_separator_)
    {}
//#pragma clang diagnostic pop
//#pragma GCC diagnostic pop

    /// \brief Returns platform information for the current platform at compile-time.
    /// \see floppy::platform::current
    [[nodiscard]] static consteval inline auto current() noexcept -> platform
    {
      return platform {
        platform::current_system(),   platform::current_architecture(),
        platform::current_compiler(), platform::current_endianness(),
        platform::current_standard(), platform::current_path_separator(),
      };
    }

    /// \brief Swaps endianness of the given value.
    /// \tparam T Value type.
    /// \param u Value.
    /// \return Value with swapped endianness.
    template <typename T>
    [[nodiscard]] static constexpr auto swap_endian(T u) -> T {
      static_assert(std::numeric_limits<unsigned char>::digits == 8, "unsigned char is not 8 bits");
      union {
        T u;
        u8 c[sizeof(T)];
      } source {}, dest {};
      source.u = u;
      for(auto k = 0_ZU; k < sizeof(T); k++)
        dest.c[k] = source.c[sizeof(T) - k - 1];
      return dest.u;
    }

   private:
    /// \brief Returns constant-evaluated current operating system enumeration.
    [[nodiscard]] static consteval auto current_system() noexcept -> enum operating_system
    {
      #if defined(FLOPPY_OS_WINDOWS)
            return operating_system::windows;
      #elif defined(FLOPPY_OS_LINUX)
            return operating_system::gnu_linux;
      #elif defined(FLOPPY_OS_DARWIN)
            return operating_system::darwin;
      #elif defined(FLOPPY_OS_ANDROID)
            return operating_system::android;
      #elif defined(FLOPPY_OS_IOS)
            return operating_system::ios;
      #elif defined(FLOPPY_OS_CYGWIN)
            return operating_system::cygwin;
      #elif defined(FLOPPY_OS_FREEBSD)
            return operating_system::freebsd;
      #elif defined(FLOPPY_OS_DRAGONFLY)
            return operating_system::dragonfly;
      #elif defined(FLOPPY_OS_NETBSD)
            return operating_system::netbsd;
      #elif defined(FLOPPY_OS_OPENBSD)
            return operating_system::openbsd;
      #else
            return operating_system::unknown;
      #endif
    }

    /// \brief Returns constant-evaluated current architecture enumeration.
    [[nodiscard]] static consteval auto current_architecture() noexcept -> enum arch
    {
      #if defined(FLOPPY_ARCH_X86_32)
            return arch::x86_32;
      #elif defined(FLOPPY_ARCH_X86_64)
            return arch::x86_64;
      #elif defined(FLOPPY_ARCH_ALPHA)
            return arch::alpha;
      #elif defined(FLOPPY_ARCH_ARM)
            return arch::arm;
      #elif defined(FLOPPY_ARCH_BFIN)
            return arch::bfin;
      #elif defined(FLOPPY_ARCH_CONVEX)
            return arch::convex;
      #elif defined(FLOPPY_ARCH_E2K)
            return arch::e2k;
      #elif defined(FLOPPY_ARCH_IA64)
            return arch::ia64;
      #elif defined(FLOPPY_ARCH_LOONGARCH)
            return arch::loongarch;
      #elif defined(FLOPPY_ARCH_M68K)
            return arch::m68k;
      #elif defined(FLOPPY_ARCH_MIPS)
            return arch::mips;
      #elif defined(FLOPPY_ARCH_HPPA)
            return arch::hppa;
      #elif defined(FLOPPY_ARCH_POWERPC)
            return arch::powerpc;
      #elif defined(FLOPPY_ARCH_CUDA)
            return arch::cuda;
      #elif defined(FLOPPY_ARCH_PYRAMID)
            return arch::pyramid;
      #elif defined(FLOPPY_ARCH_RISCV)
            return arch::riscv;
      #elif defined(FLOPPY_ARCH_RS6000)
            return arch::rs6000;
      #elif defined(FLOPPY_ARCH_SPARC)
            return arch::sparc;
      #elif defined(FLOPPY_ARCH_SUPERH)
            return arch::superh;
      #elif defined(FLOPPY_ARCH_S370)
            return arch::s370;
      #elif defined(FLOPPY_ARCH_S390)
            return arch::s390;
      #elif defined(FLOPPY_ARCH_SYSZ)
            return arch::sysz;
      #else
            return arch::unknown;
      #endif
    };

    /// \brief Returns constant-evaluated current compiler enumeration.
    [[nodiscard]] static consteval auto current_compiler() noexcept -> enum compiler
    {
      #if defined(FLOPPY_COMPILER_BORLAND)
            return compiler::borland;
      #elif defined(FLOPPY_COMPILER_CLANG)
            return compiler::clang;
      #elif defined(FLOPPY_COMPILER_COMO)
            return compiler::comeau;
      #elif defined(FLOPPY_COMPILER_COMPAQ)
            return compiler::compaq;
      #elif defined(FLOPPY_COMPILER_DIAB)
            return compiler::diab;
      #elif defined(FLOPPY_COMPILER_DMC)
            return compiler::digitalmars;
      #elif defined(FLOPPY_COMPILER_DIGNUS)
            return compiler::dignussystem;
      #elif defined(FLOPPY_COMPILER_EDG)
            return compiler::edg;
      #elif defined(FLOPPY_COMPILER_PATHSCALE)
            return compiler::pathscale;
      #elif defined(FLOPPY_COMPILER_GNU)
            return compiler::gcc;
      #elif defined(FLOPPY_COMPILER_GHS)
            return compiler::greenhills;
      #elif defined(FLOPPY_COMPILER_HPA)
            return compiler::hpa;
      #elif defined(FLOPPY_COMPILER_IAR)
            return compiler::iar;
      #elif defined(FLOPPY_COMPILER_IBM)
            return compiler::ibm;
      #elif defined(FLOPPY_COMPILER_INTEL)
            return compiler::intel;
      #elif defined(FLOPPY_COMPILER_KAI)
            return compiler::kai;
      #elif defined(FLOPPY_COMPILER_LLVM)
            return compiler::llvm;
      #elif defined(FLOPPY_COMPILER_HIGHC)
            return compiler::metaware;
      #elif defined(FLOPPY_COMPILER_METROWERKS)
            return compiler::codewarrior;
      #elif defined(FLOPPY_COMPILER_MRI)
            return compiler::microtec;
      #elif defined(FLOPPY_COMPILER_MPW)
            return compiler::mpw;
      #elif defined(FLOPPY_COMPILER_NVIDIA)
            return compiler::nvcc;
      #elif defined(FLOPPY_COMPILER_PACC)
            return compiler::palm;
      #elif defined(FLOPPY_COMPILER_PGI)
            return compiler::portlandgroup;
      #elif defined(FLOPPY_COMPILER_MIPSPRO)
            return compiler::mipspro;
      #elif defined(FLOPPY_COMPILER_SUN)
            return compiler::oracle;
      #elif defined(FLOPPY_COMPILER_TENDRA)
            return compiler::tendra;
      #elif defined(FLOPPY_COMPILER_MSVC)
            return compiler::msvc;
      #elif defined(FLOPPY_COMPILER_WATCOM)
            return compiler::watcom;
      #else
            return compiler::unknown;
      #endif
    }

    /// \brief Returns constant-evaluated current endianness enumeration.
    [[nodiscard]] static consteval auto current_endianness() noexcept -> enum endianness
    {
      #if defined(FLOPPY_ENDIAN_LITTLE)
            return endianness::little;
      #elif defined(FLOPPY_ENDIAN_BIG)
            return endianness::big;
      #else
            return endianness::unknown;
      #endif
    }

    /// \brief Returns constant-evaluated current language standard.
    [[nodiscard]] static consteval auto current_standard() noexcept -> i16
    {
      #if defined(FLOPPY_LANGUAGE_C)
            return std::numeric_limits<i16>::min();
      #elif defined(FLOPPY_LANGUAGE_CXXPRE98)
            return -3;
      #elif defined(FLOPPY_LANGUAGE_CXX98)
            return -2;
      #elif defined(FLOPPY_LANGUAGE_CXX03)
            return 3;
      #elif defined(FLOPPY_LANGUAGE_CXX11)
            return 11;
      #elif defined(FLOPPY_LANGUAGE_CXX14)
            return 14;
      #elif defined(FLOPPY_LANGUAGE_CXX17)
            return 17;
      #elif defined(FLOPPY_LANGUAGE_CXX20)
            return 20;
      #elif defined(FLOPPY_LANGUAGE_CXX23)
            return 23;
      #else
            return -1;
      #endif
    }

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunknown-pragmas"
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "ConstantFunctionResult"
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "Simplify"
    /// \brief Returns constant-evaluated file system path separator character on the current platform.
    /// \details Returns <tt>'/'</tt> for POSIX and <tt>'\\'</tt> for Windows.
    /// \return File system path separator character.
    [[nodiscard]] static consteval auto current_path_separator() noexcept -> char {
      if constexpr(platform::current_system() == operating_system::windows)
        return '\\';
      else
        return '/';
    }
//#pragma clang diagnostic pop
//#pragma clang diagnostic pop
//#pragma GCC diagnostic pop
  };

  /// \brief Constant-initialized variable, containing current platform information.
  inline auto constexpr current_platform = platform::current();
} // namespace floppy::platform