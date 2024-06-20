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

    /// \brief Converts a hex string to an RGB color.
    /// \param str Hex string in RGB format as <i>C-string</i>.
    /// \return RGB color as <code>std::array<u8, 3></code>.
    /// \note Assumes the string is 6 characters long.
    constexpr auto str_to_rgb(char const* str) -> std::array<u8, 3> {
      return {
        static_cast<u8>(hex_to_dec(str[0]) << 4 | hex_to_dec(str[1])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[2]) << 4 | hex_to_dec(str[3])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[4]) << 4 | hex_to_dec(str[5]))  // NOLINT(*-pro-bounds-pointer-arithmetic)
      };
    }

    /// \brief Converts a hex string to an RGBA color.
    /// \param str Hex string in RGBA format as <i>C-string</i>.
    /// \return RGBA color as <code>std::array<u8, 4></code>.
    /// \note Assumes the string is 8 characters long.
    constexpr auto str_to_rgba(char const* str) -> std::array<u8, 4> {
      return {
        static_cast<u8>(hex_to_dec(str[0]) << 4 | hex_to_dec(str[1])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[2]) << 4 | hex_to_dec(str[3])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[4]) << 4 | hex_to_dec(str[5])), // NOLINT(*-pro-bounds-pointer-arithmetic)
        static_cast<u8>(hex_to_dec(str[6]) << 4 | hex_to_dec(str[7]))  // NOLINT(*-pro-bounds-pointer-arithmetic)
      };
    }

    namespace hsl
    {
      [[nodiscard]] constexpr auto k(i32 const n, u16 const hue) -> f32 {
        return fmod(static_cast<f32>(n) + static_cast<f32>(hue) / 30.0F, 12.0F);
      }

      [[nodiscard]] constexpr auto a(f32 const saturation, f32 const lightness) -> f32 {
        return saturation * min(lightness, 1.0F - lightness);
      }

      [[nodiscard]] constexpr auto chroma(std::array<u8, 3> const& rgb) -> f32 {
        return static_cast<f32>(max(rgb[0], rgb[1], rgb[2]) - min(rgb[0], rgb[1], rgb[2])) / 255.0F;
      }

      [[nodiscard]] constexpr auto h(std::array<u8, 3> const& rgb) -> f32 {
        return chroma(rgb) == 0.0F
          ? 0.0F
          : (
            max(rgb[0], rgb[1], rgb[2]) == rgb[0]
              ? static_cast<f32>(rgb[1] - rgb[2]) / 255.0F / chroma(rgb) + (rgb[1] < rgb[2]
                ? 6.0F
                : 0.0F)
              : (max(rgb[0], rgb[1], rgb[2]) == rgb[1]
                ? static_cast<f32>(rgb[2] - rgb[0]) / 255.0F / chroma(rgb) + 2.0F
                : (static_cast<f32>(rgb[0] - rgb[1]) / 255.0F / chroma(rgb) + 4.0F))
          );
      }

      [[nodiscard]] constexpr auto lightness(std::array<u8, 3> const& rgb) -> f32 {
        return static_cast<f32>(min(rgb[0], rgb[1], rgb[2]) + max(rgb[0], rgb[1], rgb[2])) / 510.0F;
      }

      [[nodiscard]] constexpr auto hue(f32 const h) -> u16 {
        return static_cast<u16>(std::round(h * 60.0F) < 0.0F
          ? std::round(h * 60.0F) + 360.0F
          : std::round(h * 60.0F));
      }
    } // namespace hsl
  } // namespace detail

  /// \brief Class representing a color.
  /// \headerfile floppy/graphics.h
  class color : public traits::formattable<color, char>
  {
    template <concepts::num T = u8>
    static constexpr auto mask = static_cast<T>(std::numeric_limits<u8>::max());

   public:
    /// \brief HSL color representation.
    /// \headerfile floppy/graphics.h
    /// \sa https://en.wikipedia.org/wiki/HSL_and_HSV
    struct hsl_t : public traits::formattable<color::hsl_t, char>
    {
      u16 h = 0;     ///< Hue
      f32 s = 0.0F;  ///< Saturation
      f32 l = 0.0F;  ///< Lightness

      /// \brief Constructs an HSL color.
      /// \param h Hue
      /// \param s Saturation
      /// \param l Lightness
      constexpr hsl_t(u16 h, f32 s, f32 l)
        : h(h)
        , s(s)
        , l(l)
      {}

      /// \brief Converts the color to RGB format.
      [[nodiscard]] constexpr auto to_rgb() const -> std::array<u8, 3> {
        return std::array<u8, 3> {
          static_cast<u8>((this->l - detail::hsl::a(this->s, this->l)
            * detail::max(-1.0F, detail::min(detail::hsl::k(0, this->h) - 3.0F,
            9.0F - detail::hsl::k(0, this->h), 1.0F))) * 255.0F),
          static_cast<u8>((this->l - detail::hsl::a(this->s, this->l)
            * detail::max(-1.0F, detail::min(detail::hsl::k(8, this->h) - 3.0F,
            9.0F - detail::hsl::k(8, this->h), 1.0F))) * 255.0F),
          static_cast<u8>((this->l - detail::hsl::a(this->s, this->l)
            * detail::max(-1.0F, detail::min(detail::hsl::k(4, this->h) - 3.0F,
            9.0F - detail::hsl::k(4, this->h), 1.0F))) * 255.0F)
        };
      }

      /// \brief Constructs an HSL color from RGB values.
      /// \param r Red
      /// \param g Green
      /// \param b Blue
      [[nodiscard]] static constexpr auto from_rgb(u8 const r, u8 const g, u8 const b) -> hsl_t {
        return hsl_t {
          detail::hsl::hue(detail::hsl::h(std::array<u8, 3>{r, g, b})),
          detail::hsl::chroma(std::array<u8, 3>{r, g, b}) == 0.0F
            ? 0.0F
            : detail::hsl::chroma(std::array<u8, 3>{r, g, b}) / (1 - detail::abs(2 * detail::hsl::lightness(std::array<u8, 3>{r, g, b}) - 1)),
          detail::hsl::lightness(std::array<u8, 3>{r, g, b})
        };
      }

      /// \brief Converts the color to a string.
      /// \details Example: <code>(180 0.5 0.5)</code>.
      /// \returns The string representation of the color.
      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {})", this->h, this->s, this->l);
      }

      constexpr auto operator==(color::hsl_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->l, other.l, 3.0F);
      }

      constexpr auto operator!=(color::hsl_t const& other) const -> bool { return not (*this == other); }
    };

    /// \brief HSLA color representation.
    /// \headerfile floppy/graphics.h
    /// \sa https://en.wikipedia.org/wiki/HSL_and_HSV
    struct hsla_t : public traits::formattable<color::hsla_t, char>
    {
      u16 h = 0;     ///< Hue
      f32 s = 0.0F;  ///< Saturation
      f32 l = 0.0F;  ///< Lightness
      f32 a = 1.0F;  ///< Alpha

      /// \brief Constructs an HSLA color.
      /// \param h Hue
      /// \param s Saturation
      /// \param l Lightness
      /// \param a Alpha
      constexpr hsla_t(u16 h, f32 s, f32 l, f32 a)
        : h(h)
        , s(s)
        , l(l)
        , a(a)
      {}

      /// \brief Converts the color to RGBA format.
      [[nodiscard]] constexpr auto to_rgba() const -> std::array<u8, 4> {
        auto rgb_ = color::hsl_t { this->h, this->s, this->l }.to_rgb();
        return std::array<u8, 4> {
          static_cast<u8>(rgb_[0]),
          static_cast<u8>(rgb_[1]),
          static_cast<u8>(rgb_[2]),
          static_cast<u8>(this->a * 255.0F)
        };
      }

      /// \brief Converts the color to a string.
      /// \details Example: <code>(180 0.5 0.5 0.5)</code>.
      /// \returns The string representation of the color.
      [[nodiscard]] virtual auto to_string() const -> std::string override {
        return fmt::format("({} {} {} {})", this->h, this->s, this->l, this->a);
      }

      constexpr auto operator==(color::hsla_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->l, other.l, 3.0F)
          and math::approx_eq(this->a, other.a, 3.0F);
      }

      constexpr auto operator!=(color::hsla_t const& other) const -> bool { return not (*this == other); }
    };

    struct hsv_t
    {
      u16 h = 0.0F;
      f32 s = 0.0F;
      f32 v = 0.0F;

      constexpr auto operator==(color::hsv_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->v, other.v, 3.0F);
      }

      constexpr auto operator!=(color::hsv_t const& other) const -> bool { return not (*this == other); }
    };

    struct hsva_t
    {
      u16 h = 0.0F;
      f32 s = 0.0F;
      f32 v = 0.0F;
      f32 a = 1.0F;

      constexpr auto operator==(color::hsva_t const& other) const -> bool {
        return (this->h == other.h)
          and math::approx_eq(this->s, other.s, 3.0F)
          and math::approx_eq(this->v, other.v, 3.0F)
          and math::approx_eq(this->a, other.a, 3.0F);
      }

      constexpr auto operator!=(color::hsva_t const& other) const -> bool { return not (*this == other); }
    };

    struct cmyk_t
    {
      f32 c = 0.0F;
      f32 m = 0.0F;
      f32 y = 0.0F;
      f32 k = 0.0F;

      constexpr auto operator==(color::cmyk_t const& other) const -> bool {
        return math::approx_eq(this->c, other.c, 3.0F)
          and math::approx_eq(this->m, other.m, 3.0F)
          and math::approx_eq(this->y, other.y, 3.0F)
          and math::approx_eq(this->k, other.k, 3.0F);
      }

      constexpr auto operator!=(color::cmyk_t const& other) const -> bool { return not (*this == other); }
    };

    struct cmyka_t
    {
      f32 c = 0.0F;
      f32 m = 0.0F;
      f32 y = 0.0F;
      f32 k = 0.0F;
      f32 a = 1.0F;

      constexpr auto operator==(color::cmyka_t const& other) const -> bool {
        return math::approx_eq(this->c, other.c, 3.0F)
          and math::approx_eq(this->m, other.m, 3.0F)
          and math::approx_eq(this->y, other.y, 3.0F)
          and math::approx_eq(this->k, other.k, 3.0F)
          and math::approx_eq(this->a, other.a, 3.0F);
      }

      constexpr auto operator!=(color::cmyka_t const& other) const -> bool { return not (*this == other); }
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
    /// \note If passed integer is greater than 0xFFFFFF, the lower 24 bits are used as red component,
    /// the next 8 bits are used as green component, the next 8 bits are used as blue component, and
    /// the last 8 bits are used as alpha component.
    /// \param rgba 32-bit unsigned integer.
    constexpr explicit color(u32 rgba)
    {
      if(rgba > 0xFFFFFF) {
        this->r_ = static_cast<f32>((rgba >> 24) & mask<u8>) / mask<f32>;
        this->g_ = static_cast<f32>((rgba >> 16) & mask<u8>) / mask<f32>;
        this->b_ = static_cast<f32>((rgba >> 8) & mask<u8>) / mask<f32>;
        this->a_ = static_cast<f32>(rgba & mask<u8>) / mask<f32>;
      } else {
        this->r_ = static_cast<f32>((rgba >> 16) & mask<u8>) / mask<f32>;
        this->g_ = static_cast<f32>((rgba >> 8) & mask<u8>) / mask<f32>;
        this->b_ = static_cast<f32>(rgba & mask<u8>) / mask<f32>;
        this->a_ = 1.0F;
      }
    }

    /// \brief Constructs a color from string_view.
    /// \details The string must be in the one of the following formats:
    /// <ul>
    /// <li><code>#32a85280</code></li>
    /// <li><code>32a85280</code></li>
    /// <li><code>#32a852</code></li>
    /// <li><code>32a852</code></li>
    /// </ul>
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

    /// \brief Constructs an opaque color from HSL values.
    /// \param hsl HSL color value.
    /// \see from_hsl
    constexpr color(color::hsl_t const& hsl)
      : a_(1.0F)
    {
      auto const t = hsl.to_rgb();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
    }

    /// \brief Constructs a color from HSLA values.
    /// \param hsla HSLA color value.
    /// \see from_hsla
    constexpr color(color::hsla_t const& hsla)
    {
      auto const t = hsla.to_rgba();
      this->r_ = static_cast<f32>(t[0]) / mask<f32>;
      this->g_ = static_cast<f32>(t[1]) / mask<f32>;
      this->b_ = static_cast<f32>(t[2]) / mask<f32>;
      this->a_ = static_cast<f32>(t[3]) / mask<f32>;
    }

    /// \brief Returns the string representation of the color.
    /// \details Example: <code>#RRGGBBAA</code>.
    /// \returns The string representation of the color.
    [[nodiscard]] virtual auto to_string() const -> std::string override {
      return fmt::format("#{:02X}{:02X}{:02X}{:02X}",
        static_cast<u8>(r_ * mask<f32>),
        static_cast<u8>(g_ * mask<f32>),
        static_cast<u8>(b_ * mask<f32>),
        static_cast<u8>(a_ * mask<f32>)
      );
    }

    /// \brief Returns the red color component in range <code>0..255</code>.
    /// \returns The red color component in range <code>0..255</code>.
    /// \see redf()
    /// \see green()
    /// \see blue()
    /// \see alpha()
    /// \see rgb()
    [[nodiscard]] constexpr auto red() const -> u8 { return static_cast<u8>(this->r_ * mask<u8>); }

    /// \brief Returns the red color component in range <code>0..1</code>.
    /// \returns The red color component in range <code>0..1</code>.
    /// \see red()
    /// \see greenf()
    /// \see bluef()
    /// \see alphaf()
    /// \see rgb()
    [[nodiscard]] constexpr auto redf() const -> f32 { return this->r_; }

    /// \brief Returns the green color component in range <code>0..255</code>.
    /// \returns The green color component in range <code>0..255</code>.
    /// \see greenf()
    /// \see red()
    /// \see blue()
    /// \see alpha()
    /// \see rgb()
    [[nodiscard]] constexpr auto green() const -> u8 { return static_cast<u8>(this->g_ * mask<u8>); }

    /// \brief Returns the green color component in range <code>0..1</code>.
    /// \returns The green color component in range <code>0..1</code>.
    /// \see green()
    /// \see redf()
    /// \see bluef()
    /// \see alphaf()
    /// \see rgb()
    [[nodiscard]] constexpr auto greenf() const -> f32 { return this->g_; }

    /// \brief Returns the blue color component in range <code>0..255</code>.
    /// \returns The blue color component in range <code>0..255</code>.
    /// \see bluef()
    /// \see red()
    /// \see green()
    /// \see alpha()
    /// \see rgb()
    [[nodiscard]] constexpr auto blue() const -> u8 { return static_cast<u8>(this->b_ * mask<u8>); }

    /// \brief Returns the blue color component in range <code>0..1</code>.
    /// \returns The blue color component in range <code>0..1</code>.
    /// \see blue()
    /// \see redf()
    /// \see greenf()
    /// \see alphaf()
    /// \see rgb()
    [[nodiscard]] constexpr auto bluef() const -> f32 { return this->b_; }

    /// \brief Returns the alpha color component in range <code>0..255</code>.
    /// \returns The alpha color component in range <code>0..255</code>.
    /// \see alphaf()
    [[nodiscard]] constexpr auto alpha() const -> u8 { return static_cast<u8>(this->a_ * mask<u8>); }

    /// \brief Returns the alpha color component in range <code>0..1</code>.
    /// \returns The alpha color component in range <code>0..1</code>.
    /// \see alpha()
    [[nodiscard]] constexpr auto alphaf() const -> f32 { return a_; }

    /// \brief Returns the hue color component (HSL/HSV) in range <code>0..255</code>.
    /// \returns The hue color component (HSL/HSV) in range <code>0..255</code>.
    /// \see huef()
    [[nodiscard]] constexpr auto hue() const -> u8 { return static_cast<u8>(this->hsl().h * mask<u8>); }

    /// \brief Returns the hue color component (HSL/HSV) in range <code>0..1</code>.
    /// \returns The hue color component (HSL/HSV) in range <code>0..1</code>.
    /// \see hue()
    [[nodiscard]] constexpr auto huef() const -> f32 { return this->hsl().h; }

    /// \brief Returns the lightness color component (HSL) in range <code>0..255</code>.
    /// \returns The lightness color component (HSL) in range <code>0..255</code>.
    /// \see lightnessf()
    /// \see hsl()
    [[nodiscard]] constexpr auto lightness() const -> u8 { return static_cast<u8>(this->hsl().l * mask<u8>); }

    /// \brief Returns the lightness color component (HSL) in range <code>0..1</code>.
    /// \returns The lightness color component (HSL) in range <code>0..1</code>.
    /// \see lightness()
    /// \see hsl()
    [[nodiscard]] constexpr auto lightnessf() const -> f32 { return this->hsl().l; }

    /// \brief Returns the saturation color component (HSL) in range <code>0..255</code>.
    /// \returns The saturation color component (HSL) in range <code>0..255</code>.
    /// \note This saturation is not the same as the HSV saturation. Use explicit \ref hsv() function to access it.
    /// \see saturationf()
    /// \see hsl()
    [[nodiscard]] constexpr auto saturation() const -> u8 { return static_cast<u8>(this->hsl().s * mask<u8>); }

    /// \brief Returns the saturation color component (HSL) in range <code>0..1</code>.
    /// \returns The saturation color component (HSL) in range <code>0..1</code>.
    /// \note This saturation is not the same as the HSV saturation. Use explicit \ref hsv() function to access it.
    /// \see saturation()
    /// \see hsl()
    [[nodiscard]] constexpr auto saturationf() const -> f32 { return this->hsl().s; }

    /// \brief Returns RGB color components as 32-bit unsigned integer.
    /// \details If you need the individual color components, use \ref red(), \ref green(), \ref blue(), \ref alpha() instead.
    /// \returns RGB color components as 32-bit unsigned integer.
    /// \see rgba()
    /// \see red()
    /// \see green()
    /// \see blue()
    /// \see alpha()
    [[nodiscard]] constexpr auto rgb() const -> u32 {
      return static_cast<u32>(r_ * mask<u8>) << 24
        | static_cast<u32>(g_ * mask<u8>) << 16
        | static_cast<u32>(b_ * mask<u8>) << 8
        | mask<u8>;
    }

    /// \brief Returns RGBA color components as 32-bit unsigned integer.
    /// \details If you need the individual color components, use \ref red(), \ref green(), \ref blue(), \ref alpha() instead.
    /// \returns RGBA color components as 32-bit unsigned integer.
    /// \see rgb()
    /// \see red()
    /// \see green()
    /// \see blue()
    /// \see alpha()
    [[nodiscard]] constexpr auto rgba() const -> u32 {
      return static_cast<u32>(r_ * mask<u8>) << 24
        | static_cast<u32>(g_ * mask<u8>) << 16
        | static_cast<u32>(b_ * mask<u8>) << 8
        | static_cast<u32>(a_ * mask<u8>);
    }

    /// \brief Returns HSL color components as <tt>color::hsl_t</tt> type.
    /// \details If you need the individual color components, use \ref hue(), \ref saturation(), \ref lightness() instead.
    /// \returns HSL color components as <tt>color::hsl_t</tt> type.
    /// \see hsla()
    /// \see hsv()
    /// \see cmyk()
    /// \see rgb()
    [[nodiscard]] constexpr auto hsl() const -> color::hsl_t {
      return color::hsl_t::from_rgb(this->red(), this->green(), this->blue());
    }

    /// \brief Returns HSLA color components as <tt>color::hsla_t</tt> type.
    /// \details If you need the individual color components, use \ref hue(), \ref saturation(), \ref lightness(), \ref alpha() instead.
    /// \returns HSLA color components as <tt>color::hsla_t</tt> type.
    /// \see hsl()
    /// \see hsva()
    /// \see cmyka()
    /// \see rgba()
    [[nodiscard]] constexpr auto hsla() const -> color::hsla_t {
      auto const hsl = this->hsl();
      return {
        hsl.h,
        hsl.s,
        hsl.l,
        this->alphaf()
      };
    }

    // todo: hsv
    // todo: cmyk

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Returns the color as <tt>QColor</tt> object.
    /// \returns The color as <tt>QColor</tt> object.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    /// \see from_qcolor()
    /// \sa https://doc.qt.io/qt-6/qcolor.html
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

    /// \brief Constructs an color from RGBA values in the range [0.0, 1.0].
    /// \param r Red component.
    /// \param g Green component.
    /// \param b Blue component.
    /// \param a Alpha component.
    [[nodiscard]] static constexpr auto from_floats(f32 r, f32 g, f32 b, f32 a) -> color {
      return color(
        r * mask<f32>,
        g * mask<f32>,
        b * mask<f32>,
        a * mask<f32>);
    }

    /// \brief Constructs an opaque color from HSL values.
    /// \param hsl HSL color value.
    [[nodiscard]] static constexpr auto from_hsl(color::hsl_t const& hsl) -> color {
      return color(hsl);
    }

    /// \brief Constructs a color from HSLA values.
    /// \param hsla HSLA color value.
    [[nodiscard]] static constexpr auto from_hsla(color::hsla_t const& hsla) -> color {
      return color(hsla);
    }

  #if defined(FL_QT_GUI) || defined(FL_DOC)
    /// \brief Constructs a color from a <b>QColor</b> class instance.
    /// \param col <b>QColor</b> class instance.
    /// \remarks This function is only available if <b>Qt Gui</b> is linked against the TU this header is compiled for.
    /// \sa https://doc.qt.io/qt-5/qcolor.html
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