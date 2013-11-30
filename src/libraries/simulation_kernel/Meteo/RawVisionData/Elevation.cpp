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

bool SplitOnMajorGridLines( int32_t cellSize, MT_Vector2D from, MT_Vector2D to,
        const std::function< bool( MT_Vector2D, MT_Vector2D )>& f )
{
    int x1 = static_cast< int >( from.rX_ );
    int y1 = static_cast< int >( from.rY_ );
    int x2 = static_cast< int >( to.rX_ );
    int y2 = static_cast< int >( to.rY_ );

    // Bresenham's line algorithm
    int delta_x = x2 - x1;
    int d_x = std::abs( delta_x ) << 1;
    int delta_y = y2 - y1;
    int d_y = std::abs( delta_y ) << 1;
    const bool flip = d_x < d_y;
    if( flip )
    {
        std::swap( x1, y1 );
        std::swap( x2, y2 );
        std::swap( d_x, d_y );
        std::swap( delta_x, delta_y );
    }
    int ix = (delta_x > 0) - (delta_x < 0);
    int iy = (delta_y > 0) - (delta_y < 0);
    int error = d_y - ( d_x >> 1 );

    int x = x1;
    int y = y1;
    // counter to avoid a (x % cellSize) in the main loop
    int cellPart = std::abs( x % cellSize );
    if( ix < 0 != x < 0 )
        cellPart = ( cellSize - cellPart ) % cellSize;
    auto prev = from;
    while( x != x2 )
    {
        if( error >= 0 && ( error || ix > 0 ) )
        {
            error -= d_x;
            y += iy;
        }
        error += d_y;
        x += ix;
        cellPart++;
        if( cellPart != cellSize && x != x2 )
            continue;
        cellPart = 0;
        auto next = x == x2
            ? to
            : flip
                ? MT_Vector2D( y, x )
                : MT_Vector2D( x, y );
        if( f( prev, next ) )
            return true;
        prev = next;
    }
    return false;
}
