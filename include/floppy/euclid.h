/// \file floppy/euclid.h
/// \brief Small collection of classes and functions for geometric types with a focus on 2d graphics and layout.
/// \author whs31

#pragma once

#include <floppy/euclid/angle.h>
#include <floppy/euclid/length.h>
#include <floppy/euclid/scale.h>
#include <floppy/euclid/size2d.h>
#include <floppy/euclid/point2d.h>
#include <floppy/euclid/vector2d.h>

/// \defgroup geometry Geometry
/// \brief Classes for geometric types with a focus on 2d graphics and layout.
/// \details The geometry module provides classes for geometric types with a focus on 2d graphics and layout.
///
/// This module provides optional measurement unit system. All types below support this system.
/// By default, types use the default unit `floppy::math::default_unit`. See \ref floppy::math::scale for
/// tutorial on implementing your own units and converting between them.
///
/// The following classes for measuring lengths and angles are provided:
/// - `floppy::math::angle`
/// - `floppy::math::length`
/// - `floppy::math::scale` for converting between units
/// \todo Add class `box2d`.
/// \todo Add class `rect`.
/// \todo Add class `transform2d`.
/// \todo Add class `translation2d`.
/// \todo Add class `rotation2d`
///
/// The following classes for 2-dimensional geometry are provided:
/// - `floppy::math::point2d`
/// - `floppy::math::vector2d`
/// - `floppy::math::size2d`
///
/// 3-dimensional geometry is not yet supported. See to-do items below.
/// \todo Add class `box3d`
/// \todo Add class `transform3d`.
/// \todo Add class `translation3d`.
/// \todo Add class `vector3d`
/// \todo Add class `rotation3d`
/// \todo Add class `rigidtransform3d`
/// \todo Add class `point3d`
/// \todo Add class `size3d`
///
/// This module in inspired (copied from) Rust's `euclid` crate.
/// \sa https://docs.rs/euclid/latest/euclid
///
/// \invariant If **Qt Gui** is linked against the TU this header is compiled for, the conversions between
/// Qt classes (such as `QPointF`) and the `floppy` types are available.

// todo: if needed
// \defgroup geometry_details Geometry Details
// \ingroup geometry
// \brief Implementation details for classes and functions related to \ref geometry.