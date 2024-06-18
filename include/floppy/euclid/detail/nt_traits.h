#pragma once

#include <floppy/detail/math.h>
#include <floppy/detail/concepts.h>

namespace floppy::math::detail
{
  template <typename T>
  struct default_ordering
  {
    [[nodiscard]] constexpr auto operator<=>(T const& other) const { return strong_compare(**static_cast<T const*>(this), *other); }

    template <concepts::num T2>
    [[nodiscard]] constexpr auto operator<=>(T2 const& other) const { return strong_compare(**static_cast<T const*>(this), static_cast<T::type>(other)); }
  };

  template <typename T>
  struct default_equality : public default_ordering<T>
  {
    [[nodiscard]] constexpr auto operator==(T const& other) const -> bool { return (((*static_cast<T const*>(this)) <=> other) == 0); }
    [[nodiscard]] constexpr auto operator!=(T const& other) const -> bool { return (((*static_cast<T const*>(this)) <=> other) != 0); }

    template <concepts::num T2>
    [[nodiscard]] constexpr auto operator==(T2 const& other) const -> bool { return (((*static_cast<T const*>(this)) <=> static_cast<T::type>(other)) == 0); }

    template <concepts::num T2>
    [[nodiscard]] constexpr auto operator!=(T2 const& other) const -> bool { return (((*static_cast<T const*>(this)) <=> static_cast<T::type>(other)) != 0); }
  };
} // namespace floppy::math::detail