// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "protocol/protocol.h"
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
crossbow::Area::Area( const Common::MsgCoordLatLongList& message )
    : PointCollection( message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2010-03-02
// -----------------------------------------------------------------------------
crossbow::Area::Area( const OGRPolygon& area )
    : PointCollection( *area.getExteriorRing() )
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
// Name: Point::Extract
// Created: JCR 2010-02-26
// -----------------------------------------------------------------------------
OGRPolygon* crossbow::Area::Extract( OGRSpatialReference* spatialReference ) const
{
    OGRPolygon* geometry = new OGRPolygon();

    geometry->assignSpatialReference( spatialReference );
    OGRLinearRing* ring = static_cast< OGRLinearRing* >( PointCollection::Extract( new OGRLinearRing(), spatialReference ) );
    // ring->closeRing();
    geometry->addRingDirectly( ring );
    return geometry;
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
void crossbow::Area::Serialize( Common::MsgLocation& message ) const
{
    message.set_type( Common::MsgLocation_Geometry_polygon );
    PointCollection::Serialize( message );
}
