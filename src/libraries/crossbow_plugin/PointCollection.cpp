// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "PointCollection.h"
#include "Point.h"
#include "protocol/protocol.h"

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
// Created: JCR 2007-11-06
// -----------------------------------------------------------------------------
PointCollection::PointCollection( IGeometryPtr geometry )
{
    IPointCollectionPtr points;
    if( SUCCEEDED( geometry.QueryInterface( IID_IPointCollection, &points ) ) )
    {
        long count = 0;
        points->get_PointCount( &count );
        for( long i = 0; i < count; ++i )
        {
            IPointPtr point;
            points->get_Point( i, &point );
            points_.push_back( Point( point ) );
        }
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
// Created: JCR 2007-08-31
// -----------------------------------------------------------------------------
void PointCollection::Serialize( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const
{
    geometry->putref_SpatialReference( spatialReference );
    
    IZAwarePtr zAwareness;
    geometry.QueryInterface( IID_IZAware, &zAwareness );
    zAwareness->put_ZAware( VARIANT_FALSE );

    IPointCollectionPtr points;
    geometry.QueryInterface( IID_IPointCollection, &points );
    for( CIT_Points it = points_.begin(); it != points_.end(); ++it )
    {
        IPointPtr point( CLSID_Point );
        it->Serialize( point, spatialReference );
        points->AddPoint( point );
    }
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
