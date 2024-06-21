#pragma once

#include <limits>
#include <floppy/detail/export.h>
#include <floppy/detail/types.h>
#include <floppy/detail/predefs.h>

namespace floppy
{
  /// \brief Platform information.
  /// \headerfile floppy/floppy.h
  /// \ingroup platform
  /// \details Contains information about the operating system, architecture, language version and etc.
  /// Use \ref platform::current function or \ref floppy::current_platform constant to get the current platform information.
  /// \see platform
  /// \see platform::current
  /// \see floppy::current_platform
  class platform
  {
   public:
    /// \brief Operating systems supported by the library.
    enum class [[maybe_unused]] operating_system : u8
    {
      windows,           ///< Microsoft Windows. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_WINDOWS</tt></i>.
      gnu_linux,         ///< GNU/Linux. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_LINUX</tt></i>.
      darwin,            ///< Apple macOS. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_DARWIN</tt></i>.
      android,           ///< Android. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_ANDROID</tt></i>.
      cygwin,            ///< Cygwin environment. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_CYGWIN</tt></i>.
      freebsd,           ///< FreeBSD. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_FREEBSD</tt></i>.
      dragonfly,         ///< DragonFly BSD. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_DRAGONFLY</tt></i>.
      netbsd,            ///< NetBSD. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_NETBSD</tt></i>.
      openbsd,           ///< OpenBSD. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_OPENBSD</tt></i>.
      unknown,           ///< Unknown OS - unable to determine. <i>Corresponds to C preprocessor macro <tt>FLOPPY_OS_UNKNOWN</tt></i>.
      linux_ = gnu_linux,///< Alias for GNU/Linux.
      macos = darwin     ///< Alias for Apple macOS.
    };

    /// \brief Architectures supported by the library.
    enum class [[maybe_unused]] arch : u8
    {
      x86_32,       ///< 32-bit x86. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_X86_32</tt></i>.
      x86_64,       ///< 64-bit x86. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_X86_64</tt></i>.
      alpha,        ///< Alpha. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_ALPHA</tt></i>.
      arm,          ///< ARM (any). <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_ARM</tt></i>.
      bfin,         ///< Blackfin. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_BFIN</tt></i>.
      convex,       ///< Convex. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_CONVEX</tt></i>.
      e2k,          ///< Elbrus 2k. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_E2K</tt></i>.
      ia64,         ///< 64-bit Itanium. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_IA64</tt></i>.
      loongarch,    ///< LoongArch. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_LOONGARCH</tt></i>.
      m68k,         ///< 68k. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_M68K</tt></i>.
      mips,         ///< Mips. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_MIPS</tt></i>.
      hppa,         ///< HP-PA. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_HPPA</tt></i>.
      powerpc,      ///< PowerPC. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_POWERPC</tt></i>.
      cuda,         ///< CUDA. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_CUDA</tt></i>.
      pyramid,      ///< Pyramid. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_PYRAMID</tt></i>.
      riscv,        ///< RISC-V. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_RISCV</tt></i>.
      rs6000,       ///< RS6000. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_RS6000</tt></i>.
      sparc,        ///< Sparc. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_SPARC</tt></i>.
      superh,       ///< SuperH. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_SUPERH</tt></i>.
      s370,         ///< 370. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_SYS370</tt></i>.
      s390,         ///< 390. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_S390</tt></i>.
      sysz,         ///< SystemZ. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_SYSZ</tt></i>.
      unknown       ///< Unknown architecture - unable to determine. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ARCH_UNKNOWN</tt></i>.
    };

    /// \brief Compilers supported by the library.
    enum class [[maybe_unused]] compiler : u8
    {
      borland,       ///< Borland C++ Builder. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_BORLAND</tt></i>.
      clang,         ///< Clang. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_CLANG</tt></i>.
      comeau,        ///< Comeau C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_COMO</tt></i>.
      compaq,        ///< Compaq C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_COMPAQ</tt></i>.
      diab,          ///< Diab C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_DIAB</tt></i>.
      digitalmars,   ///< Digital Mars. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_DMC</tt></i>.
      dignussystem,  ///< Dignus Systems C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_DIGNUS</tt></i>.
      edg,           ///< EDG C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_EDG</tt></i>.
      pathscale,     ///< PathScale C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_PATHSCALE</tt></i>.
      gcc,           ///< GNU GCC. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_GNU</tt></i>.
      greenhills,    ///< Greenhills C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_GREENHILLS</tt></i>.
      hpa,           ///< Hewlett-Packard C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_HPA</tt></i>.
      iar,           ///< IAR C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_IAR</tt></i>.
      ibm,           ///< IBM C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_IBM</tt></i>.
      intel,         ///< Intel C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_INTEL</tt></i>.
      kai,           ///< Kai C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_KAI</tt></i>.
      llvm,          ///< LLVM C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_LLVM</tt></i>.
      metaware,      ///< Metaware C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_HIGHC</tt></i>.
      codewarrior,   ///< CodeWarrior C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_METROWERKS</tt></i>.
      microtec,      ///< Microtec C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_MRI</tt></i>.
      mpw,           ///< MPW C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_MPW</tt></i>.
      nvcc,          ///< NVIDIA C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_NVIDIA</tt></i>.
      palm,          ///< Palm C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_PACC</tt></i>.
      portlandgroup, ///< Portland Group C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_PGI</tt></i>.
      mipspro,       ///< MIPSpro C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_SGI</tt></i>.
      oracle,        ///< Oracle C++ (Sun). <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_SUN</tt></i>.
      tendra,        ///< Tendra C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_TENDRA</tt></i>.
      msvc,          ///< Microsoft Visual C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_MSVC</tt></i>.
      watcom,        ///< Watcom C++. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_WATCOM</tt></i>.
      unknown        ///< Unknown compiler - unable to determine. <i>Corresponds to C preprocessor macro <tt>FLOPPY_COMPILER_UNKNOWN</tt></i>.
    };

    /// \brief Endiannesses enumeration.
    enum class [[maybe_unused]] endianness : u8
    {
      little,     ///< Little-endian. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ENDIAN_LITTLE</tt></i>.
      big,        ///< Big-endian. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ENDIAN_BIG</tt></i>.
      unknown     ///< Unknown endianness - unable to determine. <i>Corresponds to C preprocessor macro <tt>FLOPPY_ENDIAN_UNKNOWN</tt></i>.
    };

    /// \brief Holds current operating system.
    operating_system operating_system;

    /// \brief Holds current architecture.
    arch architecture;

    /// \brief Holds current compiler.
    compiler compiler;

    /// \brief Holds current endianness.
    /// \bug Can often be unknown in current library version. I'm working on it.
    endianness endianness;

    /// \brief Holds current C++ standard.
    i16 cxx_standard;

    /// \brief Holds current path separator.
    char path_separator;

    /// \brief Constructs platform information for the current platform at compile-time.
    constexpr platform(
      enum operating_system operating_system_,
      enum arch architecture_,
      enum compiler compiler_,
      enum endianness endianness_,
      i16 cxx_standard_, // NOLINT(*-easily-swappable-parameters)
      char path_separator_
    )
      : operating_system(operating_system_)
      , architecture(architecture_)
      , compiler(compiler_)
      , endianness(endianness_)
      , cxx_standard(cxx_standard_)
      , path_separator(path_separator_)
    {}

    /// \brief Returns platform information for the current platform at compile-time.
    /// \see floppy::current_platform
    [[nodiscard]] static consteval auto current() noexcept -> platform
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
    [[nodiscard]] static constexpr auto swap_endian(T u) noexcept -> T {
      static_assert(std::numeric_limits<unsigned char>::digits == 8, "unsigned char is not 8 bits"); // NOLINT(*-magic-numbers)
      union {
        T u;
        u8 c[sizeof(T)]; // NOLINT(*-avoid-c-arrays)
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
            return -3; // NOLINT(*-magic-numbers)
      #elif defined(FLOPPY_LANGUAGE_CXX98)
            return -2; // NOLINT(*-magic-numbers)
      #elif defined(FLOPPY_LANGUAGE_CXX03)
            return 3; // NOLINT(*-magic-numbers)
      #elif defined(FLOPPY_LANGUAGE_CXX11)
            return 11; // NOLINT(*-magic-numbers)
      #elif defined(FLOPPY_LANGUAGE_CXX14)
            return 14; // NOLINT(*-magic-numbers)
      #elif defined(FLOPPY_LANGUAGE_CXX17)
            return 17; // NOLINT(*-magic-numbers)
      #elif defined(FLOPPY_LANGUAGE_CXX20)
            return 20; // NOLINT(*-magic-numbers)
      #elif defined(FLOPPY_LANGUAGE_CXX23)
            return 23; // NOLINT(*-magic-numbers)
      #else
            return -1; // NOLINT(*-magic-numbers)
      #endif
    }

    /// \brief Returns constant-evaluated file system path separator character on the current platform.
    /// \details Returns <tt>'/'</tt> for POSIX and <tt>'\\'</tt> for Windows.
    /// \return File system path separator character.
    [[nodiscard]] static consteval auto current_path_separator() noexcept -> char {
      if constexpr(platform::current_system() == operating_system::windows)
        return '\\';
      else
        return '/';
    }
  };

  /// \brief Constant-initialized variable, containing current platform information.
  /// \headerfile floppy/floppy.h
  /// \ingroup platform
  inline auto constexpr current_platform = platform::current();
} // namespace floppy