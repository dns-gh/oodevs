// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "DisaggregationStrategy.h"
#include "DisaggregationArea.h"
#include "protocol/Protocol.h"

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: DisaggregationStrategy constructor
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
DisaggregationStrategy::DisaggregationStrategy( Facade& vrforces )
    : vrforces_( vrforces )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisaggregationStrategy destructor
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
DisaggregationStrategy::~DisaggregationStrategy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisaggregationStrategy::IsAggregatedLocation
// Created: SEB 2011-03-17
// -----------------------------------------------------------------------------
bool DisaggregationStrategy::IsAggregatedLocation( double latitude, double longitude ) const
{
    const geometry::Point2d location( latitude, longitude );
    for( std::map< unsigned long, boost::shared_ptr< DisaggregationArea_ABC > >::const_iterator it = area_.begin(); it != area_.end(); ++it )
        if( it->second->IsInside( location ) )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DisaggregationStrategy::AddArea
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
void DisaggregationStrategy::AddArea( const sword::ShapeCreation& message )
{
    if( message.shape().category() == "Tasks" && message.shape().pattern() == "Disaggregation area" )
        area_[ message.id().id() ].reset( new DisaggregationArea( message, vrforces_ ) );
}

// -----------------------------------------------------------------------------
// Name: DisaggregationStrategy::RemoveArea
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
void DisaggregationStrategy::RemoveArea( const sword::ShapeDestruction& message )
{
    area_.erase( message.id().id() );
}
