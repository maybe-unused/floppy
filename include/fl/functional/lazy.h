#pragma once

#include <utility>
#include <type_traits>
#include <optional>
#include "../concepts/standard_copyable_and_movable.h"
#include "../traits/noncopyable.h"

namespace fl
{
  namespace detail
  {
    template <std::invocable F>
    requires concepts::standard_copyable_and_movable<F>
      and (not std::is_reference_v<F>)
      and (not std::is_const_v<F>)
    class lazy : noncopyable // NOLINT(*-special-member-functions)
    {
     public:
      using result_type = std::invoke_result_t<F>;

      explicit lazy(F&& f)
        : f_(std::move(f))
      {}

      explicit lazy(F const& f)
        : f_(f)
      {}

      ~lazy() = default;
      lazy& operator=(lazy&&) noexcept = delete;
      lazy(lazy&& m) noexcept
        : value_(std::move(m.value_))
        , f_(std::move(m.f_))
      {}

      result_type const& operator()() const {
        this->eval();
        return *this->value_;
      }

      result_type& operator()() {
        this->eval();
        return *this->value_;
      }

     private:
      void eval() const {
        if(not this->value_)
          this->value_ = std::invoke(this->f_);
      }

      mutable std::optional<result_type> value_;
      mutable F f_;
    };
  } // namespace detail

  template <typename F>
  auto lazy(F&& f) {
    return detail::lazy<std::remove_cvref_t<F>>(std::forward<F>(f));
  }
} // namespace fl