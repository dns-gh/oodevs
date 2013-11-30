// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Elevation.h"
#include "MT_Tools/MT_Vector2D.h"

bool SplitSegmentOnGrid( double cellSize, MT_Vector2D from, MT_Vector2D to,
        const std::function< bool( MT_Vector2D, MT_Vector2D )>& f )
{
    int x1 = static_cast< int >( from.rX_ / cellSize );
    int y1 = static_cast< int >( from.rY_ / cellSize );
    int x2 = static_cast< int >( to.rX_ / cellSize );
    int y2 = static_cast< int >( to.rY_ / cellSize );

    // Bresenham's line algorithm
    int delta_x = x2 - x1;
    int ix = (delta_x > 0) - (delta_x < 0);
    int d_x = std::abs( delta_x ) << 1;
    int delta_y = y2 - y1;
    int iy = (delta_y > 0) - (delta_y < 0);
    int d_y = std::abs( delta_y ) << 1;
    const bool swap = d_x < d_y;
    if( swap )
    {
        std::swap( x1, y1 );
        std::swap( x2, y2 );
        std::swap( d_x, d_y );
        std::swap( ix, iy );
        std::swap( delta_x, delta_y );
    }
    int error = d_y - ( d_x >> 1 );
    while( x1 != x2 )
    {
        MT_Vector2D origin( x1 * cellSize, y1 * cellSize );
        if( error >= 0 && ( error || ix > 0 ) )
        {
            error -= d_x;
            y1 += iy;
        }
        error += d_y;
        x1 += ix;
        const bool stop = swap
            ? f( MT_Vector2D( origin.GetY(), origin.GetX() ), MT_Vector2D( y1 * cellSize, x1 * cellSize ) )
            : f( origin, MT_Vector2D( x1 * cellSize, y1 * cellSize ) );
        if( stop )
            return true;
    }
    return false;
}
