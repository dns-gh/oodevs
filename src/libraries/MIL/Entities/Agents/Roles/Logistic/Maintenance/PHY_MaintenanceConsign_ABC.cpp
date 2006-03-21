// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceConsign_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:55 $
// $Revision: 7 $
// $Workfile: PHY_MaintenanceConsign_ABC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MaintenanceConsign_ABC.h"

#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Logistic/PHY_LogWorkTime.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::PHY_MaintenanceConsign_ABC( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState )
    : pMaintenance_    ( &maintenance )
    , pComposanteState_( &composanteState )
    , nTimer_          ( 0 )
    , pActor_          ( 0 )
    , bHasChanged_     ( true )
{
    pComposanteState_->SetConsign( this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::PHY_MaintenanceConsign_ABC()
    : pMaintenance_    ( 0 )
    , pComposanteState_( 0 )
    , pActor_          ( 0 )
    , bHasChanged_     ( true )
    , nTimer_          ( 0 )
{
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

// =============================================================================
// ACCESSORS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetComposanteType
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
const PHY_ComposanteTypePion& PHY_MaintenanceConsign_ABC::GetComposanteType() const
{
    assert( pComposanteState_ );
    return pComposanteState_->GetComposanteType();
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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::Cancel()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    
    if ( pActor_ )
    {
        pMaintenance_->StopUsingForLogistic( *pActor_ );
        pActor_ = 0;
    }
    SetState( eFinished );
    nTimer_ = 0;
    pComposanteState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::DoWaitingForCarrier
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::DoWaitingForCarrier()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    assert( !pActor_ );
    
    nTimer_ = 0;
    pActor_ = pMaintenance_->GetAvailableHauler( pComposanteState_->GetComposanteType() );
    if( pActor_ )
        pMaintenance_->StartUsingForLogistic( *pActor_ );
    return pActor_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::DoSearchingForUpperLevel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::DoSearchingForUpperLevel()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    assert( !pActor_ );
    
    nTimer_ = 0;
    MIL_AutomateLOG* pMaintenanceSuperior = pMaintenance_->GetAutomate().GetMaintenanceSuperior();
    if( !pMaintenanceSuperior )
        return true; // Si pas de supérieur => la 'composante state' ne sera gérée par aucun pion, mais sera toujours active

    if( pMaintenanceSuperior->MaintenanceHandleComposanteForTransport( *pComposanteState_ ) )
    {
        pComposanteState_ = 0;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::DoWaitingForGoingBackToWar
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::DoWaitingForGoingBackToWar()
{
    assert( pComposanteState_ );
    assert( !pActor_ );
    nTimer_ = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::DoReturnComposante
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::DoReturnComposante()
{
    assert( pComposanteState_ );
    assert( !pActor_ );
    nTimer_ = 0;
    pComposanteState_->NotifyRepaired();
    pComposanteState_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::DoWaitingForParts
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::DoWaitingForParts()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    assert( !pActor_ );
    
    nTimer_ = 0;
    return pMaintenance_->ConsumePartsForBreakdown( pComposanteState_->GetComposanteBreakdown() );
}
 
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::DoWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceConsign_ABC::DoWaitingForRepairer()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    assert( !pActor_ );
    
    nTimer_ = 0;
    pActor_ = pMaintenance_->GetAvailableRepairer( pComposanteState_->GetComposanteBreakdown() );
    if ( pActor_ )
        pMaintenance_->StartUsingForLogistic( *pActor_ );
    return pActor_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateWaitingForCarrier
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateWaitingForCarrier()
{
    assert( pComposanteState_ );
    assert( !pActor_ );

    SetState( eWaitingForCarrier );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateGoingFrom
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateGoingFrom()
{
    assert( pComposanteState_ );
    assert( !pActor_ );
    
    SetState( eGoingFrom );
    nTimer_ = pComposanteState_->ApproximateTravelTime( pComposanteState_->GetComposantePosition(), pMaintenance_->GetPosition() );
    pComposanteState_->NotifyHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateCarrierGoingTo
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateCarrierGoingTo()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    assert( pActor_ );
    
    SetState( eCarrierGoingTo );
    nTimer_ = pActor_->ApproximateTravelTime( pMaintenance_->GetPosition(), pComposanteState_->GetComposantePosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateCarrierLoading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateCarrierLoading()
{
    assert( pComposanteState_ );
    assert( pActor_ );
    
    SetState( eCarrierLoading );
    nTimer_ = (int)( pComposanteState_->GetComposanteWeight() / pActor_->GetType().GetHaulWeightLoadedPerTimeStep() );
    pComposanteState_->NotifyHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateCarrierGoingFrom
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateCarrierGoingFrom()
{
    assert( pComposanteState_ );
    assert( pActor_ );
    
    SetState( eCarrierGoingFrom );
    nTimer_ = pActor_->ApproximateTravelTime( pComposanteState_->GetComposantePosition(), pMaintenance_->GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateCarrierUnloading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateCarrierUnloading()
{
    assert( pComposanteState_ );
    assert( pActor_ );
    
    SetState( eCarrierUnloading );
    nTimer_ = (int)( pComposanteState_->GetComposanteWeight() / pActor_->GetType().GetHaulWeightUnloadedPerTimeStep() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateDiagnosing
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateDiagnosing()
{
    assert( pComposanteState_ );
    
    if ( pActor_ )
    {
        assert( pMaintenance_ );
        pMaintenance_->StopUsingForLogistic( *pActor_ );
        pActor_ = 0;
    }    
    
    if ( pComposanteState_->NeedDiagnosis() )
    {
        SetState( eDiagnosing );
        nTimer_ = PHY_BreakdownType::GetDiagnosticTime();    
    }
    else
        ChooseStateAfterDiagnostic();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::ChooseStateAfterDiagnostic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::ChooseStateAfterDiagnostic()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    
    pComposanteState_->NotifyDiagnosed();
    pComposanteState_->SetComposantePosition( pMaintenance_->GetPosition() );
    nTimer_ = 0;

    if ( pMaintenance_->GetAutomate().MaintenanceHandleComposanteForRepair( *pComposanteState_ ) )
    {
        pComposanteState_ = 0;
        SetState( eFinished ); // Managed by a 'repair consign'
    }
    else
        SetState( eSearchingForUpperLevel );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateWaitingForParts
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateWaitingForParts()
{
    assert( pComposanteState_ );
    SetState( eWaitingForParts );
    nTimer_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateWaitingForRepairer()
{
    assert( pComposanteState_ );
    SetState( eWaitingForRepairer );
    nTimer_ = 0;    
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateRepairing
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateRepairing()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    
    SetState( eRepairing );
    nTimer_ = (int)( pComposanteState_->GetComposanteBreakdown().GetRepairTime() * pMaintenance_->GetWorkTime().GetRepairTimeFactor() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateWaitingForGoingBackToWar
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateWaitingForGoingBackToWar()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    assert( pActor_ );
    
    pMaintenance_->StopUsingForLogistic( *pActor_ );
    pActor_ = 0;
    nTimer_ = 0;
    SetState( eWaitingForGoingBackToWar );    
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateGoingBackToWar
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateGoingBackToWar()
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    assert( !pActor_ );
    
    SetState( eGoingBackToWar );
    nTimer_ = pComposanteState_->ApproximateTravelTime( pMaintenance_->GetPosition(), pComposanteState_->GetPionPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateFinished
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateFinished()
{
    assert( !pActor_ );
    SetState( eFinished );
    nTimer_ = 0;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SendFullState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::SendFullState( NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate& asn ) const
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    
    asn.GetAsnMsg().oid_pion_log_traitant = pMaintenance_->GetPion().GetID();
    asn.GetAsnMsg().m.etatPresent         = 1;
    asn.GetAsnMsg().etat                  = (ASN1T_EnumLogMaintenanceTraitementEtat)nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::SendChangedState( NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate& asn ) const
{
    assert( pComposanteState_ );
    if( bHasChanged_ )
        SendFullState( asn );
}
