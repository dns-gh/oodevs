// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FloodProxy.h"
#include "clients_kernel/DetectionMap.h"
#include "flood/FloodDrawer.h"
#include "flood/FloodModel.h"

using namespace flood;

// -----------------------------------------------------------------------------
// Name: FloodProxy constructor
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
FloodProxy::FloodProxy( const kernel::DetectionMap& detection )
    : detection_  ( detection )
    , idManager_  ( 0 )
    , pFloodModel_( new FloodModel( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodProxy destructor
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
FloodProxy::~FloodProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodProxy::GetElevationAt
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
short FloodProxy::GetElevationAt( const geometry::Point2f& point ) const
{
    if( detection_.Extent().IsOutside( point ) )
        return std::numeric_limits< short >::max();
    return detection_.ElevationAt( point );
}

// -----------------------------------------------------------------------------
// Name: FloodProxy::Draw
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
void FloodProxy::Draw( unsigned int floodId ) const
{
    CIT_Floods it = floods_.find( floodId );
    if( it != floods_.end() )
        it->second->Draw();
}

// -----------------------------------------------------------------------------
// Name: FloodProxy::Remove
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
void FloodProxy::Remove( unsigned int floodId )
{
    floods_.erase( floodId );
}

// -----------------------------------------------------------------------------
// Name: FloodProxy::GenerateFlood
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
unsigned int FloodProxy::GenerateFlood( unsigned int floodId, const geometry::Point2f& point, int depth, int refDist )
{
    CIT_Floods it = floods_.find( floodId );
    if( it == floods_.end() )
    {
        FloodDrawer* floodDrawer = new FloodDrawer( *pFloodModel_, point, depth, refDist );
        ++idManager_;
        floods_[ idManager_ ].reset( floodDrawer );
        it = floods_.find( idManager_ );
    }
    it->second->Reset( *pFloodModel_, point, depth, refDist );
    return it->first;
}

// -----------------------------------------------------------------------------
// Name: FloodProxy::FindFlood
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
unsigned int FloodProxy::FindFlood( const geometry::Point2f& point, int depth, int refDist ) const
{
    for( CIT_Floods it = floods_.begin(); it != floods_.end(); ++it )
    {
        const FloodDrawer& drawer = *it->second;
        if( drawer.GetCenter() == point && drawer.GetDepth() == depth && drawer.GetReferenceDistance() == refDist )
            return it->first;
    }
    return 0;
}
