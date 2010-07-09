// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationSerializer_h_
#define __LocationSerializer_h_

#include "clients_kernel/LocationVisitor_ABC.h"
#include "protocol/clientsenders.h"

namespace Common
{
    //struct MsgCoordLatLong;
    class MsgLocation;
}
namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

// =============================================================================
/** @class  LocationSerializer
    @brief  Location serializer
*/
// Created: AGE 2006-08-09
// =============================================================================
class LocationSerializer : public kernel::LocationVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationSerializer( const kernel::CoordinateConverter_ABC& converter );
             LocationSerializer( const kernel::CoordinateConverter_ABC& converter, Common::MsgLocation& localisation );
    virtual ~LocationSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( const kernel::Location_ABC& location );
    void Serialize( const kernel::Location_ABC& location, Common::MsgLocation& localisation );
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines     ( const T_PointVector& points );
    virtual void VisitRectangle ( const T_PointVector& points );
    virtual void VisitPolygon   ( const T_PointVector& points );
    virtual void VisitCircle    ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint     ( const geometry::Point2f& point );
    virtual void VisitPath      ( const geometry::Point2f& first, const T_PointVector& points );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationSerializer( const LocationSerializer& );            //!< Copy constructor
    LocationSerializer& operator=( const LocationSerializer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetPoints( const T_PointVector& points );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    Common::MsgLocation* location_;
    //@}
};

#endif // __LocationSerializer_h_
