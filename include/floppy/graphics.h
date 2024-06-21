/// \file floppy/graphics.h
/// \brief Classes and functions related to graphics and drawing.
/// \author whs31

#pragma once

#include <floppy/graphics/color.h>

/// \defgroup gfx Graphics
/// \brief Classes related to 2d graphics and drawing.
/// \details The graphics module provides classes related to 2d graphics and drawing.
/// For now, only color type is supported.
///
/// For color manipulation, these classes are provided:
/// - `floppy::graphics::color`
///
/// \invariant If **Qt Gui** is linked against the TU this header is compiled for, the conversions between
/// Qt classes (such as `QColor`) and the `floppy` types are available.

/// \defgroup gfx_details Graphics Details
/// \ingroup gfx
/// \brief Implementation details for classes and functions related to \ref gfx.