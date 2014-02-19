// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef Elevation_h
#define Elevation_h

#include <functional>

class MT_Vector2D;

// "f" is called with sub-subsegments of [from, to] which endings are "from",
// "to" or the intersection of [from, to] with grid lines orthogonal to
// [from, to] major axis, the major axis being the axis on which the segments 
// crosses the most cells. For instance, with cellSize == 100, the segment
// (50, 50) -> (250, 150) will be split into:
//
//   (50, 50) -> (100, 75)
//   (100, 75) -> (200, 125)
//   (200, 125) -> (250, 150)
//
// Note that no split occurs on the grid line y == 100.
//
// If from and to lies in the same cell, "f" is called once with boths.
// If from == to, "f" is not called.
//
// If "f" returns true, the walk terminates and the call returns true.
//
// cellSize, from and to coordinates are assumed to fix on an integer grid.
bool SplitOnMajorGridLines( int32_t cellSize, MT_Vector2D from, MT_Vector2D to,
        const std::function< bool( MT_Vector2D, MT_Vector2D )>& f );

#endif // Elevation_h
