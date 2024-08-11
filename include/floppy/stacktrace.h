#pragma once

#include <cctype>
#include <cstdlib>
#include <csignal>
#include <new>
#include <streambuf>
#include <exception>
#include <floppy/backtrace/trace_resolver_tag.h>
#include <floppy/backtrace/trace.h>
#include <floppy/backtrace/resolved_trace.h>
#include <floppy/backtrace/snippet_factory.h>
#include <floppy/backtrace/stack_trace.h>
#include <floppy/backtrace/printer.h>
#include <floppy/backtrace/file.h>
#include <floppy/traits.h>
#if defined(FLOPPY_OS_WINDOWS)
# include <condition_variable>
# include <mutex>
# include <thread>
#else // defined(FLOPPY_OS_WINDOWS)
# include <floppy/backtrace/unix/default_signals.h>
#endif // defined(FLOPPY_OS_WINDOWS)

/// \brief Stacktrace namespace.
/// \details Currently implemented using reworked <a href="https://github.com/bombela/backward-cpp">backward-cpp</a> library.
/// \ingroup foundation
/// \sa https://github.com/bombela/backward-cpp
namespace floppy::stacktrace
{
  enum class crash_behavior
  {
    print_to_console,
    print_to_file,
    print_all
  };

  template <auto Behavior>
  requires std::is_same_v<decltype(Behavior), crash_behavior>
  class signal_watcher : pin
  {
   public:
    explicit signal_watcher(
      [[maybe_unused]] std::vector<int> const& posix_signals =
       #if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
        impl::make_default_signals()
       #else // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
        std::vector<int>()
       #endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
    )
      #if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
       : _loaded(false)
      #elif defined(FLOPPY_OS_WINDOWS)
       : reporter_thread_([]() {
         {
           std::unique_lock<std::mutex> lk(mtx());
           cv().wait(lk, [] { return signal_watcher::crashed() != crash_status::running; });
         }
         if(crashed() == crash_status::crashed)
           signal_watcher::handle_stacktrace(skip_recs());
         {
           std::unique_lock<std::mutex> const lk(mtx());
           signal_watcher::crashed() = crash_status::ending;
         }
         cv().notify_one();
       })
      #endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
   {
    #if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
      bool success = true;
      const size_t stack_size = 1024 * 1024 * 8;
      _stack_content.reset(static_cast<char *>(malloc(stack_size)));
      if(_stack_content) {
        stack_t ss;
        ss.ss_sp = _stack_content.get();
        ss.ss_size = stack_size;
        ss.ss_flags = 0;
        if(sigaltstack(&ss, nullptr) < 0)
          success = false;
      } else
        success = false;
      for(size_t i = 0; i < posix_signals.size(); ++i) {
        struct sigaction action;
        memset(&action, 0, sizeof action);
        action.sa_flags =
          static_cast<int>(SA_SIGINFO | SA_ONSTACK | SA_NODEFER | SA_RESETHAND);
        sigfillset(&action.sa_mask);
        sigdelset(&action.sa_mask, posix_signals[i]);
       #if defined(__clang__)
       # pragma clang diagnostic push
       # pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
       #endif // __clang__
        action.sa_sigaction = &sig_handler;
       #if defined(__clang__)
       # pragma clang diagnostic pop
       #endif // __clang__
        int r = sigaction(posix_signals[i], &action, nullptr);
        if (r < 0)
          success = false;
      }
      this->_loaded = success;
    #elif defined(FLOPPY_OS_WINDOWS)
      ::SetUnhandledExceptionFilter(signal_watcher::crash_handler);

      signal(SIGABRT, signal_watcher_);
      _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

      std::set_terminate(&terminator);
      _set_purecall_handler(&terminator);
      _set_invalid_parameter_handler(&invalid_parameter_handler);
    #endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
   }

   ~signal_watcher() {
    #if defined(FLOPPY_OS_WINDOWS)
     {
       std::unique_lock<std::mutex> const lk(mtx());
       crashed() = crash_status::normal_exit;
     }
     cv().notify_one();
     reporter_thread_.join();
    #endif // FLOPPY_OS_WINDOWS
   }

   [[nodiscard]] auto init() const -> bool {
    #if defined(FLOPPY_OS_UNKNOWN)
       return false;
    #else // FLOPPY_OS_UNKNOWN
       return true;
    #endif // FLOPPY_OS_UNKNOWN
   }

   [[nodiscard]] auto loaded() const -> bool {
    #if defined(FLOPPY_OS_UNKNOWN)
         return false;
    #elif defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
         return this->_loaded;
    #else // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
         return true;
    #endif // FLOPPY_OS_UNKNOWN
   }

  #if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
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
    # warning ":/ sorry, ain't know no nothing none not of your architecture!"
    #endif
     if (error_addr)
       st.load_from(error_addr, 32, reinterpret_cast<void *>(uctx), info->si_addr);
     else
       st.load_here(32, reinterpret_cast<void *>(uctx), info->si_addr);

     impl::printer printer_;
     printer_.address = true;
     if constexpr(Behavior == crash_behavior::print_to_console or Behavior == crash_behavior::print_all)
       printer_.print(st, stderr);
     if constexpr(Behavior == crash_behavior::print_to_file or Behavior == crash_behavior::print_all)
       printer_.print(st, impl::make_crash_report_path());

    #if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)
     psiginfo(info, nullptr);
    #else
     (void)info;
    #endif
   }
  #endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN

  private:
  #if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
   details::handle<char*> _stack_content;
   bool _loaded;
  #elif defined(FLOPPY_OS_WINDOWS)
   std::thread reporter_thread_;
  #endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN

  #if defined(FLOPPY_OS_LINUX) || defined(FLOPPY_OS_DARWIN)
   [[noreturn]] static void sig_handler(int signo, siginfo_t *info, void *_ctx) {
     handleSignal(signo, info, _ctx);
     raise(info->si_signo);
     puts("forcing exit");
     _exit(EXIT_FAILURE);
   }
  #elif defined(FLOPPY_OS_WINDOWS)
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


   static const constexpr auto signal_skip_recs =
      #ifdef __clang__
        4
      #else // __clang__
        3
      #endif // __clang__
        ;

   static auto skip_recs() -> int& {
     static int data;
     return data;
   }

   static inline auto terminator() -> void {
     signal_watcher::crash_handler(signal_watcher::signal_skip_recs);
     std::abort();
   }

   static inline auto signal_watcher_([[maybe_unused]] int unused) -> void {
     signal_watcher::crash_handler(signal_watcher::signal_skip_recs);
     std::abort();
   }

   static inline auto __cdecl invalid_parameter_handler(
     [[maybe_unused]] wchar_t const* _unused1,
     [[maybe_unused]] wchar_t const* _unused2,
     [[maybe_unused]] wchar_t const* _unused3,
     [[maybe_unused]] unsigned int _unused4,
     [[maybe_unused]] uintptr_t _unused5
   ) -> void {
     signal_watcher::crash_handler(signal_watcher::signal_skip_recs);
     std::abort();
   }

   __noinline__ static auto WINAPI crash_handler(EXCEPTION_POINTERS* info) -> long {
     signal_watcher::crash_handler(0, info->ContextRecord);
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
       &signal_watcher::thread_handle(),
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
     st.set_thread_handle(signal_watcher::thread_handle());
     st.load_here(32 + skip_frames, ctx());
     st.skip_n_firsts(skip_frames);
     p.address = true;

     if constexpr(Behavior == crash_behavior::print_to_console or Behavior == crash_behavior::print_all)
       p.print(st, stderr);
     if constexpr(Behavior == crash_behavior::print_to_file or Behavior == crash_behavior::print_all)
       p.print(st, impl::make_crash_report_path());
   }
  #endif // FLOPPY_OS_LINUX || FLOPPY_OS_DARWIN
  };
} // namespace floppy::stacktrace
