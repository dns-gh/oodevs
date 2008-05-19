// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Line_h_
#define __Line_h_

#include "PointCollection.h"

namespace crossbow
{
    class Point;

// =============================================================================
/** @class  Line
    @brief  Line
*/
// Created: SBO 2007-08-30
// =============================================================================
class Line : public PointCollection
{
public:
    //! @name Constructors/Destructor
    //@{
             Line();
    explicit Line( const ASN1T_CoordLatLongList& asn );
    explicit Line( IGeometryPtr geometry );
    virtual ~Line();
    //@}

    //! @name Operations
    //@{
    void UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const;
    void Serialize( ASN1T_Location& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Line( const Line& );            //!< Copy constructor
    Line& operator=( const Line& ); //!< Assignment operator
    //@}
};

}

#endif // __Line_h_
