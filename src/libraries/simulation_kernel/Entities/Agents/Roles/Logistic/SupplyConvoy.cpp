//*****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConvoy.h"
#include "SupplyConveyor.h"
#include "SupplyConvoyEventsObserver_ABC.h"
#include "SupplyConvoyConfig.h"
#include "SupplyRequestParameters_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include <boost/foreach.hpp>

using namespace logistic;

// =============================================================================
// Constructor / destructor 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoy constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConvoy::SupplyConvoy( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters )
    : eventsObserver_               ( eventsObserver )
    , parameters_                   ( parameters )
    , supplier_                     ( supplier )
    , transportersProvider_         ( parameters.GetTransportersProvider() ? *parameters.GetTransportersProvider() : supplier )
    , autoAllocateNewTransporters_  ( parameters.GetTransporters().empty() )
    , currentAction_                ( eNone )
    , timeRemainingForCurrentAction_( 0 )
    , currentSupplyRecipient_       ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy destructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConvoy::~SupplyConvoy()
{
}

// =============================================================================
// Operations
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::CreateConveyor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
template< typename T >
SupplyConveyor_ABC* SupplyConvoy::CreateConveyor( const T& constraint )
{
    PHY_ComposantePion* conveyorComposante = 0;
    MIL_AgentPion*      conveyorPion = 0;
    if( !transportersProvider_.SupplyGetAvailableConvoyTransporter( conveyorComposante, conveyorPion, constraint ) )
        return 0;
    SupplyConveyor_ABC* conveyor = new SupplyConveyor( *conveyorComposante, *conveyorPion );
    bool out = conveyors_.insert( std::make_pair( conveyorComposante, conveyor ) ).second;
    assert( out ); //$$ ???
    return conveyor;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::ReserveTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConvoy::ReserveTransporters( const PHY_DotationCategory& dotationCategory, double quantity )
{
    // Fill the previously used conveyors
    for( T_Conveyors::const_iterator it = conveyors_.begin(); it != conveyors_.end() && quantity > 0.; ++it )
    {
        SupplyConveyor_ABC& conveyor = *it->second;
        if( conveyor.IsFull() )
            continue;
        quantity -= conveyor.Convoy( eventsObserver_, dotationCategory, quantity );
    }

    // Allocate new conveyors
    if( autoAllocateNewTransporters_ )
    {
        while( quantity > 0. )
        {
            SupplyConveyor_ABC* conveyor = CreateConveyor( dotationCategory );
            if( conveyor )
                quantity -= conveyor->Convoy( eventsObserver_, dotationCategory, quantity );
            else
                break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::ReserveTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
unsigned SupplyConvoy::ReserveTransporters( const T_Resources& resources )
{
    // Allocate the specified transporters
    BOOST_FOREACH( const SupplyRequestParameters_ABC::T_Transporters::value_type& data, parameters_.GetTransporters() )
    {
        for( unsigned i = 0; i < data.second; ++i )
        {
            if( !CreateConveyor( *data.first ) )
                break;
        }
    }

    // Link the resources to the transporters
    BOOST_FOREACH( const T_Resources::value_type& data, resources )
        ReserveTransporters( *data.first, data.second );
    
    // Remove the empty transporters (can happen when the transporters are explicitly specified by the user)
    for( T_Conveyors::iterator it = conveyors_.begin(); it != conveyors_.end(); )
    {
        if( it->second->IsEmpty() )
            it = conveyors_.erase( it );
        else 
            ++it;
    }

    if( conveyors_.empty() )
        return std::numeric_limits< unsigned >::max();

    eventsObserver_.OnAllResourcesAssignedToConvoy();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::ComputeTimeRemainingForAction
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoy::ComputeTimeRemainingForAction( E_Action action, MT_InterpolatedFunction< double >& timeComputer )
{
    if( currentAction_ == action )
    {
        if( timeRemainingForCurrentAction_ > 0 ) 
            --timeRemainingForCurrentAction_;
    }
    else
    {
        currentAction_ = action;
        timeRemainingForCurrentAction_ = (unsigned int)( timeComputer( static_cast< double >( conveyors_.size() ) ) );
    }
    return timeRemainingForCurrentAction_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Setup
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
unsigned SupplyConvoy::Setup()
{
    return ComputeTimeRemainingForAction( eSetup, SupplyConvoyConfig::setupTime_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Load
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoy::Load()
{
    return ComputeTimeRemainingForAction( eLoad, SupplyConvoyConfig::loadingTime_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Unload
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoy::Unload()
{
    return ComputeTimeRemainingForAction( eUnload, SupplyConvoyConfig::unloadingTime_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::SetCurrentSupplyRecipient
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoy::SetCurrentSupplyRecipient( SupplyRecipient_ABC* supplyRecipient )
{
    if( supplyRecipient != currentSupplyRecipient_ )
    {
        currentAction_ = eNone; //$$$
        currentSupplyRecipient_ = supplyRecipient;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Unload
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoy::Supply( SupplyRecipient_ABC& /*supplyRecipient*/, const PHY_DotationCategory& dotationCategory, double quantity )
{
    for( T_Conveyors::const_iterator it = conveyors_.begin(); it != conveyors_.end() && quantity > 0.; ++it )
    {
        SupplyConveyor_ABC& conveyor = *it->second;
        quantity -= conveyor.Supply( dotationCategory, quantity );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Finish
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoy::Finish()
{
    conveyors_.clear();
}

// =============================================================================
// Accessors
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Finish
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplySupplier_ABC& SupplyConvoy::GetTransportersProvider() const
{
    return transportersProvider_;
}
