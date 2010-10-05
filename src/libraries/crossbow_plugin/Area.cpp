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

namespace
{
    void ImportWkt( OGRPolygon& geometry, const std::string& strwkt )
    {
        char* value = const_cast< char* >( strwkt.c_str() );
        geometry.importFromWkt( &value );
    }
}

// -----------------------------------------------------------------------------
// Name: Area::Serialize
//    NOTE: OGR builds geometry with 3 dimension points even if 2 is explictly specified
//       OGR implementation is :
/*
    OGRLinearRing ring;
    ring.setCoordinateDimension( 2 );
    PointCollection::Serialize( ring, spatialReference );
    geometry.setCoordinateDimension( 2 );
    geometry.addRing( &ring );
    geometry.closeRings();
*/
// Created: JCR 2010-04-08
// -----------------------------------------------------------------------------
void crossbow::Area::Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const
{
    OGRPolygon geometry;
    std::stringstream ssWkt;

    SerializeWkt( ssWkt );
    ImportWkt( geometry, ssWkt.str() );
    geometry.assignSpatialReference( spatialReference );
    if( feature.SetGeometry( &geometry ) != OGRERR_NONE )
        throw std::runtime_error( "Failed to area geometry." );
}

// -----------------------------------------------------------------------------
// Name: Area::SerializeWkt
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void crossbow::Area::SerializeWkt( std::ostream& geometry ) const
{
    geometry << "POLYGON(";
    PointCollection::Serialize( geometry );
    geometry << ")";
}

// -----------------------------------------------------------------------------
// Name: Area::Serialize
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void crossbow::Area::Serialize( std::ostream& geometry ) const
{
    const int srid = 0;
    geometry << "st_polygon('";
    SerializeWkt( geometry );
    geometry << "'," << srid << ")";
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
