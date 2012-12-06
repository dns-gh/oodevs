// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "protocol/Protocol.h"
#include "PointCollection.h"
#include "Point.h"
#include <gdal/ogr_geometry.h>
#include <gdal/ogr_feature.h>

using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: PointCollection constructor
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
PointCollection::PointCollection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointCollection constructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
PointCollection::PointCollection( const sword::CoordLatLongList& message )
{
    for( int i = 0; i < message.elem_size(); ++i )
        points_.push_back( crossbow::Point( message.elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: PointCollection constructor
// Created: JCR 2010-03-02
// -----------------------------------------------------------------------------
PointCollection::PointCollection( const OGRLineString& geometry )
{
    for( int i = 0; i < geometry.getNumPoints(); ++i )
    {
        OGRPoint point;
        geometry.getPoint( i, &point );
        points_.push_back( crossbow::Point( point )  );
    }
}

// -----------------------------------------------------------------------------
// Name: PointCollection destructor
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
PointCollection::~PointCollection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PointCollection::Serialize
// Created: JCR 2010-04-08
// -----------------------------------------------------------------------------
void PointCollection::Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const
{
    OGRLineString points;
    Serialize( points, spatialReference );
    feature.SetGeometry( &points );
}

// -----------------------------------------------------------------------------
// Name: PointCollection::Extract
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
void PointCollection::Serialize( OGRLineString& points, OGRSpatialReference* spatialReference ) const
{
    points.assignSpatialReference( spatialReference );
    for( auto it = points_.begin(); it != points_.end(); ++it )
    {
        OGRPoint point;
        // point.setCoordinateDimension( 2 );
        it->Serialize( point, spatialReference );
        points.addPoint( &point );
    }
}

// -----------------------------------------------------------------------------
// Name: PointCollection::Serialize
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void PointCollection::Serialize( std::ostream& geometry ) const
{
    std::stringstream ss;

    for( auto it = points_.begin(); it != points_.end(); ++it )
    {
        it->SerializeCoordinates( ss, ' ' );
        if( (it + 1) != points_.end() )
            ss << ",";
    }
    geometry << "(" << ss.str() << ")";
}


// -----------------------------------------------------------------------------
// Name: PointCollection::Serialize
// Created: JCR 2010-12-04
// -----------------------------------------------------------------------------
void PointCollection::Serialize( kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter ) const
{
    for( auto it = points_.begin(); it != points_.end(); ++it )
        it->Serialize( location, converter );
}
