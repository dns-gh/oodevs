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

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
crossbow::Area::Area()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2008-04-25
// -----------------------------------------------------------------------------
crossbow::Area::Area( const ASN1T_CoordLatLongList& asn )
    : PointCollection( asn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2007-11-06
// -----------------------------------------------------------------------------
crossbow::Area::Area( IGeometryPtr geometry )
    : PointCollection( geometry )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area destructor
// Created: JCR 2008-04-25
// -----------------------------------------------------------------------------
crossbow::Area::~Area()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area::UpdateGeometry
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void crossbow::Area::Serialize( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    if( geometry == NULL )
        geometry.CreateInstance( CLSID_Polygon );
    PointCollection::Serialize( geometry, spatialReference );
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void crossbow::Area::Serialize( std::ostream& geometry ) const
{
	const int srid = 0;
    geometry << "st_polygon("
             << "'polygon(";
    PointCollection::Serialize( geometry );
    geometry << ")'," << srid
             << ")";
}

// -----------------------------------------------------------------------------
// Name: Area::Serialize
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Area::Serialize( ASN1T_Location& asn ) const
{
    asn.type = EnumLocationType::polygon;
    PointCollection::Serialize( asn );
}
