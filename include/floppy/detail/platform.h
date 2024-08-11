#pragma once

#include <limits>
#include <floppy/detail/export.h>
#include <floppy/detail/types.h>
#include <floppy/detail/predefs.h>

namespace floppy
{
  namespace system_tag
  {
    struct linux_;
    struct darwin;
    struct windows;
    struct unknown;

    #if defined(FLOPPY_OS_LINUX)
    using current = linux_;
    #elif defined(FLOPPY_OS_DARWIN)
    using current = darwin;
    #elif defined(FLOPPY_OS_WINDOWS)
    using current = windows;
    #else
    using current = unknown;
    #endif
  } // namespace system_tag

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

    /// \brief Vendors enumeration.
    enum class [[maybe_unused]] vendor : u8
    {
      apple,        ///< Apple.
      pc,           ///< PC.
      scei,         ///< SCEI.
      freescale,    ///< Freescale.
      ibm,          ///< IBM.
      imagination,  ///< Imagination.
      mips,         ///< MIPS.
      nvidia,       ///< NVIDIA.
      csr,          ///< CSR.
      amd,          ///< AMD.
      mesa,         ///< Mesa.
      suse,         ///< SUSE.
      open_embedded,///< Open Embedded.
      unknown       ///< Unknown vendor.
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

    /// \brief Holds current backward path delimiter.
    char backward_path_delimiter;

    /// \brief Constructs platform information for the current platform at compile-time.
    constexpr platform(
      enum operating_system operating_system_,
      enum arch architecture_,
      enum compiler compiler_,
      enum endianness endianness_,
      i16 cxx_standard_, // NOLINT(*-easily-swappable-parameters)
      char path_separator_,
      char backward_path_delimiter_
    )
      : operating_system(operating_system_)
      , architecture(architecture_)
      , compiler(compiler_)
      , endianness(endianness_)
      , cxx_standard(cxx_standard_)
      , path_separator(path_separator_)
      , backward_path_delimiter(backward_path_delimiter_)
    {}

    /// \brief Returns platform information for the current platform at compile-time.
    /// \see floppy::current_platform
    [[nodiscard]] static constexpr auto current() noexcept -> platform
    {
      return platform {
        platform::current_system(),
        platform::current_architecture(),
        platform::current_compiler(),
        platform::current_endianness(),
        platform::current_standard(),
        platform::current_path_separator(),
        platform::current_backward_path_delimiter()
      };
    }

    /// \brief Returns platform information as target triple string.
    /// \details Target triple string is a format that is used by LLVM. For example:
    /// \code
    /// x86_64-unknown-linux-gnu
    /// arm-apple-darwin
    /// \endcode
    /// \note This is intended to be used purely for debugging or logging purposes. Do not expect it to be
    /// a stable identifier for a target triple. If you need a stable identifier, use LLVM corresponding
    /// functions.
    /// \sa https://www.flother.is/til/llvm-target-triple/
    template <typename C>
    [[nodiscard]] auto as_target_triple() const noexcept -> std::basic_string<C> {
      return fmt::format(
        "{}-{}-{}{}",
        (std::basic_stringstream<C>() << this->architecture).str(),
        (std::basic_stringstream<C>() << platform::guess_vendor(this->operating_system)).str(),
        (std::basic_stringstream<C>() << this->operating_system).str(),
        this->compiler == compiler::unknown
            or (this->operating_system != operating_system::windows
                and this->operating_system != operating_system::gnu_linux)
          ? ""
          : fmt::format("-{}", (std::basic_stringstream<C>() << this->compiler).str())
      );
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
    [[nodiscard]] static constexpr auto current_system() noexcept -> enum operating_system
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
    [[nodiscard]] static constexpr auto current_architecture() noexcept -> enum arch
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
    [[nodiscard]] static constexpr auto current_compiler() noexcept -> enum compiler
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
    [[nodiscard]] static constexpr auto current_endianness() noexcept -> enum endianness
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
    [[nodiscard]] static constexpr auto current_standard() noexcept -> i16
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
    [[nodiscard]] static constexpr auto current_path_separator() noexcept -> char {
      if constexpr(platform::current_system() == operating_system::windows)
        return '\\';
      else
        return '/';
    }

    [[nodiscard]] static constexpr auto current_backward_path_delimiter() noexcept -> char {
      if constexpr(platform::current_system() == operating_system::windows)
        return ';';
      else
        return ':';
    }

    /// \brief Tries to guess a vendor from operating system name.
    /// \note This function is not 100% reliable.
    [[nodiscard]] static constexpr auto guess_vendor(enum operating_system const os) noexcept -> enum vendor {
      switch(os) {
        case operating_system::windows: return vendor::pc;
        case operating_system::darwin: return vendor::apple;
        default: return vendor::unknown;
      }
    }
  };

  /// \brief Stream adaptor for floppy::platform::operating_system
  template <class E, class T>
  auto operator<<(std::basic_ostream<E, T>& os, enum platform::operating_system const& d) -> std::basic_ostream<E, T>& {
    switch(d) {
      case platform::operating_system::windows: os << "windows"; break;
      case platform::operating_system::gnu_linux: os << "linux"; break;
      case platform::operating_system::darwin: os << "darwin"; break;
      case platform::operating_system::android: os << "android"; break;
      case platform::operating_system::cygwin: os << "cygwin"; break;
      case platform::operating_system::freebsd: os << "freebsd"; break;
      case platform::operating_system::dragonfly: os << "dragonfly"; break;
      case platform::operating_system::netbsd: os << "netbsd"; break;
      case platform::operating_system::openbsd: os << "openbsd"; break;
      case platform::operating_system::unknown: os << "unknown"; break;
    }
    return os;
  }

  /// \brief Stream adaptor for floppy::platform::arch
  template <class E, class T>
  auto operator<<(std::basic_ostream<E, T>& os, enum platform::arch const& d) -> std::basic_ostream<E, T>& {
    switch(d) {
      case platform::arch::x86_32: os << "x86_32"; break;
      case platform::arch::x86_64: os << "x86_64"; break;
      case platform::arch::alpha: os << "alpha"; break;
      case platform::arch::arm: os << "arm"; break;
      case platform::arch::bfin: os << "bfin"; break;
      case platform::arch::convex: os << "convex"; break;
      case platform::arch::e2k: os << "e2k"; break;
      case platform::arch::ia64: os << "ia64"; break;
      case platform::arch::loongarch: os << "loongarch"; break;
      case platform::arch::m68k: os << "m68k"; break;
      case platform::arch::mips: os << "mips"; break;
      case platform::arch::hppa: os << "hppa"; break;
      case platform::arch::powerpc: os << "powerpc"; break;
      case platform::arch::cuda: os << "cuda"; break;
      case platform::arch::pyramid: os << "pyramid"; break;
      case platform::arch::riscv: os << "riscv"; break;
      case platform::arch::rs6000: os << "rs6000"; break;
      case platform::arch::sparc: os << "sparc"; break;
      case platform::arch::superh: os << "superh"; break;
      case platform::arch::s370: os << "s370"; break;
      case platform::arch::s390: os << "s390"; break;
      case platform::arch::sysz: os << "sysz"; break;
      case platform::arch::unknown: os << "unknown"; break;
    }
    return os;
  }

  /// \brief Stream adaptor for floppy::platform::compiler
  template <class E, class T>
  auto operator<<(std::basic_ostream<E, T>& os, enum platform::compiler const& d) -> std::basic_ostream<E, T>& {
    switch(d) {
      case platform::compiler::borland: os << "borland"; break;
      case platform::compiler::clang: os << "clang"; break;
      case platform::compiler::comeau: os << "comeau"; break;
      case platform::compiler::compaq: os << "compaq"; break;
      case platform::compiler::diab: os << "diab"; break;
      case platform::compiler::digitalmars: os << "digitalmars"; break;
      case platform::compiler::dignussystem: os << "dignussystem"; break;
      case platform::compiler::edg: os << "edg"; break;
      case platform::compiler::pathscale: os << "pathscale"; break;
      case platform::compiler::gcc: os << "gnu"; break;
      case platform::compiler::greenhills: os << "greenhills"; break;
      case platform::compiler::hpa: os << "hpa"; break;
      case platform::compiler::iar: os << "iar"; break;
      case platform::compiler::ibm: os << "ibm"; break;
      case platform::compiler::intel: os << "intel"; break;
      case platform::compiler::kai: os << "kai"; break;
      case platform::compiler::llvm: os << "llvm"; break;
      case platform::compiler::metaware: os << "metaware"; break;
      case platform::compiler::codewarrior: os << "codewarrior"; break;
      case platform::compiler::microtec: os << "microtec"; break;
      case platform::compiler::mpw: os << "mpw"; break;
      case platform::compiler::nvcc: os << "nvcc"; break;
      case platform::compiler::palm: os << "palm"; break;
      case platform::compiler::portlandgroup: os << "portlandgroup"; break;
      case platform::compiler::mipspro: os << "mipspro"; break;
      case platform::compiler::oracle: os << "oracle"; break;
      case platform::compiler::tendra: os << "tendra"; break;
      case platform::compiler::msvc: os << "msvc"; break;
      case platform::compiler::watcom: os << "watcom"; break;
      case platform::compiler::unknown: os << "unknown"; break;
    }
    return os;
  }

  /// \brief Stream adaptor for floppy::platform::endianness
  template <class E, class T>
  auto operator<<(std::basic_ostream<E, T>& os, enum platform::endianness const& d) -> std::basic_ostream<E, T>& {
    switch(d) {
      case platform::endianness::big: os << "big"; break;
      case platform::endianness::little: os << "little"; break;
      case platform::endianness::unknown: os << "unknown"; break;
    }
    return os;
  }

  /// \brief Stream adaptor for floppy::platform::vendor
  template <class E, class T>
  auto operator<<(std::basic_ostream<E, T>& os, enum platform::vendor const& d) -> std::basic_ostream<E, T>& {
    switch(d) {
      case platform::vendor::apple: os << "apple"; break;
      case platform::vendor::pc: os << "pc"; break;
      case platform::vendor::scei: os << "scei"; break;
      case platform::vendor::freescale: os << "freescale"; break;
      case platform::vendor::ibm: os << "ibm"; break;
      case platform::vendor::imagination: os << "imagination"; break;
      case platform::vendor::mips: os << "mips"; break;
      case platform::vendor::nvidia: os << "nvidia"; break;
      case platform::vendor::csr: os << "csr"; break;
      case platform::vendor::amd: os << "amd"; break;
      case platform::vendor::mesa: os << "mesa"; break;
      case platform::vendor::suse: os << "suse"; break;
      case platform::vendor::open_embedded: os << "open_embedded"; break;
      case platform::vendor::unknown: os << "unknown"; break;
    }
    return os;
  }

  /// \brief Stream adaptor for floppy::platform
  template <class E, class T>
  auto operator<<(std::basic_ostream<E, T>& os, platform const& d) -> std::basic_ostream<E, T>& {
    os << d.as_target_triple<char>();
    return os;
  }

  /// \brief Constant-initialized variable, containing current platform information.
  /// \headerfile floppy/floppy.h
  /// \ingroup platform
  inline auto constexpr current_platform = platform::current();
} // namespace floppy