// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GeometryEditor_h_
#define __GeometryEditor_h_

#include "ESRI.h"
#include "game_asn/Asn.h"

namespace crossbow
{

// =============================================================================
/** @class  GeometryEditor
    @brief  Geometry Editor
*/
// Created: JCR 2007-05-15
// =============================================================================
class GeometryEditor
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit GeometryEditor( ISpatialReferencePtr spSpatialReference );
    virtual ~GeometryEditor();
    //@}

    //! @name Operations
    //@{
    void Create     ( IPointPtr& spPoint, bool bUpdateZAxis = false );
    void Create     ( ILinePtr& spLine );
    void Create     ( IPolylinePtr& spPolyline );
    void Create     ( IRingPtr& spRing );
    void Create     ( IPolygonPtr& spPolygon );
    void Validate   ( IFeaturePtr spFeature, IPointPtr& spPoint );
    bool UpdateCoord( IPointPtr spPoint, const ASN1T_CoordUTM& asnUTM );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GeometryEditor( const GeometryEditor& );            //!< Copy constructor
    GeometryEditor& operator=( const GeometryEditor& ); //!< Assignement operator
    //@}

    //! @name
    //@{
    void UpdateZAware( IPointPtr spPoint ) const;
    void ConvertCoordMosToSim( const std::string& strMgrs, IPointPtr& spPoint ) const;
    //@}

private:
    //! @name Member data
    //@{
    ISpatialReferencePtr    spSpatialReference_;
    //@}
};

}

#endif // __GeometryEditor_h_
