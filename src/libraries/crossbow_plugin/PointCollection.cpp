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
#include "PointCollection.h"
#include "Point.h"

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
PointCollection::PointCollection( const Common::MsgCoordLatLongList& message )
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
// Name: PointCollection::Extract
// Created: JCR 2010-02-26
// -----------------------------------------------------------------------------
OGRGeometry* PointCollection::Extract( OGRSpatialReference* spatialReference ) const
{
    OGRLineString* points = new OGRLineString();
    return Extract( points, spatialReference );
}

// -----------------------------------------------------------------------------
// Name: PointCollection::Extract
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
OGRLineString* PointCollection::Extract( OGRLineString* points, OGRSpatialReference* spatialReference ) const
{
    points->assignSpatialReference( spatialReference );
    for( CIT_Points it = points_.begin(); it != points_.end(); ++it )
        points->addPoint( it->Extract( spatialReference ) );
    return points;
}

// -----------------------------------------------------------------------------
// Name: PointCollection::Serialize
// Created: JCR 2009-04-27
// -----------------------------------------------------------------------------
void PointCollection::Serialize( std::ostream& geometry ) const
{
    std::stringstream ss;

    for( CIT_Points it = points_.begin(); it != points_.end(); ++it )
    {
        it->SerializeCoordinates( ss, ' ' );
        if( (it + 1) != points_.end() )
            ss << ",";
    }
    geometry << "(" << ss.str() << ")";
}

// -----------------------------------------------------------------------------
// Name: PointCollection::Serialize
// Created: JCR 2007-09-26
// -----------------------------------------------------------------------------
void PointCollection::Serialize( Common::MsgLocation& message ) const
{
    for( CIT_Points it = points_.begin(); it != points_.end(); ++it )
        it->Serialize( *message.mutable_coordinates()->add_elem() );
}
