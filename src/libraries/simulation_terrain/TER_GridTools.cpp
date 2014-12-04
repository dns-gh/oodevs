// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_GridTools.h"
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

bool SplitOnMajorGridLinesNoOutlier( int32_t cellSize, MT_Vector2D from,
        MT_Vector2D to, const std::function< bool( MT_Vector2D, MT_Vector2D )>& f )
{
    MT_Vector2D p0;
    int seen = 0;
    return SplitOnMajorGridLines( cellSize, from, to,
        [&]( MT_Vector2D p1, MT_Vector2D p2 ) -> bool
        {
            bool result = false;
            if( p1 == from )
            {
                if( p2 == to )
                    // No split, just forward the call
                    return f( p1, p2 );
                // Skip first segment
                p0 = p1;
            }
            else
            {
                if( p2 == to )
                {
                    if( p0 == from && seen > 1 )
                    {
                        // Special case, from and to are separated by one cell:
                        // coalesce the 3 segments and split them in 2 again.
                        p1 = MT_Vector2D( ( p0.rX_ + p2.rX_ ) / 2,
                                          ( p0.rY_ + p2.rY_ ) / 2 );
                        if( f( p0, p1 ) )
                            return true;
                        result = f( p1, p2 );
                    }
                    else
                    {
                        result = f( p0, p2 );
                    }
                }
                else
                {
                    if( seen > 1 )
                    {
                        // Emit the previous segment, unless it was merged with
                        // the first one.
                        result = f( p0, p1 ); 
                        p0 = p1;
                    }
                }
            }
            ++seen;
            return result;
        });
}
