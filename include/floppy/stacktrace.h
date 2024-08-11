#pragma once

#include <cctype>
#include <cstdlib>
#include <csignal>
#include <new>
#include <sstream>
#include <streambuf>
#include <exception>
#include <floppy/backtrace/trace_resolver_tag.h>
#include <floppy/backtrace/trace.h>
#include <floppy/backtrace/resolved_trace.h>
#include <floppy/backtrace/snippet_factory.h>
#include <floppy/backtrace/stack_trace.h>
#include <floppy/backtrace/printer.h>
#if defined(FLOPPY_OS_WINDOWS)
# include <condition_variable>
# include <mutex>
# include <thread>
#endif // defined(FLOPPY_OS_WINDOWS)

/// \brief Stacktrace namespace.
/// \details Currently implemented using reworked <a href="https://github.com/bombela/backward-cpp">backward-cpp</a> library.
/// \ingroup foundation
/// \sa https://github.com/bombela/backward-cpp
namespace floppy::stacktrace
{
#if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)

class signal_handler
{
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

    impl::stack_trace st;
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

    impl::printer printer_;
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
    auto p = impl::printer();
    auto st = impl::stack_trace();
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
