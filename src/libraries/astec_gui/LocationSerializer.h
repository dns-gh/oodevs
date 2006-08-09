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

#include "astec_kernel/LocationVisitor_ABC.h"
struct ASN1T_CoordUTM;
struct ASN1T_Localisation;
class CoordinateConverter_ABC;
class Location_ABC;

// =============================================================================
/** @class  LocationSerializer
    @brief  Location serializer
*/
// Created: AGE 2006-08-09
// =============================================================================
class LocationSerializer : public LocationVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationSerializer( const CoordinateConverter_ABC& converter );
             LocationSerializer( const CoordinateConverter_ABC& converter, ASN1T_Localisation& localisation );
    virtual ~LocationSerializer();
    //@}

    //! @name Operations
    //@{
    void Serialize( const Location_ABC& location );
    void Serialize( const Location_ABC& location, ASN1T_Localisation& localisation );
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
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
    const CoordinateConverter_ABC& converter_;
    ASN1T_Localisation* localisation_;
    ASN1T_CoordUTM* pCoords_;
    bool ownsCoords_;
    //@}
};

#endif // __LocationSerializer_h_
