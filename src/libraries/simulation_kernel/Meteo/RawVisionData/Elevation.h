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

#include <boost/function.hpp>
#include "MT_Tools/MT_Vector2D.h"

// =============================================================================
/** @class  Elevation
    @brief  Elevation
*/
// Created: MCO 2013-06-04
// =============================================================================
class Elevation
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Elevation( double cellSize )
        : cellSize_( cellSize )
    {}
    //@}

    //! @name Operations
    //@{
    template< typename Functor >
    bool FindPath( const MT_Vector2D& from, const MT_Vector2D& to, Functor f ) const
    {
        int x1 = static_cast< int >( from.rX_ / cellSize_ );
        int y1 = static_cast< int >( from.rY_ / cellSize_ );
        const int x2 = static_cast< int >( to.rX_ / cellSize_ );
        const int y2 = static_cast< int >( to.rY_ / cellSize_ );

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
                MT_Vector2D origin( x1 * cellSize_, y1 * cellSize_ );
                if( error >= 0 && ( error || ix > 0 ) )
                {
                    error -= d_x;
                    y1 += iy;
                }
                error += d_y;
                x1 += ix;
                if( f( origin, MT_Vector2D( x1 * cellSize_, y1 * cellSize_ ) ) )
                    return true;
            }
        }
        else
        {
            int error = d_x - ( d_y >> 1 );
            while( y1 != y2 )
            {
                MT_Vector2D origin( x1 * cellSize_, y1 * cellSize_ );
                if( error >= 0 && ( error || iy > 0 ) )
                {
                    error -= d_y;
                    x1 += ix;
                }
                error += d_x;
                y1 += iy;
                if( f( origin, MT_Vector2D( x1 * cellSize_, y1 * cellSize_ ) ) )
                    return true;
            }
        }
        return false;
    }

    //@}

private:
    //! @name Member data
    //@{
    double cellSize_;
    //@}
};

#endif // Elevation_h
