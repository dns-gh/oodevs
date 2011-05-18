// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LocationFunctor_ABC_h_
#define __LocationFunctor_ABC_h_

#include <boost/noncopyable.hpp>
#include "Types.h"

namespace kernel
{

// =============================================================================
/** @class  LocationVisitor_ABC
    @brief  Location visitor definition
*/
// Created: AGE 2006-08-09
// =============================================================================
class LocationVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LocationVisitor_ABC() {}
    virtual ~LocationVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines    ( const T_PointVector& points ) = 0;
    virtual void VisitRectangle( const T_PointVector& points ) = 0;
    virtual void VisitPolygon  ( const T_PointVector& points ) = 0;
    virtual void VisitCircle   ( const geometry::Point2f& center, float radius ) = 0;
    virtual void VisitPoint    ( const geometry::Point2f& point ) = 0;
    virtual void VisitPath     ( const geometry::Point2f& first, const T_PointVector& points ) = 0;
    //@}
};

}

#endif // __LocationFunctor_ABC_h_
