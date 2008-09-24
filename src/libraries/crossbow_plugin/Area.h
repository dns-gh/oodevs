// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Area_h_
#define __Area_h_

#include "PointCollection.h"

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  Area
*/
// Created: JCR 2008-04-25
// =============================================================================
class Area : public PointCollection
{
public:
    //! @name Constructors/Destructor
    //@{
             Area();
    explicit Area( const ASN1T_CoordLatLongList& asn );
    explicit Area( IGeometryPtr geometry );    
    virtual ~Area();
    //@}

    //! @name Operations
    //@{
    void UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const;
    void Serialize( ASN1T_Location& asn ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Area( const Area& );            //!< Copy constructor
    Area& operator=( const Area& ); //!< Assignement operator
    //@}
};

}
}

#endif // __Area_h_
