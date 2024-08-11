#pragma once

#if defined(__cplusplus) && __cplusplus > 201703L
# define FL_CXX20
#elif defined(__cplusplus) && __cplusplus > 201402L
# define FL_CXX17
#else
# error "Unsupported C++ version"
#endif // __cplusplus

#if defined(FL_CXX20)
//NOLINTNEXTLINE(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __wrap_concept__(c) c

//NOLINTNEXTLINE(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __cxx20_or_cxx17__(a, b) a
#else // FL_CXX20
//NOLINTNEXTLINE(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __wrap_concept__(c) typename

//NOLINTNEXTLINE(*-reserved-identifier, *-identifier-naming, *-macro-usage)
# define __cxx20_or_cxx17__(a, b) b
#endif // FL_CXX20