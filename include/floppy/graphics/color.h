/// \file floppy/graphics/color.h
/// \brief Class representing a color.
/// \author whs31

#pragma once

#include <array>
#include <floppy/detail/math.h>
#include <floppy/detail/formatters.h>
#include <floppy/traits.h>

#if defined(FL_QT_GUI) || defined(FL_DOC)
#  include <qcolor.h>
#endif


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
    template <concepts::num T = u8>
    static constexpr auto mask = static_cast<T>(std::numeric_limits<u8>::max());

   public:
    struct hsl
    {
      f32 h = 0.0F;
      f32 s = 0.0F;
      f32 l = 0.0F;
    };

    struct hsla
    {
      f32 h = 0.0F;
      f32 s = 0.0F;
      f32 l = 0.0F;
      f32 a = 1.0F;
    };

    struct hsv
    {
      f32 h = 0.0F;
      f32 s = 0.0F;
      f32 v = 0.0F;
    };

    struct hsva
    {
      f32 h = 0.0F;
      f32 s = 0.0F;
      f32 v = 0.0F;
      f32 a = 1.0F;
    };

    struct cmyk
    {
      f32 c = 0.0F;
      f32 m = 0.0F;
      f32 y = 0.0F;
      f32 k = 0.0F;
    };

    struct cmyka
    {
      f32 c = 0.0F;
      f32 m = 0.0F;
      f32 y = 0.0F;
      f32 k = 0.0F;
      f32 a = 1.0F;
    };

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
      : r_(static_cast<f32>(r) / mask<f32>)
      , g_(static_cast<f32>(g) / mask<f32>)
      , b_(static_cast<f32>(b) / mask<f32>)
      , a_(1.0F)
    {}

    /// \brief Constructs a color from rgba values.
    /// \param r Red component.
    /// \param g Green component.
    /// \param b Blue component.
    /// \param a Alpha component.
    constexpr color(u8 r, u8 g, u8 b, u8 a)
      : r_(static_cast<f32>(r) / mask<f32>)
      , g_(static_cast<f32>(g) / mask<f32>)
      , b_(static_cast<f32>(b) / mask<f32>)
      , a_(static_cast<f32>(a) / mask<f32>)
    {}

    /// \brief Constructs a color from 32-bit unsigned integer.
    /// \details The upper 8 bits are the red component,
    /// the next 8 bits are the green component, the next 8 bits are the blue component, and the
    /// last 8 bits are the alpha component.
    /// For example, the color <b>#32a852</b> is represented as <code>0x32a852FF</code> and
    /// color <b>#10101010</b> is represented as <code>0x10101010</code>.
    /// \param rgba 32-bit unsigned integer.
    constexpr explicit color(u32 rgba)
      : r_(static_cast<f32>((rgba >> 24) & mask<u8>) / mask<f32>)
      , g_(static_cast<f32>((rgba >> 16) & mask<u8>) / mask<f32>)
      , b_(static_cast<f32>((rgba >> 8) & mask<u8>) / mask<f32>)
      , a_(static_cast<f32>(rgba & mask<u8>) / mask<f32>)
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
        this->r_ = static_cast<f32>(rgb[0]) / mask<f32>;
        this->g_ = static_cast<f32>(rgb[1]) / mask<f32>;
        this->b_ = static_cast<f32>(rgb[2]) / mask<f32>;
        this->a_ = 1.0F;
      } else if (s.size() == 8) {
        auto const rgba = detail::str_to_rgba(s.data());
        this->r_ = static_cast<f32>(rgba[0]) / mask<f32>;
        this->g_ = static_cast<f32>(rgba[1]) / mask<f32>;
        this->b_ = static_cast<f32>(rgba[2]) / mask<f32>;
        this->a_ = static_cast<f32>(rgba[3]) / mask<f32>;
      } else {
        throw std::invalid_argument("invalid color: "  + std::string(s));
      }
    }

    [[nodiscard]] constexpr auto redf() const -> f32 { return r_; }
    [[nodiscard]] constexpr auto greenf() const -> f32 { return g_; }
    [[nodiscard]] constexpr auto bluef() const -> f32 { return b_; }
    [[nodiscard]] constexpr auto alphaf() const -> f32 { return a_; }
    [[nodiscard]] constexpr auto red() const -> u8 { return static_cast<u8>(r_ * mask<u8>); }
    [[nodiscard]] constexpr auto green() const -> u8 { return static_cast<u8>(g_ * mask<u8>); }
    [[nodiscard]] constexpr auto blue() const -> u8 { return static_cast<u8>(b_ * mask<u8>); }
    [[nodiscard]] constexpr auto alpha() const -> u8 { return static_cast<u8>(a_ * mask<u8>); }

    [[nodiscard]] constexpr auto rgb() const -> u32 {
      return static_cast<u32>(r_ * mask<u8>) << 24
        | static_cast<u32>(g_ * mask<u8>) << 16
        | static_cast<u32>(b_ * mask<u8>) << 8
        | mask<u8>;
    }

    [[nodiscard]] constexpr auto rgba() const -> u32 {
      return static_cast<u32>(r_ * mask<u8>) << 24
        | static_cast<u32>(g_ * mask<u8>) << 16
        | static_cast<u32>(b_ * mask<u8>) << 8
        | static_cast<u32>(a_ * mask<u8>);
    }

    [[nodiscard]] constexpr auto hsl() const -> color::hsl {
      auto constexpr k = 3.0F;
      auto max = std::max(std::max(this->redf(), this->greenf()), this->bluef());
      auto min = std::min(std::min(this->redf(), this->greenf()), this->bluef());
      auto l = (max + min) / 2.0F;
      auto const delta = max - min;
      if(math::approx_eq(max, min, k))
        return {
          .h = 0.0F,
          .s = 0.0F,
          .l = l
        };
      auto s = l < 0.5F ? delta / (max + min) : delta / (2.0F - max - min);
      auto const r2 = (((max - this->redf()) / 6.0F) + (delta / 2.0F)) / delta;
      auto const g2 = (((max - this->greenf()) / 6.0F) + (delta / 2.0F)) / delta;
      auto const b2 = (((max - this->bluef()) / 6.0F) + (delta / 2.0F)) / delta;
      auto h = 0.0F;
      if(math::approx_eq(max, this->redf(), k))
        h = b2 - g2;
      else if(math::approx_eq(max, this->greenf(), k))
        h = (1.0F / k) + r2 - b2;
      else
        h = (2.0F / k) + g2 - r2;
      if(h < 0.0F)
        h += 1.0F;
      if(h > 1.0F)
        h -= 1.0F;
      auto const h_deg = std::round(h * 360.0F * 100.0F) / 100.0F;
      return {
        .h = h_deg,
        .s = s,
        .l = l
      };
    }

    [[nodiscard]] constexpr auto hsla() const -> color::hsla {
      auto const hsl = this->hsl();
      return {
        .h = hsl.h,
        .s = hsl.s,
        .l = hsl.l,
        .a = this->alphaf()
      };
    }

    // todo: hsv
    // todo: cmyk

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    [[nodiscard]] constexpr auto to_qcolor() const -> QColor {
      return QColor(
        this->red(),
        this->green(),
        this->blue(),
        this->alpha()
      );
    }
  #endif


    [[nodiscard]] constexpr auto operator==(color const& other) const -> bool {
      return math::approx_eq(this->r_, other.r_, 3.0F)
        and math::approx_eq(this->g_, other.g_, 3.0F)
        and math::approx_eq(this->b_, other.b_, 3.0F)
        and math::approx_eq(this->a_, other.a_, 3.0F);
    }

    [[nodiscard]] constexpr auto operator!=(color const& other) const -> bool { return not (*this == other); }

    [[nodiscard]] constexpr auto operator+(color const& other) const -> color {
      return color(this->r_ + other.r_, this->g_ + other.g_, this->b_ + other.b_, this->a_ + other.a_);
    }

    [[nodiscard]] constexpr auto operator-(color const& other) const -> color {
      return color(this->r_ - other.r_, this->g_ - other.g_, this->b_ - other.b_, this->a_ - other.a_);
    }

    [[nodiscard]] constexpr auto operator*(color const& other) const -> color {
      return color(this->r_ * other.r_, this->g_ * other.g_, this->b_ * other.b_, this->a_ * other.a_);
    }

    [[nodiscard]] constexpr auto operator/(color const& other) const -> color {
      return color(this->r_ / other.r_, this->g_ / other.g_, this->b_ / other.b_, this->a_ / other.a_);
    }

    [[nodiscard]] constexpr auto operator-() const -> color {
      return color(-this->r_, -this->g_, -this->b_, -this->a_);
    }

    [[nodiscard]] constexpr auto operator*=(color const& other) -> color& {
      this->r_ *= other.r_;
      this->g_ *= other.g_;
      this->b_ *= other.b_;
      this->a_ *= other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator/=(color const& other) -> color& {
      this->r_ /= other.r_;
      this->g_ /= other.g_;
      this->b_ /= other.b_;
      this->a_ /= other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator+=(color const& other) -> color& {
      this->r_ += other.r_;
      this->g_ += other.g_;
      this->b_ += other.b_;
      this->a_ += other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator-=(color const& other) -> color& {
      this->r_ -= other.r_;
      this->g_ -= other.g_;
      this->b_ -= other.b_;
      this->a_ -= other.a_;
      return *this;
    }

    [[nodiscard]] constexpr auto operator*=(f32 other) -> color& {
      this->r_ *= other;
      this->g_ *= other;
      this->b_ *= other;
      this->a_ *= other;
      return *this;
    }

    [[nodiscard]] constexpr auto operator/=(f32 other) -> color& {
      this->r_ /= other;
      this->g_ /= other;
      this->b_ /= other;
      this->a_ /= other;
      return *this;
    }

    [[nodiscard]] static constexpr auto from_floats(f32 r, f32 g, f32 b, f32 a) -> color {
      return color(
        r * mask<f32>,
        g * mask<f32>,
        b * mask<f32>,
        a * mask<f32>);
    }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    [[nodiscard]] static constexpr auto from_qcolor(QColor const& col) -> color {
      return from_floats(
        static_cast<f32>(col.redF()),
        static_cast<f32>(col.greenF()),
        static_cast<f32>(col.blueF()),
        static_cast<f32>(col.alphaF())
      );
    }
  #endif

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
  static_assert(color::from_floats(0.1966F, 0.659F, 0.322F, 0.505F).rgba() == color("32a85280").rgba());
} // namespace floppy::gfx

// #32a852
// 50 168 82 rgb
// 0x32a852
// 70% 0% 51% 34% cmyk
// 136 70 66 hsv
// 136 54 43 hsl
// https://github.com/proxict/constexpr-color/blob/master/include/constexpr-color/constexpr-color.hpp