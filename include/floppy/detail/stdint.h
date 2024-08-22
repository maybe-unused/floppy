#pragma once

#include <cstddef>
#include <cstdint>

namespace floppy
{
  inline namespace types
  {
    using u8 = uint8_t;                       ///< 8-bit unsigned integer
    using u16 = uint16_t;                     ///< 16-bit unsigned integer
    using u32 = uint32_t;                     ///< 32-bit unsigned integer
    using u64 = uint64_t;                     ///< 64-bit unsigned integer
    using i8 = int8_t;                        ///< 8-bit signed integer
    using i16 = int16_t;                      ///< 16-bit signed integer
    using i32 = int32_t;                      ///< 32-bit signed integer
    using i64 = int64_t;                      ///< 64-bit signed integer
    using usize = size_t;                     ///< Unsigned integer with pointer size
    using isize = std::make_signed_t<size_t>; ///< Signed integer with pointer size
    using f32 = float;                        ///< Float with 32-bit precision
    using f64 = double;                       ///< Float with 64-bit precision
    using f128 [[maybe_unused]] = long double;///< Float with 128-bit precision
    using uptr = uintptr_t;                   ///< Unsigned integer with pointer size

    //NOLINTNEXTLINE(*-runtime-int)
    using ulonglong [[maybe_unused]] = unsigned long long; ///< Alias for <tt>unsigned long long</tt>.

    //NOLINTNEXTLINE(*-runtime-int)
    using uint [[maybe_unused]] = unsigned int; ///< Alias for <tt>unsigned int</tt>.
  } // namespace types

  /// \brief Inline namespace for literal operators.
  /// \headerfile floppy/floppy.h
  /// \ingroup aliases
  inline namespace literals
  {
    //#if !defined(__cpp_size_t_suffix)
    /// \brief Literal operator for <b>signed version</b> of <tt>size_t</tt>/\ref types::isize.
    /// \details Since version <b>1.2.1</b> is always available.
    /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
    constexpr isize operator""_z(ulonglong value) { return static_cast<isize>(value); }

    /// \brief Literal operator for <b>signed version</b> of <tt>size_t</tt>/\ref types::isize.
    /// \details Since version <b>1.2.1</b> is always available.
    /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
    constexpr isize operator""_Z(ulonglong value) { return static_cast<isize>(value); }

    /// \brief Literal operator for <tt>size_t</tt>/\ref types::usize.
    /// \details Since version <b>1.2.1</b> is always available.
    /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
    constexpr usize operator""_zu(ulonglong value) { return static_cast<usize>(value); }

    /// \brief Literal operator for <tt>size_t</tt>/\ref types::usize.
    /// \details Since version <b>1.2.1</b> is always available.
    /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
    constexpr usize operator""_ZU(ulonglong value) { return static_cast<usize>(value); }

    /// \brief Literal operator for <tt>size_t</tt>/\ref types::usize.
    /// \version 1.4.0
    constexpr usize operator""_usize(ulonglong value) { return static_cast<usize>(value); }

    /// \brief Literal operator for <tt>ssize_t</tt>/\ref types::isize.
    /// \version 1.4.0
    constexpr isize operator""_isize(ulonglong value) { return static_cast<isize>(value); }
    //#endif // defined(__cpp_size_t_suffix)

    /// \brief Literal operator for \ref types::u8.
    constexpr u8 operator""_u8(ulonglong value) { return static_cast<u8>(value); }

    /// \brief Literal operator for \ref types::u16.
    constexpr u16 operator""_u16(ulonglong value) { return static_cast<u16>(value); }

    /// \brief Literal operator for \ref types::u32.
    constexpr u32 operator""_u32(ulonglong value) { return static_cast<u32>(value); }

    /// \brief Literal operator for \ref types::u64.
    constexpr u64 operator""_u64(ulonglong value) { return static_cast<u64>(value); }

    /// \brief Literal operator for \ref types::i8.
    constexpr i8 operator""_i8(ulonglong value) { return static_cast<i8>(value); }

    /// \brief Literal operator for \ref types::i16.
    constexpr i16 operator""_i16(ulonglong value) { return static_cast<i16>(value); }

    /// \brief Literal operator for \ref types::i32.
    constexpr i32 operator""_i32(ulonglong value) { return static_cast<i32>(value); }

    /// \brief Literal operator for \ref types::i64.
    constexpr i64 operator""_i64(ulonglong value) { return static_cast<i64>(value); }

    /// \brief Literal operator for <tt>std::ptrdiff_t</tt>.
    constexpr std::ptrdiff_t operator""_ptrdiff(ulonglong value) { return static_cast<std::ptrdiff_t>(value); }

    // NOLINTBEGIN(*-pro-type-reinterpret-cast, *-no-int-to-ptr)

    /// \brief Literal operator for <tt>void*</tt>.
    /// \note This operator is not constexpr.
    inline void* operator""_pvoid(unsigned long long value) { return reinterpret_cast<void*>(value); }

    // NOLINTEND(*-pro-type-reinterpret-cast, *-no-int-to-ptr)
  } // namespace literals
} // namespace floppy