// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalConsign_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 15:28 $
// $Revision: 5 $
// $Workfile: PHY_MedicalConsign_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_MedicalConsign_ABC.h"

#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "MIL_AgentServer.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalConsign_ABC::PHY_MedicalConsign_ABC( PHY_RoleInterface_Medical& medical, PHY_MedicalHumanState& humanState )
    : pMedical_               ( &medical )
    , pHumanState_            ( &humanState )
    , nTimer_                 ( 0 )
    , currentStateEndTimeStep_( std::numeric_limits< unsigned >::max() )
    , bHasChanged_            ( true )
    , nState_                 ( eWaitingForEvacuation )
{
    pHumanState_->SetConsign( this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalConsign_ABC::PHY_MedicalConsign_ABC()
    : pMedical_               ( 0 )
    , pHumanState_            ( 0 )
    , nTimer_                 ( 0 )
    , currentStateEndTimeStep_( std::numeric_limits< unsigned >::max() )
    , bHasChanged_            ( true )
    , nState_                 ( eWaitingForEvacuation )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalConsign_ABC::~PHY_MedicalConsign_ABC()
{
    if( pHumanState_ )
        pHumanState_->SetConsign( 0 );
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::IsAnEmergency
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalConsign_ABC::IsAnEmergency() const
{
    assert( pHumanState_ );
    return pHumanState_->IsAnEmergency();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::Cancel()
{
    assert( pHumanState_ );
    SetState( eFinished );
    ResetTimer( 0 );
    pHumanState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::EnterStateFinished
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::EnterStateFinished()
{
    SetState( eFinished );
    ResetTimer( 0 );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::SendFullState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::SendFullState( client::LogMedicalHandlingUpdate& asn ) const
{
    assert( pHumanState_ );
    assert( pMedical_ );
    asn().mutable_provider()->set_id( pMedical_->GetPion().GetID() );
    asn().set_state( sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus( nState_ ) );
    asn().set_current_state_end_tick( currentStateEndTimeStep_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::SendChangedState( client::LogMedicalHandlingUpdate& asn ) const
{
    assert( pHumanState_ );
    if( bHasChanged_ )
        SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::Clean
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::SetTimer
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::SetState( E_State nNewState )
{
    nState_ = nNewState;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
PHY_MedicalConsign_ABC::E_State PHY_MedicalConsign_ABC::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::ResetTimer
// Created: NLD 2012-01-02
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::SendExternalTimerValue( int timer )
{
    unsigned tmp = std::numeric_limits< unsigned >::max();
    if( timer > 0 )
        tmp = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + timer;
    if( tmp != currentStateEndTimeStep_ )
    {
        currentStateEndTimeStep_ = tmp;
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::ResetTimer
// Created: NLD 2012-01-02
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::ResetTimer( int timer )
{
    nTimer_ = timer;
    SendExternalTimerValue( timer );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::DecrementTimer
// Created: NLD 2012-01-02
// -----------------------------------------------------------------------------
bool PHY_MedicalConsign_ABC::DecrementTimer()
{
    return --nTimer_ > 0;
}
// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::GetHumanState
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
const PHY_MedicalHumanState& PHY_MedicalConsign_ABC::GetHumanState() const
{
    assert( pHumanState_ );
    return *pHumanState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::HasChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
bool PHY_MedicalConsign_ABC::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::IsFinished
// Created: NLD 2005-03-08
// -----------------------------------------------------------------------------
bool PHY_MedicalConsign_ABC::IsFinished() const
{
    return nState_ == eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::GetPionMedical
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_RoleInterface_Medical& PHY_MedicalConsign_ABC::GetPionMedical() const
{
    assert( pMedical_ );
    return *pMedical_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetPionMaintenance
// Created: NLD 2006-08-11
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::FinishSuccessfullyWithoutDelay()   
{
    if( pHumanState_ )
    {
        pHumanState_->NotifyHandledByMedical();
        Human_ABC& human = const_cast< Human_ABC& >( pHumanState_->GetHuman() ); //$$$ BEURK
        human.HealContamination();
        human.HealWound();
        human.HealMentalDisease();
        // NB : DO NOT CALL "pHumanState_ = 0; - The consign stays in the last unit forever in the "finished" state
    }
    EnterStateFinished();
}
