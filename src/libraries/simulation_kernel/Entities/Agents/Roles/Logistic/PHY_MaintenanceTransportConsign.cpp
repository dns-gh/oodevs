// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MaintenanceTransportConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:58 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceTransportConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "PHY_MaintenanceTransportConsign.h"
#include "PHY_RoleInterface_Maintenance.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MaintenanceTransportConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::PHY_MaintenanceTransportConsign( MIL_Agent_ABC& maintenanceAgent, PHY_MaintenanceComposanteState& composanteState )
    : PHY_MaintenanceConsign_ABC( maintenanceAgent, composanteState )
    , pCarrier_                 ( 0 )
    , searchForUpperLevelDone_  ( false )
    , forceTransferToLogisticSuperior_( false )
{
    const PHY_Breakdown& breakdown = composanteState.GetComposanteBreakdown();

    if( breakdown.AffectMobility() )
        EnterStateWaitingForCarrier();
    else
        EnterStateGoingFrom();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::PHY_MaintenanceTransportConsign()
    : PHY_MaintenanceConsign_ABC()
    , pCarrier_                 ( 0 )
    , searchForUpperLevelDone_  ( false )
    , forceTransferToLogisticSuperior_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::~PHY_MaintenanceTransportConsign()
{
    if( pCarrier_ )
    {
        GetPionMaintenance().StopUsingForLogistic( *pCarrier_ );
        pCarrier_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceTransportConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this )
         & pCarrier_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::Cancel()
{
    if( pCarrier_ )
    {
        GetPionMaintenance().StopUsingForLogistic( *pCarrier_ );
        pCarrier_ = 0;
    }
    PHY_MaintenanceConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::DoWaitingForCarrier
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::DoWaitingForCarrier()
{
    assert( pComposanteState_ );
    assert( !pCarrier_ );

    ResetTimer( 0 );
    pCarrier_ = GetPionMaintenance().GetAvailableHauler( GetComposanteType() );
    if( pCarrier_ )
        GetPionMaintenance().StartUsingForLogistic( *pCarrier_ );
    else
    {
        // Find alternative transport unit
        MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager();
        if( pLogisticManager )
        {
            PHY_RoleInterface_Maintenance* newPion = pLogisticManager->MaintenanceFindAlternativeTransportHandler( *pComposanteState_ );
            if( newPion && newPion != &GetPionMaintenance() && newPion->HandleComposanteForTransport( *pComposanteState_ ) )
            {
                EnterStateFinished();
                pComposanteState_ = 0; // Crade
                return false;
            }
        }
    }
    return pCarrier_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::DoSearchForUpperLevel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::DoSearchForUpperLevel()
{
    assert( pComposanteState_ );
    assert( !pCarrier_ );

    searchForUpperLevelDone_ = true;
    ResetTimer( 0 );
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().FindLogisticManager();
    if( !pLogisticManager )
        return false;
    MIL_AutomateLOG* pLogisticSuperior = pLogisticManager->GetLogisticHierarchy().GetPrimarySuperior();
    if( pLogisticSuperior && pLogisticSuperior->MaintenanceHandleComposanteForTransport( *pComposanteState_ ) )
    {
        pComposanteState_ = 0;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateWaitingForCarrier
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateWaitingForCarrier()
{
    assert( pComposanteState_ );
    assert( !pCarrier_ );

    if( IsManualMode() )
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection );
    else
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateGoingFrom
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateGoingFrom()
{
    assert( pComposanteState_ );
    assert( !pCarrier_ );

    SetState( sword::LogMaintenanceHandlingUpdate::moving_to_supply );
    ResetTimer( pComposanteState_->ApproximateTravelTime( pComposanteState_->GetComposantePosition(), pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition() ) );
    pComposanteState_->NotifyHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierGoingTo
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierGoingTo()
{
    assert( pComposanteState_ );
    assert( pCarrier_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_moving_to_supply );
    ResetTimer( pCarrier_->ApproximateTravelTime( pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition(), pComposanteState_->GetComposantePosition() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierLoading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierLoading()
{
    assert( pComposanteState_ );
    assert( pCarrier_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_loading );
    ResetTimer( static_cast< int >( pCarrier_->GetType().GetHaulerLoadingTime() ));
    pComposanteState_->NotifyHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierGoingFrom
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierGoingFrom()
{
    assert( pComposanteState_ );
    assert( pCarrier_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_moving_back );
    ResetTimer( pCarrier_->ApproximateTravelTime( pComposanteState_->GetComposantePosition(), pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierUnloading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierUnloading()
{
    assert( pComposanteState_ );
    assert( pCarrier_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_unloading );
    ResetTimer( static_cast< int >( pCarrier_->GetType().GetHaulerUnloadingTime() ));
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateDiagnosing
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateDiagnosing()
{
    assert( pComposanteState_ );

    if( pCarrier_ )
    {
        GetPionMaintenance().StopUsingForLogistic( *pCarrier_ );
        pCarrier_ = 0;
    }

    if( pComposanteState_->NeedDiagnosis() )
    {
        SetState( sword::LogMaintenanceHandlingUpdate::diagnosing );
        ResetTimer( PHY_BreakdownType::GetDiagnosticTime() );
    }
    else
        ChooseStateAfterDiagnostic();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::ChooseStateAfterDiagnostic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::ChooseStateAfterDiagnostic()
{
    assert( pComposanteState_ );

    pComposanteState_->NotifyDiagnosed();
    pComposanteState_->SetComposantePosition( pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition() );
    ResetTimer( 0 );

    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MaintenanceHandleComposanteForRepair( *pComposanteState_ ) )
    {
        pComposanteState_ = 0;
        SetState( sword::LogMaintenanceHandlingUpdate::finished ); // Managed by a 'repair consign'
    }
    else
        SetState( sword::LogMaintenanceHandlingUpdate::searching_upper_levels );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::Update()
{
    if( DecrementTimer() )
        return GetState() == sword::LogMaintenanceHandlingUpdate::finished;

    switch( GetState() )
    {
        case sword::LogMaintenanceHandlingUpdate::moving_to_supply:
            EnterStateDiagnosing();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_transporter:
            if( DoWaitingForCarrier() )
                EnterStateCarrierGoingTo();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection:
            if( forceTransferToLogisticSuperior_ )
            {
                SetState( sword::LogMaintenanceHandlingUpdate::searching_upper_levels );
                forceTransferToLogisticSuperior_ = false;
            }
            else
                EnterStateWaitingForCarrier();
            break;
        case sword::LogMaintenanceHandlingUpdate::transporter_moving_to_supply:
            EnterStateCarrierLoading();
            break;
        case sword::LogMaintenanceHandlingUpdate::transporter_loading:
            EnterStateCarrierGoingFrom();
            break;
        case sword::LogMaintenanceHandlingUpdate::transporter_moving_back:
            EnterStateCarrierUnloading();
            break;
        case sword::LogMaintenanceHandlingUpdate::transporter_unloading:
            EnterStateDiagnosing();
            break;
        case sword::LogMaintenanceHandlingUpdate::diagnosing:
            ChooseStateAfterDiagnostic();
            break;
        case sword::LogMaintenanceHandlingUpdate::searching_upper_levels:
            if( DoSearchForUpperLevel() )
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
// Name: PHY_MaintenanceTransportConsign::SearchForUpperLevelNotFound
// Created: MMC 2012-11-15
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::SearchForUpperLevelNotFound() const
{
    return GetState() == sword::LogMaintenanceHandlingUpdate::searching_upper_levels && searchForUpperLevelDone_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::SelectNewState
// Created: MCO 2014-01-24
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::SelectNewState()
{
    if( GetState() != sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
        return;
    SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter );

}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::IsManualMode
// Created: SLI 2014-01-30
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::IsManualMode() const
{
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().FindLogisticManager();
    return pLogisticManager && pLogisticManager->IsMaintenanceManual();
}

void PHY_MaintenanceTransportConsign::TransferToLogisticSuperior()
{
    if( GetState() != sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
        return;
    forceTransferToLogisticSuperior_ = true;
}
