// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Area.h"
#include "Point.h"

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
plugins::crossbow::Area::Area()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2008-04-25
// -----------------------------------------------------------------------------
plugins::crossbow::Area::Area( const ASN1T_CoordLatLongList& asn )
    : PointCollection( asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2007-11-06
// -----------------------------------------------------------------------------
plugins::crossbow::Area::Area( IGeometryPtr geometry )
    : PointCollection( geometry )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area destructor
// Created: JCR 2008-04-25
// -----------------------------------------------------------------------------
plugins::crossbow::Area::~Area()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area::UpdateGeometry
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void plugins::crossbow::Area::UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    if( geometry == NULL )
        geometry.CreateInstance( CLSID_Polygon );
    PointCollection::UpdateGeometry( geometry, spatialReference );
}

// -----------------------------------------------------------------------------
// Name: Area::Serialize
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
void plugins::crossbow::Area::Serialize( ASN1T_Location& asn ) const
{
    asn.type = EnumLocationType::polygon;
    PointCollection::Serialize( asn );
}
