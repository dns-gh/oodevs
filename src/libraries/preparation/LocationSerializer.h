// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LocationSerializer_h_
#define __LocationSerializer_h_

#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  LocationSerializer
    @brief  LocationSerializer
*/
// Created: SBO 2006-09-12
// =============================================================================
class LocationSerializer : public kernel::LocationVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationSerializer( const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocationSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( const kernel::Location_ABC& location, xml::xostream& xos );
    //@}

    //! @name Accessors
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
    virtual void VisitPath   ( const geometry::Point2f& first, const T_PointVector& points );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LocationSerializer( const LocationSerializer& );            //!< Copy constructor
    LocationSerializer& operator=( const LocationSerializer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void SetPoints( const T_PointVector& points ); 
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    xml::xostream* xos_;
    //@}
};

#endif // __LocationSerializer_h_
