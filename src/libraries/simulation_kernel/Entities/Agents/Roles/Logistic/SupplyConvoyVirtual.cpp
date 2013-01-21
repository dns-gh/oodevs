//  *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConvoyVirtual.h"
#include "SupplyConveyor_ABC.h"
#include "SupplySupplier_ABC.h"
#include "SupplyRecipient_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

// =============================================================================
// Constructor / destructor 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoyVirtual constructor
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplyConvoyVirtual::SupplyConvoyVirtual( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters )
    : SupplyConvoy( eventsObserver, supplier, parameters )
    , position_   ( transportersProvider_.GetPosition() )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyVirtual destructor
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplyConvoyVirtual::~SupplyConvoyVirtual()
{
}

// =============================================================================
// Operations
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoyVirtual::ComputeTravelTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
unsigned SupplyConvoyVirtual::ComputeTravelTime( const MT_Vector2D& startPos, const MT_Vector2D& endPos ) const
{
    if( startPos.IsZero() || endPos.IsZero() )
        return 0;
    unsigned travelTime = 0;
    BOOST_FOREACH( const T_Conveyors::value_type& data, conveyors_ )
        travelTime = std::max( travelTime, data.second->ApproximateTravelTime( startPos, endPos ) );
    return travelTime;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::MoveTo
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyVirtual::MoveTo( const MT_Vector2D& position )
{
    unsigned travelTime = ComputeTravelTime( position_, position );
    if( travelTime <= 0 )
        position_ = position;
    else
    {
        MT_Vector2D tmp( position - position_ );
        position_ += tmp.Normalized() * tmp.Magnitude() / travelTime;
    }
    return travelTime;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::MoveToSupplier
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyVirtual::MoveToSupplier()
{
    currentAction_ = eMoveToSupplier;
    return MoveTo( supplier_.GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::MoveToSupplyRecipient
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyVirtual::MoveToSupplyRecipient()
{
    currentAction_ = eMoveToSupplyRecipient;
    if( !currentSupplyRecipient_ )
        return 0;    
    return MoveTo( currentSupplyRecipient_->GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::MoveToTransportersProvider
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyVirtual::MoveToTransportersProvider()
{
    currentAction_ = eMoveToTransportersProvider;
    return MoveTo( transportersProvider_.GetPosition() );
}

// =============================================================================
// Network
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Serialize
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyVirtual::Serialize( sword::UnitId& msg ) const
{
    msg.set_id( 0 );
}
