// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Point_h_
#define __Point_h_

#include "Shape_ABC.h"

class OGRPoint;

namespace sword
{
    class CoordLatLong;
}

namespace plugins
{
namespace crossbow
{
// =============================================================================
/** @class  Point
    @brief  Point
*/
// Created: SBO 2007-08-30
// =============================================================================
class Point : public Shape_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Point();
    explicit Point( const sword::CoordLatLong& coord );
    explicit Point( const OGRPoint& point );
    virtual ~Point();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( std::ostream& geometry ) const;
    virtual void Serialize( kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter ) const;
    virtual void Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const;

    void Serialize( OGRPoint& point, OGRSpatialReference* spatialReference ) const;
    std::ostream& SerializeCoordinates( std::ostream& geometry, char sep ) const;
    //@}

private:
    //! @name Member data
    //@{
    double x_;
    double y_;
    //@}
};

}
}

#endif // __Point_h_
