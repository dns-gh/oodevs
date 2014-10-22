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
#include "clients_gui/FloodDrawer.h"
#include "propagation/FloodModel.h"

using namespace propagation;

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
double FloodProxy::GetElevationAt( const geometry::Point2f& point ) const
{
    if( detection_.Extent().IsOutside( point ) )
        return std::numeric_limits< double >::max();
    return detection_.ElevationAt( point, true );
}

// -----------------------------------------------------------------------------
// Name: FloodProxy::GetCellSize
// Created: JSR 2014-08-11
// -----------------------------------------------------------------------------
float FloodProxy::GetCellSize() const
{
    return detection_.GetCellSize();
}

// -----------------------------------------------------------------------------
// Name: FloodProxy::Draw
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
void FloodProxy::Draw( unsigned int floodId, gui::GLView_ABC& view ) const
{
    auto it = floods_.find( floodId );
    if( it != floods_.end() )
        it->second->Draw( view );
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
    auto it = floods_.find( floodId );
    if( it == floods_.end() )
    {
        floods_[ ++idManager_ ].reset( new gui::FloodDrawer( *pFloodModel_, point, depth, refDist ) );
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
    for( auto it = floods_.begin(); it != floods_.end(); ++it )
    {
        auto& drawer = *it->second;
        if( drawer.GetCenter() == point && drawer.GetDepth() == depth && drawer.GetReferenceDistance() == refDist )
            return it->first;
    }
    return 0;
}
