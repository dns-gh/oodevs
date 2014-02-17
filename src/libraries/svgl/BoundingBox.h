// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BoundingBox_h_
#define __BoundingBox_h_

#include "Point.h"
#include <limits>
#include <xutility>

namespace geometry
{

// =============================================================================
/** @class  BoundingBox
    @brief  BoundingBox
*/
// Created: AGE 2006-09-04
// =============================================================================
class BoundingBox
{

public:
    //! @name Constructors/Destructor
    //@{
             BoundingBox()
                 : xMin_(  std::numeric_limits< float >::infinity() ), yMin_(  std::numeric_limits< float >::infinity() )
                 , xMax_( -std::numeric_limits< float >::infinity() ), yMax_( -std::numeric_limits< float >::infinity() )
             {};
             BoundingBox( float left, float bottom, float right, float top )
                 : xMin_( left ), yMin_( bottom ), xMax_( right ), yMax_( top ) 
             {};
    //@}

    //! @name Operations
    //@{
    void Incorporate( const Point& p ) 
    {
        xMin_ = std::min( xMin_, p.X() ); yMin_ = std::min( yMin_, p.Y() );
        xMax_ = std::min( xMax_, p.X() ); yMax_ = std::min( yMax_, p.Y() );
    };

    BoundingBox Intersect( const BoundingBox& r ) const
    {
        return BoundingBox( std::max( xMin_, r.xMin_ ), std::max( yMin_, r.yMin_ )
                          , std::min( xMax_, r.xMax_ ), std::min( yMax_, r.yMax_ ) );
    }

    bool IsEmpty() const {
        return xMin_ > xMax_ || yMin_ > yMax_;
    }

    float Width() const {
        return xMax_ - xMin_;
    }
    float Height() const {
        return yMax_ - yMin_;
    }
    //@}

private:
    //! @name Member data
    //@{
    float xMin_, yMin_;
    float xMax_, yMax_;
    //@}
};

}

#endif // __BoundingBox_h_
