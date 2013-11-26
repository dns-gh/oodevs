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
    const int x2 = static_cast< int >( to.rX_ / cellSize );
    const int y2 = static_cast< int >( to.rY_ / cellSize );

    // Bresenham's line algorithm
    const int delta_x = x2 - x1;
    const int ix = (delta_x > 0) - (delta_x < 0);
    const int d_x = std::abs( delta_x ) << 1;
    const int delta_y = y2 - y1;
    const int iy = (delta_y > 0) - (delta_y < 0);
    const int d_y = std::abs( delta_y ) << 1;
    if( d_x >= d_y )
    {
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
            if( f( origin, MT_Vector2D( x1 * cellSize, y1 * cellSize ) ) )
                return true;
        }
    }
    else
    {
        int error = d_x - ( d_y >> 1 );
        while( y1 != y2 )
        {
            MT_Vector2D origin( x1 * cellSize, y1 * cellSize );
            if( error >= 0 && ( error || iy > 0 ) )
            {
                error -= d_y;
                x1 += ix;
            }
            error += d_x;
            y1 += iy;
            if( f( origin, MT_Vector2D( x1 * cellSize, y1 * cellSize ) ) )
                return true;
        }
    }
    return false;
}
