// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Line.h"
#include "Point.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
crossbow::Line::Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Line::Line( const ASN1T_CoordLatLongList& asn )
    : PointCollection( asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line constructor
// Created: JCR 2007-11-06
// -----------------------------------------------------------------------------
crossbow::Line::Line( IGeometryPtr geometry )
    : PointCollection( geometry )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line destructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Line::~Line()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Line::UpdateGeometry
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Line::UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    if( geometry == NULL )
        geometry.CreateInstance( CLSID_Polyline );
    PointCollection::UpdateGeometry( geometry, spatialReference );
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( ASN1T_Location& asn ) const
{
    asn.type = EnumLocationType::line;
    PointCollection::Serialize( asn );
}
