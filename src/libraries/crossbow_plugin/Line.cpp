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
#include "protocol/SimulationSenders.h"
#pragma warning( push, 0 )
#include <gdal/ogr_geometry.h>
#include <gdal/ogr_feature.h>
#pragma warning( pop )
#include <sstream>

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
crossbow::Line::Line( const sword::CoordLatLongList& message )
    : PointCollection( message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Area constructor
// Created: JCR 2010-03-02
// -----------------------------------------------------------------------------
crossbow::Line::Line( const OGRLineString& line )
    : PointCollection( line )
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

namespace
{
    void ImportWkt( OGRLineString& geometry, const std::string& strwkt )
    {
        char* value = const_cast< char* >( strwkt.c_str() );
        geometry.importFromWkt( &value );
    }
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
//    NOTE: OGR builds geometry with 3 dimension points even if 2 is explictly specified
//       OGR implementation is :
/*
        geometry.setCoordinateDimension( 2 );
        geometry.assignSpatialReference( spatialReference );
        PointCollection::Serialize( geometry, spatialReference );
*/
// Created: JCR 2010-04-08
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const
{
    OGRLineString geometry;
    std::stringstream ssWkt;

    SerializeWkt( ssWkt );
    ImportWkt( geometry, ssWkt.str() );
    geometry.assignSpatialReference( spatialReference );
    if( feature.SetGeometry( &geometry ) != OGRERR_NONE )
        throw std::runtime_error( "Failed to set line geometry." );
}

// -----------------------------------------------------------------------------
// Name: Line::SerializeWkt
// Created: JCR 2010-05-17
// -----------------------------------------------------------------------------
void crossbow::Line::SerializeWkt( std::ostream& geometry ) const
{
    geometry << "LINESTRING"; // point collection already provides '(' and ')'
    PointCollection::Serialize( geometry );
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( std::ostream& geometry ) const
{
    const int srid = 0;
    geometry << "st_linestring('";
    SerializeWkt( geometry );
    geometry << "'," << srid << ")";
}

// -----------------------------------------------------------------------------
// Name: Line::Serialize
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::Line::Serialize( kernel::Location_ABC& message, const kernel::CoordinateConverter_ABC& converter ) const
{
    PointCollection::Serialize( message, converter );
}
