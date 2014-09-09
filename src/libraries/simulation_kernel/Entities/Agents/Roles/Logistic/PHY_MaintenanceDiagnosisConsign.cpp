// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaintenanceDiagnosisConsign.h"
#include "PHY_RoleInterface_Maintenance.h"
#include "PHY_RoleInterface_Maintenance.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes//PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Tools/NET_AsnException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MaintenanceDiagnosisConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign constructor
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
PHY_MaintenanceDiagnosisConsign::PHY_MaintenanceDiagnosisConsign( MIL_Agent_ABC& maintenanceAgent, const boost::shared_ptr< PHY_MaintenanceComposanteState >& state )
    : PHY_MaintenanceConsign_ABC( maintenanceAgent, state )
    , component_              ( 0 )
    , searchForUpperLevelDone_( false )
{
    EnterStateWaitingForDiagnosisTeam();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign constructor
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
PHY_MaintenanceDiagnosisConsign::PHY_MaintenanceDiagnosisConsign()
    : component_              ( 0 )
    , searchForUpperLevelDone_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign destructor
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
PHY_MaintenanceDiagnosisConsign::~PHY_MaintenanceDiagnosisConsign()
{
    ResetComponent();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::serialize
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceDiagnosisConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this );
    file & component_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::Cancel
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::Cancel()
{
    ResetComponent();
    PHY_MaintenanceConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::Update
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
bool PHY_MaintenanceDiagnosisConsign::Update()
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
        case sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection:
            if( !IsManualMode() )
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
// Name: PHY_MaintenanceDiagnosisConsign::SelectNewState
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::SelectNewState()
{
    if( GetState() == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
        next_ = [&]() { EnterStateDiagnosing(); };
    else
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::consign_already_resolved,
            "transport consign not in a waiting state" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::SearchForUpperLevelNotFound
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
bool PHY_MaintenanceDiagnosisConsign::SearchForUpperLevelNotFound() const
{
    return GetState() == sword::LogMaintenanceHandlingUpdate::searching_upper_levels && searchForUpperLevelDone_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::TransferToLogisticSuperior
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::TransferToLogisticSuperior()
{
    const auto state = GetState();
    if( state != sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
        throw MASA_EXCEPTION( "diagnosis consign is not in a waiting state" );
    next_ = [&]() { SetState( sword::LogMaintenanceHandlingUpdate::searching_upper_levels, 0 ); };
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::SelectMaintenanceTransporter
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::SelectMaintenanceTransporter( const PHY_ComposanteTypePion& /*type*/, const MIL_Agent_ABC* /*destination*/ )
{
    throw MASA_EXCEPTION( "cannot select a transporter for a diagnosis consign" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::SelectDiagnosisTeam
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::SelectDiagnosisTeam( const PHY_ComposanteTypePion& type )
{
    if( GetState() != sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::consign_already_resolved,
            "diagnosis consign not in a waiting for diagnosis team selection state" );
    if( component_ )
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::consign_already_resolved,
            "diagnosis consign already has a diagnosis team selected" );
    component_ = GetPionMaintenance().GetAvailableDiagnoser( &type );
    if( component_ )
    {
        GetPionMaintenance().StartUsingForLogistic( *component_ );
        next_ = [&]() { EnterStateDiagnosing(); };
    }
    else if( !FindAlternativeDiagnosisTeam( &type ) )
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::diagnosis_team_unavailable,
            "no component of specified type available for diagnosis team selection" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::SelectRepairTeam
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::SelectRepairTeam( const PHY_ComposanteTypePion& /*type*/ )
{
    throw MASA_EXCEPTION( "cannot select a repair team for a diagnosis consign" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::EnterStateWaitingForDiagnosisTeam
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::EnterStateWaitingForDiagnosisTeam()
{
    const auto state = GetComposanteState();
    ResetComponent();
    if( !state->NeedDiagnosis() )
        ChooseStateAfterDiagnostic();
    else if( IsManualMode() )
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection, 0 );
    else
    {
        component_ = GetPionMaintenance().GetAvailableDiagnoser();
        if( component_ )
        {
            GetPionMaintenance().StartUsingForLogistic( *component_ );
            next_ = [&]() { EnterStateDiagnosing(); };
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::EnterStateDiagnosing
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::EnterStateDiagnosing()
{
    SetState( sword::LogMaintenanceHandlingUpdate::diagnosing, PHY_BreakdownType::GetDiagnosticTime() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::ChooseStateAfterDiagnostic
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::ChooseStateAfterDiagnostic()
{
    const auto state = GetComposanteState();
    ResetComponent();
    state->NotifyDiagnosed();
    state->SetComposantePosition( GetPosition() );
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MaintenanceHandleComposanteForRepair( state ) )
    {
        pComposanteState_.reset();
        EnterStateFinished();
    }
    else
        next_ = [&]() { SetState( sword::LogMaintenanceHandlingUpdate::searching_upper_levels, 0 ); };
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::DoSearchForUpperLevel
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
bool PHY_MaintenanceDiagnosisConsign::DoSearchForUpperLevel()
{
    assert( !component_ );
    searchForUpperLevelDone_ = true;
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().FindLogisticManager();
    if( !pLogisticManager )
        return false;
    MIL_AutomateLOG* pLogisticSuperior = pLogisticManager->GetLogisticHierarchy().GetPrimarySuperior();
    if( pLogisticSuperior && pLogisticSuperior->MaintenanceHandleComposanteForTransport( GetComposanteState() ) )
    {
        pComposanteState_.reset();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::ResetComponent
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
void PHY_MaintenanceDiagnosisConsign::ResetComponent()
{
    if( component_ )
        GetPionMaintenance().StopUsingForLogistic( *component_ );
    component_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceDiagnosisConsign::FindAlternativeDiagnosisTeam
// Created: SLI 2014-02-12
// -----------------------------------------------------------------------------
bool PHY_MaintenanceDiagnosisConsign::FindAlternativeDiagnosisTeam( const PHY_ComposanteTypePion* type /*= 0*/ )
{
    if( MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager() )
    {
        const auto state = GetComposanteState();
        PHY_RoleInterface_Maintenance* newPion = pLogisticManager->MaintenanceFindAlternativeDiagnosisHandler( type );
        if( newPion && newPion != &GetPionMaintenance() && newPion->HandleComposanteForDiagnosis( state ) )
        {
            if( type )
            {
                state->GetConsign()->SetState( GetState(), 0 );
                state->SelectDiagnosisTeam( *type );
            }
            EnterStateFinished();
            pComposanteState_.reset();
            return true;
        }
    }
    return false;
}
