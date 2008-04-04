// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_GeometryEditor_h_
#define __gearth_GeometryEditor_h_

#include "game_asn/Simulation.h"
#include "geometry/Types.h"

namespace gearth
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
    explicit GeometryEditor();
    virtual ~GeometryEditor();
    //@}

    //! @name Operations
    //@{    
    geometry::Point2d ConvertPosition( const std::string& stMGRS ) const;
//    void Create     ( ILinePtr& spLine );
//    void Create     ( IPolylinePtr& spPolyline );
//    void Validate   ( IFeaturePtr spFeature, IPointPtr& spPoint );
//    bool UpdateCoord( IPointPtr spPoint, const ASN1T_CoordLatLong& asnUTM );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GeometryEditor( const GeometryEditor& );            //!< Copy constructor
    GeometryEditor& operator=( const GeometryEditor& ); //!< Assignement operator
    //@}
};

}

#endif // __gearth_GeometryEditor_h_
