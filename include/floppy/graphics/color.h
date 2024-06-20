/// \file floppy/graphics/color.h
/// \brief Class representing a color.
/// \author whs31

#pragma once

#include <array>
#include <floppy/detail/math.h>
#include <floppy/detail/formatters.h>
#include <floppy/traits.h>

/// \brief Namespace with types related to graphics and drawing.
namespace floppy::gfx
{
  namespace detail
  {
    [[nodiscard]] constexpr auto hex_to_dec(char const c) -> i32 {
      return (c >= '0' and c <= '9')
        ? c - '0'
        : ((c >= 'a' and c <= 'f')
          ? c - 'a' + 10
          : (c >= 'A' and c <= 'F')
             ? c - 'A' + 10
             : throw std::invalid_argument("invalid hex character"));
    }

    template <typename T> constexpr auto min(T&& v) -> T { return std::forward<T>(v); }
    template <typename T, typename... Args>
    constexpr auto min(T const& v1, T const& v2, Args const&... args) -> T {
      return v2 < v1 ? min(v2, args...) : min(v1, args...);
    }

    template <typename T> constexpr auto max(T&& v) -> T { return std::forward<T>(v); }
    template <typename T, typename... Args>
    constexpr auto max(T const& v1, T const& v2, Args const&... args) -> T {
      return v2 > v1 ? max(v2, args...) : max(v1, args...);
    }

    template <typename T> constexpr auto abs(T const& v) -> T { return v < T(0) ? -v : v; }
    constexpr auto fmod(f32 const x, f32 const y) -> f32 { return x - y * std::floor(x / y); }

    /// \brief Converts a hex string to an rgb color.
    /// \note Assumes the string is 6 characters long.
    constexpr auto str_to_rgb(char const* str) -> std::array<u8, 3> {
      return {
        static_cast<u8>(hex_to_dec(str[0]) << 4 | hex_to_dec(str[1])),
        static_cast<u8>(hex_to_dec(str[2]) << 4 | hex_to_dec(str[3])),
        static_cast<u8>(hex_to_dec(str[4]) << 4 | hex_to_dec(str[5]))
      };
    }

    /// \brief Converts a hex string to an rgba color.
    /// \note Assumes the string is 8 characters long.
    constexpr auto str_to_rgba(char const* str) -> std::array<u8, 4> {
      return {
        static_cast<u8>(hex_to_dec(str[0]) << 4 | hex_to_dec(str[1])),
        static_cast<u8>(hex_to_dec(str[2]) << 4 | hex_to_dec(str[3])),
        static_cast<u8>(hex_to_dec(str[4]) << 4 | hex_to_dec(str[5])),
        static_cast<u8>(hex_to_dec(str[6]) << 4 | hex_to_dec(str[7]))
      };
    }
  } // namespace detail

  class color
  {
    static constexpr inline auto u8_max = static_cast<u8>(std::numeric_limits<u8>::max());
    static constexpr inline auto u8_maxf = static_cast<f32>(std::numeric_limits<u8>::max());
    static constexpr inline auto mask = static_cast<u8>(0xFF);

   public:
    /// \brief Constructs a default black opaque color.
    constexpr color()
      : r_(0.0F)
      , g_(0.0F)
      , b_(0.0F)
      , a_(1.0F)
    {}

    /// \brief Constructs an opaque color from rgb values.
    /// \param r Red component.
    /// \param g Green component.
    /// \param b Blue component.
    constexpr color(u8 r, u8 g, u8 b)
      : r_(static_cast<f32>(r) / u8_maxf)
      , g_(static_cast<f32>(g) / u8_maxf)
      , b_(static_cast<f32>(b) / u8_maxf)
      , a_(1.0F)
    {}

    /// \brief Constructs a color from rgba values.
    /// \param r Red component.
    /// \param g Green component.
    /// \param b Blue component.
    /// \param a Alpha component.
    constexpr color(u8 r, u8 g, u8 b, u8 a)
      : r_(static_cast<f32>(r) / u8_maxf)
      , g_(static_cast<f32>(g) / u8_maxf)
      , b_(static_cast<f32>(b) / u8_maxf)
      , a_(static_cast<f32>(a) / u8_maxf)
    {}

    /// \brief Constructs a color from 32-bit unsigned integer.
    /// \details The upper 8 bits are the red component,
    /// the next 8 bits are the green component, the next 8 bits are the blue component, and the
    /// last 8 bits are the alpha component.
    /// For example, the color <b>#32a852</b> is represented as <code>0x32a852FF</code> and
    /// color <b>#10101010</b> is represented as <code>0x10101010</code>.
    /// \param rgba 32-bit unsigned integer.
    constexpr explicit color(u32 rgba)
      : r_(static_cast<f32>((rgba >> 24) & mask) / u8_maxf)
      , g_(static_cast<f32>((rgba >> 16) & mask) / u8_maxf)
      , b_(static_cast<f32>((rgba >> 8) & mask) / u8_maxf)
      , a_(static_cast<f32>(rgba & mask) / u8_maxf)
    {}

    /// \brief Constructs a color from string_view.
    /// \throws std::invalid_argument if the string is not a valid color.
    /// \param s String view.
    constexpr explicit color(std::string_view s)
    {
      if(s.starts_with("#"))
        s = s.substr(1);
      if (s.size() == 6) {
        auto const rgb = detail::str_to_rgb(s.data());
        r_ = static_cast<f32>(rgb[0]) / u8_maxf;
        g_ = static_cast<f32>(rgb[1]) / u8_maxf;
        b_ = static_cast<f32>(rgb[2]) / u8_maxf;
        a_ = 1.0F;
      } else if (s.size() == 8) {
        auto const rgba = detail::str_to_rgba(s.data());
        r_ = static_cast<f32>(rgba[0]) / u8_maxf;
        g_ = static_cast<f32>(rgba[1]) / u8_maxf;
        b_ = static_cast<f32>(rgba[2]) / u8_maxf;
        a_ = static_cast<f32>(rgba[3]) / u8_maxf;
      } else {
        throw std::invalid_argument("invalid color: "  + std::string(s));
      }
    }

    [[nodiscard]] constexpr auto r() const -> f32 { return r_; }
    [[nodiscard]] constexpr auto g() const -> f32 { return g_; }
    [[nodiscard]] constexpr auto b() const -> f32 { return b_; }
    [[nodiscard]] constexpr auto a() const -> f32 { return a_; }

    [[nodiscard]] constexpr auto rgba() const -> u32 {
      return static_cast<u32>(r_ * u8_max) << 24
        | static_cast<u32>(g_ * u8_max) << 16
        | static_cast<u32>(b_ * u8_max) << 8
        | static_cast<u32>(a_ * u8_max);
    }

    [[nodiscard]] constexpr auto rgb() const -> u32 {
      return static_cast<u32>(r_ * u8_max) << 24
        | static_cast<u32>(g_ * u8_max) << 16
        | static_cast<u32>(b_ * u8_max) << 8
        | u8_max;
    }

   private:
    f32 r_;
    f32 g_;
    f32 b_;
    f32 a_;
  };

  static_assert(color(50, 168, 82).rgb() == color(0x32A852FF).rgb());
  static_assert(color("#32a852").rgb() == color(0x32A852FF).rgb());
  static_assert(color("32a852").rgb() == color(0x32A852FF).rgb());
  static_assert(color(50, 168, 82, 128).rgba() == color(0x32A85280).rgba());
  static_assert(color("#32a85280").rgba() == color(0x32A85280).rgba());
  static_assert(color("32a85280").rgba() == color(0x32A85280).rgba());
} // namespace floppy::gfx

// #32a852
// 50 168 82 rgb
// 0x32a852
// 70% 0% 51% 34% cmyk
// 136 70 66 hsv
// 136 54 43 hsl
// https://github.com/proxict/constexpr-color/blob/master/include/constexpr-color/constexpr-color.hpp