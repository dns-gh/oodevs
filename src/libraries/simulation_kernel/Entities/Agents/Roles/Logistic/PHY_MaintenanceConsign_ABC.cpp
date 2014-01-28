// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MaintenanceConsign_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:55 $
// $Revision: 7 $
// $Workfile: PHY_MaintenanceConsign_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaintenanceConsign_ABC.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "MIL_Time_ABC.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::PHY_MaintenanceConsign_ABC(  MIL_Agent_ABC& maintenanceAgent, PHY_MaintenanceComposanteState& composanteState )
    : nState_( sword::LogMaintenanceHandlingUpdate::moving_to_supply )
    , pMaintenance_( &maintenanceAgent )
    , pComposanteState_( &composanteState )
    , nTimer_( 0 )
    , currentStateEndTimeStep_( std::numeric_limits< unsigned >::max() )
    , bHasChanged_( true )
{
    pComposanteState_->SetConsign( this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::PHY_MaintenanceConsign_ABC()
    : nState_( sword::LogMaintenanceHandlingUpdate::moving_to_supply )
    , pMaintenance_( 0 )
    , pComposanteState_( 0 )
    , nTimer_( 0 )
    , currentStateEndTimeStep_ ( std::numeric_limits< unsigned >::max() )
    , bHasChanged_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::~PHY_MaintenanceConsign_ABC()
{
    if( pComposanteState_ )
        pComposanteState_->SetConsign( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetComposanteType
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
const PHY_ComposanteTypePion& PHY_MaintenanceConsign_ABC::GetComposanteType() const
{
    assert( pComposanteState_ );
    return pComposanteState_->GetComposante().GetType();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetComposanteBreakdown
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const PHY_Breakdown& PHY_MaintenanceConsign_ABC::GetComposanteBreakdown() const
{
    assert( pComposanteState_ );
    return pComposanteState_->GetComposanteBreakdown();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::Cancel()
{
    assert( pComposanteState_ );
    SetState( sword::LogMaintenanceHandlingUpdate::finished );
    ResetTimer( 0 );
    pComposanteState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateFinished
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateFinished()
{
    SetState( sword::LogMaintenanceHandlingUpdate::finished );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SendFullState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::SendFullState( client::LogMaintenanceHandlingUpdate& asn ) const
{
    assert( pMaintenance_ );
    asn().mutable_provider()->set_id( pMaintenance_->GetID() );
    asn().set_state( nState_ );
    if( currentStateEndTimeStep_ != std::numeric_limits< unsigned int >::max() )
        asn().set_current_state_end_tick( currentStateEndTimeStep_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::SendChangedState( client::LogMaintenanceHandlingUpdate& asn ) const
{
    if( bHasChanged_ )
        SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::Clean
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::SetState( sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus nNewState )
{
    nState_ = nNewState;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::ResetTimer
// Created: NLD 2012-01-02
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::ResetTimer( int timer )
{
    nTimer_ = timer;
    unsigned tmp = std::numeric_limits< unsigned >::max();
    if( timer > 0 )
        tmp = MIL_Time_ABC::GetTime().GetCurrentTimeStep() + timer;
    if( tmp != currentStateEndTimeStep_ )
    {
        currentStateEndTimeStep_ = tmp;
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus PHY_MaintenanceConsign_ABC::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::DecrementTimer
// Created: NLD 2012-01-02
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::DecrementTimer()
{
    return --nTimer_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::HasChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::IsFinished
// Created: NLD 2005-03-08
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::IsFinished() const
{
    return nState_ == sword::LogMaintenanceHandlingUpdate::finished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetComposanteState
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
const PHY_MaintenanceComposanteState& PHY_MaintenanceConsign_ABC::GetComposanteState() const
{
    assert( pComposanteState_ );
    return *pComposanteState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::HasValidComposanteState
// Created: JSR 2012-07-13
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::HasValidComposanteState() const
{
    return pComposanteState_ != 0 ;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetPionMaintenance
// Created: NLD 2006-08-11
// -----------------------------------------------------------------------------
PHY_RoleInterface_Maintenance& PHY_MaintenanceConsign_ABC::GetPionMaintenance() const
{
    assert( pMaintenance_ );
    return pMaintenance_->GetRole< PHY_RoleInterface_Maintenance >();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::FinishSuccessfullyWithoutDelay
// Created: NLD 2006-08-11
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::FinishSuccessfullyWithoutDelay()
{
    if( pComposanteState_ )
        pComposanteState_->NotifyRepaired();
    pComposanteState_ = 0;
    EnterStateFinished();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::ClearConsign
// Created: JSR 2013-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::ClearConsign()
{
    if( pComposanteState_ )
        const_cast< PHY_ComposantePion& >( pComposanteState_->GetComposante() ).DeleteMaintenanceState();
    pComposanteState_ = 0;
}
