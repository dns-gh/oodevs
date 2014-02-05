// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MaintenanceRepairConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:57 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceRepairConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "PHY_MaintenanceRepairConsign.h"
#include "PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MaintenanceRepairConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::PHY_MaintenanceRepairConsign( MIL_Agent_ABC& maintenanceAgent, PHY_MaintenanceComposanteState& composanteState )
    : PHY_MaintenanceConsign_ABC( maintenanceAgent, composanteState )
    , pRepairer_                ( 0 )
{
    EnterStateWaitingForParts();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::PHY_MaintenanceRepairConsign()
    : PHY_MaintenanceConsign_ABC()
    , pRepairer_                ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::~PHY_MaintenanceRepairConsign()
{
    if( pRepairer_ )
    {
        GetPionMaintenance().StopUsingForLogistic( *pRepairer_ );
        pRepairer_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceRepairConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this )
         & pRepairer_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::Cancel()
{
    if( pRepairer_ )
    {
        GetPionMaintenance().StopUsingForLogistic( *pRepairer_ );
        pRepairer_ = 0;
    }
    PHY_MaintenanceConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoReturnComposante
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::DoReturnComposante()
{
    assert( pComposanteState_ );
    assert( !pRepairer_ );
    pComposanteState_->NotifyRepaired();
    pComposanteState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoWaitingForParts
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::DoWaitingForParts()
{
    assert( pComposanteState_ );
    assert( !pRepairer_ );
    return GetPionMaintenance().ConsumePartsForBreakdown( pComposanteState_->GetComposanteBreakdown() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::DoWaitingForRepairer()
{
    assert( pComposanteState_ );
    assert( !pRepairer_ );

    pRepairer_ = GetPionMaintenance().GetAvailableRepairer( pComposanteState_->GetComposanteBreakdown() );
    if( !pRepairer_ )
    {
        // Find alternative repair unit
        MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager();
        if( pLogisticManager && pComposanteState_ )
        {
            PHY_RoleInterface_Maintenance* newPion = pLogisticManager->MaintenanceFindAlternativeRepairHandler( *pComposanteState_ );
            if( newPion && newPion != &GetPionMaintenance() && newPion->HandleComposanteForRepair( *pComposanteState_ ) )
            {
                EnterStateFinished();
                pComposanteState_ = 0; // Crade
                return false;
            }
        }
        if( !GetPionMaintenance().HasUsableRepairer( pComposanteState_->GetComposanteBreakdown() ) )
            EnterStateWaitingForCarrier();
        return false;
    }
    GetPionMaintenance().StartUsingForLogistic( *pRepairer_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForCarrier
// Created: NLD 2006-08-16
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForCarrier()
{
    SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoSearchForCarrier
// Created: NLD 2006-08-16
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::DoSearchForCarrier()
{
    assert( pComposanteState_ );
    assert( !pRepairer_ );

    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MaintenanceHandleComposanteForTransport( *pComposanteState_ ) )
    {
        pComposanteState_ = 0;
        EnterStateFinished();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForParts
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForParts()
{
    assert( pComposanteState_ );
    SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_parts, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairerSelection
// Created: ABR 2014-02-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairerSelection()
{
    assert( pComposanteState_ );
    if( IsManualMode() )
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection, 0 );
    else
        EnterStateWaitingForRepairer();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer()
{
    assert( pComposanteState_ );
    SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_repairer, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateRepairing
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateRepairing()
{
    assert( pComposanteState_ );
    SetState( sword::LogMaintenanceHandlingUpdate::repairing,
        pComposanteState_->GetComposanteBreakdown().GetRepairTime() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateGoingBackToWar
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateGoingBackToWar()
{
    assert( pComposanteState_ );
    assert( pRepairer_ );

    GetPionMaintenance().StopUsingForLogistic( *pRepairer_ );
    pRepairer_ = 0;
    SetState( sword::LogMaintenanceHandlingUpdate::moving_back,
        pComposanteState_->ApproximateTravelTime(
            pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition(),
            pComposanteState_->GetPionPosition() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::Update()
{
    if( next_ )
    {
        next_();
        next_ = 0;
        return false;
    }
    if( DecrementTimer() )
        return GetState() == sword::LogMaintenanceHandlingUpdate::finished;
    switch( GetState() )
    {
        case sword::LogMaintenanceHandlingUpdate::waiting_for_transporter:
            if( DoSearchForCarrier() )
                EnterStateFinished();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_parts:
            if( DoWaitingForParts() )
                EnterStateWaitingForRepairerSelection();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_repairer:
            if( DoWaitingForRepairer() )
                EnterStateRepairing();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection:
            if( !IsManualMode() )
                EnterStateWaitingForRepairer();
            break;
        case sword::LogMaintenanceHandlingUpdate::repairing:
            EnterStateGoingBackToWar();
            break;
        case sword::LogMaintenanceHandlingUpdate::moving_back:
            DoReturnComposante();
            EnterStateFinished();
            break;
        case sword::LogMaintenanceHandlingUpdate::finished:
            break;
        default:
            assert( false );
    }
    return GetState() == sword::LogMaintenanceHandlingUpdate::finished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::SelectNewState
// Created: MCO 2014-01-30
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::SelectNewState()
{
    if( GetState() == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
        next_ = [&]() { EnterStateWaitingForRepairer(); };
}

void PHY_MaintenanceRepairConsign::TransferToLogisticSuperior()
{
    throw MASA_EXCEPTION( "cannot treansfer a repair consign to superior" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::SelectMaintenanceTransporter
// Created: SLI 2014-01-30
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::SelectMaintenanceTransporter( const PHY_ComposanteTypePion& /*type*/ )
{
    throw MASA_EXCEPTION( "cannot select a transporter for a repair consign" );
}

void PHY_MaintenanceRepairConsign::SelectDiagnosisTeam( const PHY_ComposanteTypePion& /*type*/ )
{
    throw MASA_EXCEPTION( "cannot select a diagnosis team for a repair consign" );
}
