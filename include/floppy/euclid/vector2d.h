/// \file floppy/euclid/vector2d.h
/// \brief A two-dimensional vector tagged with a unit.
/// \author whs31

#pragma once

#include <functional>
#include <floppy/euclid/length.h>
#include <floppy/euclid/size2d.h>
#include <floppy/euclid/detail/nt_traits2d.h>

#if defined(FL_QT_GUI)
# include <qvector2d.h>
#endif

namespace floppy::math
{
  /// \brief A two-dimensional vector tagged with a unit.
  /// \headerfile floppy/euclid.h
  /// \tparam U Associated unit of measurement. Default is \ref default_unit.
  /// \tparam T Number type. Must satisfy concept <tt>floppy::concepts::num</tt>. Default is \c f32.
  /// \see floppy::math::length
  /// \see floppy::math::size2d
  /// \see floppy::math::point2d
  template <typename U = default_unit, concepts::num T = f32>
  class vector2d : public detail::basic_two_dimensional_type<vector2d<U, T>, U, T>
  {
   public:
    /// \brief Associated unit type.
    using unit = U;
  };
} // namespace floppy::math