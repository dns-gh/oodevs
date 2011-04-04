// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "AggregatedPosition.h"
#include "Agent.h"
#pragma warning( push, 0 )
#include <matrix/geodcoord.h>
#pragma warning( pop )

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: AggregatedPosition constructor
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
AggregatedPosition::AggregatedPosition( const Agent& agent )
    : agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregatedPosition destructor
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
AggregatedPosition::~AggregatedPosition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregatedPosition::Update
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
void AggregatedPosition::Update( const DtEntityIdentifier& identifier, const DtVector& position )
{
    DtGeodeticCoord converter;
    converter.setGeocentric( position );
    points_[ identifier ] = geometry::Point2d( DtRad2Deg( converter.lat() ), DtRad2Deg( converter.lon() ) );
    ComputeBarycenter();
    if( last_.Distance( barycenter_ ) > 0.001 )
    {
        agent_.MoveTo( barycenter_ );
        last_ = barycenter_;
    }
}

// -----------------------------------------------------------------------------
// Name: AggregatedPosition::Clear
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
void AggregatedPosition::Clear()
{
    points_.clear();
    barycenter_.Set( 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: AggregatedPosition::ComputeBarycenter
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
void AggregatedPosition::ComputeBarycenter()
{
    barycenter_.Set( 0, 0 );
    for( std::map< const DtEntityIdentifier, geometry::Point2d >::const_iterator it = points_.begin(); it != points_.end(); ++it )
        barycenter_ += geometry::Vector2d( it->second.X(), it->second.Y() );
    barycenter_.Set( barycenter_.X() / points_.size(), barycenter_.Y() / points_.size() );
}
