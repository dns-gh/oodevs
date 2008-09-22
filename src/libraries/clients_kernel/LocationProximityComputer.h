// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationProximityComputer_h_
#define __LocationProximityComputer_h_

#include "LocationVisitor_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  LocationProximityComputer
    @brief  LocationProximityComputer
*/
// Created: SBO 2008-06-02
// =============================================================================
class LocationProximityComputer : public LocationVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LocationProximityComputer( const geometry::Point2f& point, float precision );
    virtual ~LocationProximityComputer();
    //@}

    //! @name Accessors
    //@{
    bool IsAt() const;
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
    virtual void VisitPath   ( const geometry::Point2f& , const T_PointVector& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationProximityComputer( const LocationProximityComputer& );            //!< Copy constructor
    LocationProximityComputer& operator=( const LocationProximityComputer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f point_;
    float precision_, squarePrecision_;
    bool isAt_;
    //@}
};

}

#endif // __LocationProximityComputer_h_
