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
#include "OnComponentComputer_ABC.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes//PHY_RolePion_Composantes.h"
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
    , component_                ( 0 )
    , searchForUpperLevelDone_  ( false )
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
    : component_              ( 0 )
    , searchForUpperLevelDone_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::~PHY_MaintenanceTransportConsign()
{
    ResetComponent();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceTransportConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this )
         & component_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::Cancel()
{
    ResetComponent();
    PHY_MaintenanceConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::FindAlternativeTransportUnit
// Created: SLI 2014-02-11
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::FindAlternativeTransportUnit( const PHY_ComposanteTypePion* type )
{
    if( MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager() )
    {
        PHY_RoleInterface_Maintenance* newPion = pLogisticManager->MaintenanceFindAlternativeTransportHandler( *pComposanteState_, type );
        if( newPion && newPion != &GetPionMaintenance() && newPion->HandleComposanteForTransport( *pComposanteState_ ) )
        {
            if( type )
            {
                pComposanteState_->GetConsign()->SetState( GetState(), 0 );
                pComposanteState_->SelectMaintenanceTransporter( *type );
            }
            EnterStateFinished();
            pComposanteState_ = 0; // Crade
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::DoWaitingForCarrier
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::DoWaitingForCarrier()
{
    assert( pComposanteState_ );
    assert( !component_ );

    component_ = GetPionMaintenance().GetAvailableHauler( GetComposanteType() );
    if( component_ )
        GetPionMaintenance().StartUsingForLogistic( *component_ );
    else
    {
        FindAlternativeTransportUnit();
        return false;
    }
    return component_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::DoWaitingForCarrierSelection
// Created: SLI 2014-02-03
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::DoWaitingForCarrierSelection()
{
    if( !IsManualMode() )
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::DoSearchForUpperLevel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::DoSearchForUpperLevel()
{
    assert( pComposanteState_ );
    assert( !component_ );

    searchForUpperLevelDone_ = true;
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
    if( IsManualMode() )
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection, 0 );
    else
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter, 0 );
}

void PHY_MaintenanceTransportConsign::ResetComponent()
{
    if( component_ )
        GetPionMaintenance().StopUsingForLogistic( *component_ );
    component_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateGoingFrom
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateGoingFrom()
{
    assert( pComposanteState_ );
    SetState( sword::LogMaintenanceHandlingUpdate::moving_to_supply,
        pComposanteState_->ApproximateTravelTime(
            pComposanteState_->GetComposantePosition(),
            pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition() ) );
    pComposanteState_->NotifyHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierGoingTo
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierGoingTo()
{
    assert( pComposanteState_ );
    assert( component_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_moving_to_supply,
        component_->ApproximateTravelTime(
            pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition(),
            pComposanteState_->GetComposantePosition() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierLoading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierLoading()
{
    assert( pComposanteState_ );
    assert( component_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_loading,
        static_cast< int >( component_->GetType().GetHaulerLoadingTime() ) );
    pComposanteState_->NotifyHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierGoingFrom
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierGoingFrom()
{
    assert( pComposanteState_ );
    assert( component_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_moving_back,
        component_->ApproximateTravelTime(
            pComposanteState_->GetComposantePosition(),
            pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierUnloading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierUnloading()
{
    assert( pComposanteState_ );
    assert( component_ );

    SetState( sword::LogMaintenanceHandlingUpdate::transporter_unloading,
        static_cast< int >( component_->GetType().GetHaulerUnloadingTime() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::Update()
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
        case sword::LogMaintenanceHandlingUpdate::moving_to_supply:
            ChooseStateAfterTransport();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_transporter:
            if( DoWaitingForCarrier() )
                EnterStateCarrierGoingTo();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection:
            DoWaitingForCarrierSelection();
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
            ChooseStateAfterTransport();
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
// Name: PHY_MaintenanceTransportConsign::ChooseStateAfterTransport
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::ChooseStateAfterTransport()
{
    ResetComponent();
    assert( pComposanteState_ );
    pComposanteState_->SetComposantePosition( pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition() );
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MaintenanceHandleComposanteForDiagnosis( *pComposanteState_ ) )
    {
        pComposanteState_ = 0;
        EnterStateFinished(); // Managed by a 'diagnosis consign'
    }
    else
        next_ = [&]() { SetState( sword::LogMaintenanceHandlingUpdate::searching_upper_levels, 0 ); };
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::SearchForUpperLevelNotFound
// Created: MMC 2012-11-15
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::SearchForUpperLevelNotFound() const
{
    return GetState() == sword::LogMaintenanceHandlingUpdate::searching_upper_levels && searchForUpperLevelDone_;
}

void PHY_MaintenanceTransportConsign::SelectNewState()
{
    if( GetState() == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
        next_ = [&]() { SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter, 0 ); };
    else
        throw MASA_EXCEPTION( "transport consign not in a waiting state" );
}

void PHY_MaintenanceTransportConsign::TransferToLogisticSuperior()
{
    const auto state = GetState();
    if( state != sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
        throw MASA_EXCEPTION( "transport consign is not in a waiting state" );
    next_ = [&]() { SetState( sword::LogMaintenanceHandlingUpdate::searching_upper_levels, 0 ); };
}

void PHY_MaintenanceTransportConsign::SelectMaintenanceTransporter( const PHY_ComposanteTypePion& type )
{
    if( GetState() != sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
        throw MASA_EXCEPTION( "transport consign not in a waiting for transporter selection state" );
    component_ = GetPionMaintenance().GetAvailableHauler( GetComposanteType(), &type );
    if( component_ )
    {
        GetPionMaintenance().StartUsingForLogistic( *component_ );
        next_ = [&]() { EnterStateCarrierGoingTo(); };
    }
    else if( !FindAlternativeTransportUnit( &type ) )
        throw MASA_EXCEPTION( "no component of specified type available for maintenance transporter selection" );
}

void PHY_MaintenanceTransportConsign::SelectDiagnosisTeam( const PHY_ComposanteTypePion& /*type*/ )
{
    throw MASA_EXCEPTION( "cannot select a diagnosis team for a transport consign" );
}

void PHY_MaintenanceTransportConsign::SelectRepairTeam( const PHY_ComposanteTypePion& /*type*/ )
{
    throw MASA_EXCEPTION( "cannot select a repair team for a transport consign" );
}
