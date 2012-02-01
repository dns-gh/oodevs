//  *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConvoyReal.h"
#include "SupplyConveyor_ABC.h"
#include "SupplyConvoyConfig.h"
#include "SupplySupplier_ABC.h"
#include "SupplyRequestParameters_ABC.h"
#include "SupplyConvoyEventsObserver_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "SpeedComputer_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace logistic;

// =============================================================================
// Constructor / destructor 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal constructor
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplyConvoyReal::SupplyConvoyReal( SupplyConvoyEventsObserver_ABC& eventsObserver, SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters )
    : SupplyConvoy      ( eventsObserver, supplier, parameters )
    , convoyPion_       ( 0 )
    , currentActionDone_( false )
{
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal destructor
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplyConvoyReal::~SupplyConvoyReal()
{
    if( convoyPion_ )
    {
        transportersProvider_.SupplyDestroyConvoyPion( *convoyPion_ );
        convoyPion_ = 0;
    }
}

// =============================================================================
// Operations
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::Setup
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyReal::Setup()
{
    if( !convoyPion_ )
    {
        convoyPion_ = transportersProvider_.SupplyCreateConvoyPion( *SupplyConvoyConfig::convoyAgentType_, shared_from_this() );
        BOOST_FOREACH( T_Conveyors::value_type& data, conveyors_ )
            data.second->LendTo( *convoyPion_ );
        convoyPion_->GetOrderManager().OnReceiveMission( *SupplyConvoyConfig::convoyMissionType_ );
    }
    currentActionDone_ = false;
    return SupplyConvoy::Setup();
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::MoveToSupplier
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyReal::ReturnTimeRemainingForAction( E_Action action )
{
    if( currentAction_ != action )
    {
        currentAction_ = action;
        currentActionDone_ = false;
    }
    return currentActionDone_ ? 0 : std::numeric_limits< unsigned >::max();
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::MoveToSupplier
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyReal::MoveToSupplier()
{
    return ReturnTimeRemainingForAction( eMoveToSupplier );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::MoveToSupplyRecipient
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyReal::MoveToSupplyRecipient()
{
    return ReturnTimeRemainingForAction( eMoveToSupplyRecipient );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::MoveToTransportersProvider
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
unsigned SupplyConvoyReal::MoveToTransportersProvider()
{
    return ReturnTimeRemainingForAction( eMoveToTransportersProvider );
}

// =============================================================================
// Events
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::NotifyMovedToSupplier
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::NotifyMovedToSupplier()
{
    if( currentAction_ == eMoveToSupplier )
        currentActionDone_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::NotifyMovedToTransportersProvider
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::NotifyMovedToTransportersProvider()
{
    if( currentAction_ == eMoveToTransportersProvider )
        currentActionDone_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::NotifyMovedToSupplyRecipient
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::NotifyMovedToSupplyRecipient()
{
    if( currentAction_ == eMoveToSupplyRecipient )
        currentActionDone_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::NotifyMovedToSupplyRecipient
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::NotifyTransporterDestroyed( PHY_ComposantePion& transporter )
{
    T_Conveyors::const_iterator it = conveyors_.find( &transporter );
    if( it != conveyors_.end() )
        it->second->Destroy( eventsObserver_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::NotifyConvoyEndMission
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::NotifyConvoyEndMission()
{
    eventsObserver_.OnConvoyEndMission();
}

// =============================================================================
// Algorithms
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::NotifyMovedToSupplyRecipient
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    algorithm.AddModifier( SupplyConvoyConfig::coefSpeedModificator_( static_cast< double >( conveyors_.size() ) ), false );
}

// =============================================================================
// Accessors
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::GetCurrentAction
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
int SupplyConvoyReal::GetCurrentAction() const
{
    return currentAction_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::GetCurrentSupplyRecipient
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplyRecipient_ABC* SupplyConvoyReal::GetCurrentSupplyRecipient() const
{
    return currentSupplyRecipient_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::GetSupplier
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplySupplier_ABC& SupplyConvoyReal::GetSupplier() const
{
    return supplier_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::GetTransportersProvider
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
SupplySupplier_ABC& SupplyConvoyReal::GetTransportersProvider() const
{
    return transportersProvider_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::GetCurrentAction
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
const T_PointVector* SupplyConvoyReal::GetPathToNextDestination() const
{
    switch( currentAction_ )
    {
        case eMoveToSupplier: return &parameters_.GetPathToSupplier();
        case eMoveToTransportersProvider: return &parameters_.GetPathToTransportersProvider();
        case eMoveToSupplyRecipient: assert( currentSupplyRecipient_ ); return &parameters_.GetPathToRecipient( *currentSupplyRecipient_ );
        default:
            return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyReal::Finish
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::Finish()
{
    SupplyConvoy::Finish();
    if( convoyPion_ )
    {
        transportersProvider_.SupplyDestroyConvoyPion( *convoyPion_ );
        convoyPion_ = 0;
    }
}

// =============================================================================
// Network
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SupplyConvoy::MoveToTransportersProvider
// Created: NLD 2011-08-01
// -----------------------------------------------------------------------------
void SupplyConvoyReal::Serialize( sword::UnitId& msg ) const
{
    if( convoyPion_ )
        msg.set_id( convoyPion_->GetID() );
    else
        msg.set_id( 0 );
}
