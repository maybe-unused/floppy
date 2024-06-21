#pragma once

/// \defgroup platform Platform
/// \brief Platform information
/// \details Contains information about the operating system, architecture, language version and etc.
/// Use \ref floppy::current_platform constant to get the current platform information.
///
/// Defines macros for retrieving information about the current platform.
/// \see floppy::current_platform
/// \see floppy::platform

// os detection
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is Windows.
# define FLOPPY_OS_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is Linux.
# define FLOPPY_OS_LINUX
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__)) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is MacOS.
# define FLOPPY_OS_DARWIN
#elif defined(__ANDROID__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is Android.
# define FLOPPY_OS_ANDROID
#elif defined(__APPLE__) && defined(__MACH__) && defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is iOS.
# define FLOPPY_OS_IOS
#elif defined(__CYGWIN__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is Cygwin.
# define FLOPPY_OS_CYGWIN
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is FreeBSD.
# define FLOPPY_OS_FREEBSD
#elif defined(__DragonFly__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is DragonFly BSD.
# define FLOPPY_OS_DRAGONFLY
#elif defined(__NETBSD__) || defined(__NetBSD__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is NetBSD.
# define FLOPPY_OS_NETBSD
#elif defined(__OpenBSD__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if operating system is OpenBSD.
# define FLOPPY_OS_OPENBSD
#else
/// \ingroup platform
/// \brief Flag defined if operating system is unknown.
# define FLOPPY_OS_UNKNOWN
# warning "-- [floppy] Operating system is not detected."
#endif

// arch detection
#if defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) ||                \
  defined(__i686__) || defined(__i386) || defined(_M_IX86) || defined(_X86_) ||                    \
  defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is x86_32.
# define FLOPPY_ARCH_X86_32
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(_M_X64) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is x86_64.
# define FLOPPY_ARCH_X86_64
#elif defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
# define FLOPPY_ARCH_ALPHA
#elif defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) ||         \
  defined(_M_ARM) || defined(__arm__) || defined(__arm64) || defined(__thumb__) ||                 \
  defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__) ||                           \
  defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||               \
  defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) ||              \
  defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_5TE__) ||           \
  defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is ARM.
# define FLOPPY_ARCH_ARM
#elif defined(__bfin__) || defined(__BFIN__) || defined(bfin) || defined(BFIN) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is BF.
# define FLOPPY_ARCH_BFIN
#elif defined(__convex__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is CONVEX.
# define FLOPPY_ARCH_CONVEX
#elif defined(__e2k__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is E2K.
# define FLOPPY_ARCH_E2K
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64) || defined(__itanium__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is IA64.
# define FLOPPY_ARCH_IA64
#elif defined(__loongarch__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is LoongArch.
# define FLOPPY_ARCH_LOONGARCH
#elif defined(__m68k__) || defined(M68000) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is M68K.
# define FLOPPY_ARCH_M68K
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is MIPS.
# define FLOPPY_ARCH_MIPS
#elif defined(__hppa__) || defined(__hppa) || defined(__HPPA__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is HPPA.
# define FLOPPY_ARCH_HPPA
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) ||                      \
  defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) ||            \
  defined(__PPC64__) || defined(_M_PPC) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) ||           \
  defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON) || defined(__ppc) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is PowerPC.
# define FLOPPY_ARCH_POWERPC
#elif defined(__CUDA_ARCH__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is CUDA.
# define FLOPPY_ARCH_CUDA
#elif defined(pyr) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is Pyramid.
# define FLOPPY_ARCH_PYRAMID
#elif defined(__riscv) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is RISC-V.
# define FLOPPY_ARCH_RISCV
#elif defined(__THW_RS6000) || defined(_IBMR2) || defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is RS6000.
# define FLOPPY_ARCH_RS6000
#elif defined(__sparc__) || defined(__sparc) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is SPARC.
# define FLOPPY_ARCH_SPARC
#elif defined(__sh__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is SuperH.
# define FLOPPY_ARCH_SUPERH
#elif defined(__370__) || defined(__THW_370__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is S370.
# define FLOPPY_ARCH_SYS370
#elif defined(__s390__) || defined(__s390x__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is S390.
# define FLOPPY_ARCH_SYS390
#elif defined(__SYSC_ZARCH__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is SYSZ.
# define FLOPPY_ARCH_SYSZ
#else
/// \ingroup platform
/// \brief Flag defined if architecture is unknown.
# define FLOPPY_ARCH_UNKNOWN
# warning "-- [floppy] Architecture is not detected."
#endif

// compiler detection
#if defined(__BORLANDC__) || defined(__CODEGEARC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Borland.
# define FLOPPY_COMPILER_BORLAND
#elif defined(__clang__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Clang.
# define FLOPPY_COMPILER_CLANG
#elif defined(__COMO__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Comeau
# define FLOPPY_COMPILER_COMO
#elif defined(__DECC) || defined(__DECCXX) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Compaq
# define FLOPPY_COMPILER_COMPAQ
#elif defined(__DCC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Diab
# define FLOPPY_COMPILER_DIAB
#elif defined(__DMC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Digital
# define FLOPPY_COMPILER_DMC
#elif defined(__SYSC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Dignus
# define FLOPPY_COMPILER_DIGNUS
#elif defined(__EDG__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is EDG
# define FLOPPY_COMPILER_EDG
#elif defined(__PATHCC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is PathScale
# define FLOPPY_COMPILER_PATHSCALE
#elif defined(__GNUC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is GNU
# define FLOPPY_COMPILER_GNU
#elif defined(__ghs) || defined(__ghs__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is GreenHills
# define FLOPPY_COMPILER_GHS
#elif defined(__HP_aCC) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is HP
# define FLOPPY_COMPILER_HPA
#elif defined(__IAR_SYSTEMS_ICC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is IAR
# define FLOPPY_COMPILER_IAR
#elif defined(__IBMCPP__) || defined(__xlC__) || defined(__xlc__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is IBM
# define FLOPPY_COMPILER_IBM
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Intel
# define FLOPPY_COMPILER_INTEL
#elif defined(__KCC) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Kai
# define FLOPPY_COMPILER_KAI
#elif defined(__llvm__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is LLVM
# define FLOPPY_COMPILER_LLVM
#elif defined(__HIGHC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Metaware
# define FLOPPY_COMPILER_HIGHC
#elif defined(__MWERKS__) || defined(__CWCC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Metrowerks
# define FLOPPY_COMPILER_METROWERKS
#elif defined(_MRI) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Microtec
# define FLOPPY_COMPILER_MRI
#elif defined(__MRC__) || defined(MPW_C) || defined(MPW_CPLUS) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is MPW
# define FLOPPY_COMPILER_MPW
#elif defined(__NVCC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is NVIDIA
# define FLOPPY_COMPILER_NVIDIA
#elif defined(_PACC_VER) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Palm
# define FLOPPY_COMPILER_PACC
#elif defined(__PGI) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is PGI
# define FLOPPY_COMPILER_PGI
#elif defined(__sgi) || defined(sgi) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is MIPSpro
# define FLOPPY_COMPILER_SGI
#elif defined(__SUNPRO_CC) || defined(__SUNPRO_C) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Sun
# define FLOPPY_COMPILER_SUN
#elif defined(__TenDRA__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is tendra
# define FLOPPY_COMPILER_TENDRA
#elif defined(_MSC_VER) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Microsoft Visual C++
# define FLOPPY_COMPILER_MSVC
#elif defined(__WATCOMC__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if compiler is Watcom
# define FLOPPY_COMPILER_WATCOM
#else
/// \ingroup platform
/// \brief Flag defined if compiler is unknown
# define FLOPPY_COMPILER_UNKNOWN
# warning "-- [floppy] Compiler is not detected."
#endif

#if (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)) ||                                    \
  (defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)) || defined(__ARMEB__) ||                      \
  defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) ||       \
  defined(__MIPSEB__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is big endian
# define FLOPPY_ENDIAN_BIG
#elif (defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)) ||                                  \
  (defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)) || defined(__ARMEL__) ||                      \
  defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(__loongarch__) || defined(_MIPSEL) ||  \
  defined(__MIPSEL) || defined(__MIPSEL__) || defined(__riscv) || defined(__e2k__) || defined(DOXYGEN_GENERATING_OUTPUT)
/// \ingroup platform
/// \brief Flag defined if architecture is little endian
# define FLOPPY_ENDIAN_LITTLE
#else
/// \ingroup platform
/// \brief Flag defined if architecture is unknown
# define FLOPPY_ENDIAN_UNKNOWN
#endif

// todo: this is marked for removal
#if !defined(__cplusplus)
# define FLOPPY_LANGUAGE_C
#else
# if __cplusplus == 1
#   define FLOPPY_LANGUAGE_CXXPRE98
# elif __cplusplus <= 199811L
#   define FLOPPY_LANGUAGE_CXX98
# elif __cplusplus <= 200911L
#   define FLOPPY_LANGUAGE_CXX03
# elif __cplusplus <= 201104L
#   define FLOPPY_LANGUAGE_CXX11
# elif __cplusplus <= 201404L
#   define FLOPPY_LANGUAGE_CXX14
# elif __cplusplus <= 201704L
#   define FLOPPY_LANGUAGE_CXX17
# elif __cplusplus <= 202004L
#   define FLOPPY_LANGUAGE_CXX20
# elif __cplusplus > 202004L
#   define FLOPPY_LANGUAGE_CXX23
# else
#   define FLOPPY_LANGUAGE_UNKNOWN
# endif
#endif