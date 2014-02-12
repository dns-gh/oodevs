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
#include "Checkpoints/MIL_CheckPointInArchive.h"
#include "Checkpoints/MIL_CheckPointOutArchive.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MT_Tools/MT_InterpolatedFunction.h"
#include <boost/foreach.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::SupplyConvoy )

// -----------------------------------------------------------------------------
// Name: SupplyConvoy constructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConvoy::SupplyConvoy( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters )
    : eventsObserver_               ( &eventsObserver )
    , parameters_                   ( &parameters )
    , supplier_                     ( &supplier )
    , transportersProvider_         ( parameters.GetTransportersProvider() ? parameters.GetTransportersProvider() : &supplier )
    , currentAction_                ( eNone )
    , timeRemainingForCurrentAction_( 0 )
    , currentSupplyRecipient_       ( 0 )
    , provider_                     ( 0 )
    , autoAllocateNewTransporters_  ( parameters.GetTransporters().empty() )
    , finished_                     ( false )
    , impossible_                   ( false )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy constructor
// Created: LDC 2013-01-14
// -----------------------------------------------------------------------------
SupplyConvoy::SupplyConvoy()
    : eventsObserver_               ( 0 )
    , parameters_                   ( 0 )
    , supplier_                     ( 0 )
    , transportersProvider_         ( 0 )
    , currentAction_                ( eNone )
    , timeRemainingForCurrentAction_( 0 )
    , currentSupplyRecipient_       ( 0 )
    , provider_                     ( 0 )
    , autoAllocateNewTransporters_  ( false )
    , finished_                     ( false )
    , impossible_                   ( false )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy destructor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
SupplyConvoy::~SupplyConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::CreateConveyor
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
template< typename T >
SupplyConveyor_ABC* SupplyConvoy::CreateConveyor( const T& constraint )
{
    PHY_ComposantePion* conveyorComposante = 0;
    MIL_AgentPion*      conveyorPion = 0;
    if( !transportersProvider_->SupplyGetAvailableConvoyTransporter( conveyorComposante, conveyorPion, constraint ) )
        return 0;
    SupplyConveyor_ABC* conveyor = new SupplyConveyor( *conveyorComposante, *conveyorPion );
    conveyors_.insert( std::make_pair( conveyorComposante, conveyor ) );
    return conveyor;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::ReserveTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
void SupplyConvoy::ReserveTransporters( const PHY_DotationCategory& dotationCategory, double quantity )
{
    // Fill the previously used conveyors
    for( auto it = conveyors_.begin(); it != conveyors_.end() && quantity > 0; ++it )
        quantity -= it->second->Convoy( *eventsObserver_, dotationCategory, quantity );

    // Allocate new conveyors
    if( autoAllocateNewTransporters_ )
    {
        while( quantity > 0 )
        {
            SupplyConveyor_ABC* conveyor = CreateConveyor( dotationCategory );
            if( !conveyor )
                return;
            quantity -= conveyor->Convoy( *eventsObserver_, dotationCategory, quantity );
        }
    }
    else if( quantity > 0 )
        impossible_ = true; // Conveyors selected manually are not enough to handle the request
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::IsImpossible
// Returns true if the convoy will never be able to be created for lack of transporters
// (when specified transporters are not enough to carry the quantity requested for instance).
// Created: LDC 2013-05-15
// -----------------------------------------------------------------------------
bool SupplyConvoy::IsImpossible() const
{
    return impossible_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::ReserveTransporters
// Created: NLD 2011-07-25
// -----------------------------------------------------------------------------
unsigned SupplyConvoy::ReserveTransporters( const T_Resources& resources )
{
    // Allocate the specified transporters
    BOOST_FOREACH( const auto& data, parameters_->GetTransporters() )
        for( unsigned i = 0; i < data.second; ++i )
            if( !CreateConveyor( *data.first ) )
                break;

    // Link the resources to the transporters
    BOOST_FOREACH( const T_Resources::value_type& data, resources )
        ReserveTransporters( *data.first, data.second );

    if( conveyors_.empty() )
        return std::numeric_limits< unsigned >::max();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::ComputeTimeRemainingForAction
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoy::ComputeTimeRemainingForAction( E_Action action, MT_InterpolatedFunction& timeComputer )
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
    for( auto it = conveyors_.begin(); it != conveyors_.end() && quantity > 0; ++it )
    {
        SupplyConveyor_ABC& conveyor = *it->second;
        quantity -= conveyor.Supply( dotationCategory, quantity );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::ResetConveyors
// Created: JSR 2013-02-11
// -----------------------------------------------------------------------------
void SupplyConvoy::ResetConveyors( SupplyConvoyEventsObserver_ABC& observer )
{
    for( auto it = conveyors_.begin(); it != conveyors_.end(); ++it )
        it->second->Destroy( observer );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Finish
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoy::Finish(  bool finished /*= true*/ )
{
    finished_ = finished;
    conveyors_.clear();
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::HasConvoy
// Created: JSR 2013-02-07
// -----------------------------------------------------------------------------
bool SupplyConvoy::HasConvoy( const MIL_AgentPion& /*pion*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::IsFinished
// Created: JSR 2013-02-19
// -----------------------------------------------------------------------------
bool SupplyConvoy::IsFinished() const
{
    return finished_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::IsConvoyDestroyed
// Created: JSR 2013-02-19
// -----------------------------------------------------------------------------
bool SupplyConvoy::IsConvoyDestroyed() const
{
    return false;
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
    return *transportersProvider_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::Finish
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
bool SupplyConvoy::CanTransport( const PHY_DotationCategory& dotationCategory ) const
{
    for( auto it = conveyors_.begin(); it != conveyors_.end(); ++it )
        if( it->second->CanTransport( dotationCategory ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::SetProvider
// Created: LDC 2013-01-08
// -----------------------------------------------------------------------------
void SupplyConvoy::SetProvider( const MIL_Agent_ABC* provider )
{
    provider_ = const_cast< MIL_Agent_ABC* >( provider );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::serialize
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
void SupplyConvoy::serialize( MIL_CheckPointOutArchive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< SupplyConvoy_ABC >( *this );
    archive & eventsObserver_;
    archive & parameters_;
    archive & supplier_;
    archive & transportersProvider_;
    archive & autoAllocateNewTransporters_;
    archive & conveyors_;
    archive & currentAction_;
    archive & timeRemainingForCurrentAction_;
    archive & currentSupplyRecipient_;
    archive & provider_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::serialize
// Created: LDC 2013-01-16
// -----------------------------------------------------------------------------
void SupplyConvoy::serialize( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< SupplyConvoy_ABC >( *this );
    archive & eventsObserver_;
    archive & parameters_;
    archive & supplier_;
    archive & transportersProvider_;
    archive & autoAllocateNewTransporters_;
    archive & conveyors_;
    archive & currentAction_;
    archive & timeRemainingForCurrentAction_;
    archive & currentSupplyRecipient_;
    archive & provider_;
}
