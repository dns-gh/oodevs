// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Point_h_
#define __Point_h_

#include "Shape_ABC.h"
#include "ESRI.h"

namespace Common
{
    class MsgLocation;
    class MsgCoordLatLong;
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
    explicit Point( const Common::MsgCoordLatLong& coord );
    explicit Point( IGeometryPtr geometry );
    virtual ~Point();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const;
    virtual void Serialize( std::ostream& geometry ) const;
    virtual void Serialize( Common::MsgLocation& message ) const;
    virtual void Serialize( Common::MsgCoordLatLong& message ) const;
    //@}

    //! @name 
    //@{    
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
