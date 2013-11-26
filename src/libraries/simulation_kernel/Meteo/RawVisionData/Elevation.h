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

bool SplitSegmentOnGrid( double cellSize, MT_Vector2D from, MT_Vector2D to,
        const std::function< bool( MT_Vector2D, MT_Vector2D )>& f );

#endif // Elevation_h
