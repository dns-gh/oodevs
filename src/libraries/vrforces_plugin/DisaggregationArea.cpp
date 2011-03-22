// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "DisaggregationArea.h"
#include "Facade.h"
#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include <matrix/geodcoord.h>
#include <matrix/topocoord.h>
#include <vlutil/vllist.h>
#pragma warning( pop )
#include <sstream>

using namespace plugins::vrforces;

namespace
{
    geometry::Polygon2d Convert( const sword::CoordLatLongList& message )
    {
        geometry::Polygon2d result;
        for( int i = 0; i < message.elem_size(); ++i )
            result.Add( geometry::Point2d( message.elem( i ).latitude(), message.elem( i ).longitude() ) );
        return result;
    }

    DtVector* ConvertToGeocentric( const sword::CoordLatLong& message )
    {
        DtGeodeticCoord coord;
        coord.setLat( DtDeg2Rad( message.latitude() ) );
        coord.setLon( DtDeg2Rad( message.longitude() ) );
        return new DtVector( coord.geocentric().x(), coord.geocentric().y(), coord.geocentric().z() );
    }
}

// -----------------------------------------------------------------------------
// Name: DisaggregationArea constructor
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
DisaggregationArea::DisaggregationArea( const sword::ShapeCreation& message, Facade& facade )
    : polygon_( Convert( message.shape().points() ) )
{
    DtList vertices;
    for( int i = 0; i < message.shape().points().elem_size(); ++i )
        vertices.add( ConvertToGeocentric( message.shape().points().elem( i ) ) );
    std::stringstream name;
    name << "disaggregation area #" << message.id().id();
    facade.CreateDisaggregationArea( name.str().c_str(), vertices );
}

// -----------------------------------------------------------------------------
// Name: DisaggregationArea destructor
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
DisaggregationArea::~DisaggregationArea()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisaggregationArea::IsInside
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
bool DisaggregationArea::IsInside( const geometry::Point2d& position ) const
{
    return polygon_.IsInside( position );
}
