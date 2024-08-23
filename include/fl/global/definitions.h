#pragma once

#include "platform_definitions.h"

#if defined(DOXYGEN_GENERATING_OUTPUT)
/**
 * @ingroup macros
 * @brief Flag defined if <b>Doxygen</b> documentation is being generated.
 */
# define FL_DOC
#endif // DOXYGEN_GENERATING_OUTPUT

#if defined(FL_DOC)
/**
 * @ingroup macros
 * @brief Flag defined if <b>Qt::Core</b> library is available and linked.
 */
# define FL_QT_CORE

/**
 * @ingroup macros
 * @brief Flag defined if <b>Qt::Gui</b> library is available and linked.
 */
# define FL_QT_GUI

/**
 * @ingroup macros
 * @brief Flag defined if debug mode is <b>enabled</b>.
 * @details Enables some additional checks, majority of contract functionality and some assertions.
 * @see FL_NO_DEBUG
 */
# define FL_DEBUG

/**
 * @ingroup macros
 * @brief Flag defined if debug mode is <b>disabled</b>.
 * @details Disables some additional checks, majority of contract functionality and some assertions.
 * @see FL_DEBUG
 */
# define FL_NO_DEBUG

/**
 * @ingroup macros
 * @brief Attribute macro intended for <b>no inlining</b> functions.
 * @details Disables inlining for the function.
 *
 * Example usage:
 *
 * @code {.cpp}
 *  ___noinline___ void func() { ... } // func will not be inlined
 * @endcode
 * @see ___inline___
 */
# define ___noinline___

/**
 * @ingroup macros
 * @brief Attribute macro intended for <b>force inlining</b> functions.
 * @details Forces inlining for the function.
 *
 * Example usage:
 *
 * @code {.cpp}
 *  ___inline___ inline void func() { ... } // func will be inlined, if possible
 * @endcode
 *
 * In some cases, inlining will not be possible, for example, when code contains taking the address of a such function:
 *
 * @code {.cpp}
 *  ___inline___ inline void func() { ... } // func will not be inlined
 *  void* ptr = &func;
 *  ...
 *  func();
 * @endcode
 * @see ___noinline___
 */
# define ___inline___
#else // DOXYGEN_GENERATING_OUTPUT
# if defined(QT_CORE_LIB) || __has_include("qtglobal.h") || __has_include("qcoreapplication.h") || defined(DOXYGEN_GENERATING_OUTPUT)
#  define FL_QT_CORE
# endif
# if defined(QT_GUI_LIB) || __has_include("qpainter.h") || __has_include("qguiapplication.h") || defined(DOXYGEN_GENERATING_OUTPUT)
#  define FL_QT_GUI
# endif
# if defined(NDEBUG) || defined(QT_NO_DEBUG)
#  define FL_NO_DEBUG
# else
#  define FL_DEBUG
# endif
// NOLINTBEGIN(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# if defined(FL_COMPILER_MSVC)
#   define ___noinline___ __declspec(noinline)
#   define ___inline___ __forceinline
# else
#   define ___noinline___ __attribute__((noinline))
#   define ___inline___ __attribute__((always_inline))
# endif
# if defined(FL_DEBUG)
#   define ___fl_debug_only___(...) __VA_ARGS__
#   define ___fl_release_consteval___
#   define ___fl_release_unused___
# else
#   define ___fl_debug_only___(...)
#   define ___fl_release_consteval___ consteval
#   define ___fl_release_unused___ [[maybe_unused]]
# endif
# define ___fl_stringify_impl___(x) #x
# define ___fl_stringify___(x) ___fl_stringify_impl___(x)
// NOLINTEND(*-reserved-identifier, *-identifier-naming, *-macro-usage)
#endif // FL_DOC