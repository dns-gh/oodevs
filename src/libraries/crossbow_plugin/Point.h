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
#include "game_asn/Asn.h"
#include "ESRI.h"

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
    explicit Point( const ASN1T_CoordUTM& coord );
    virtual ~Point();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ShapeVisitor_ABC& visitor ) const;
    void UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const;
    void Serialize( ASN1T_Location& asn ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void SetFromUTM( const std::string& utm );
    //@}

private:
    //! @name Member data
    //@{
    double x_;
    double y_;
    //@}
};

}

#endif // __Point_h_
