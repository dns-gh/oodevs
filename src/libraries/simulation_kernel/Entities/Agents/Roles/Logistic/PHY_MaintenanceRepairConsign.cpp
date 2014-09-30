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
#include "PHY_MaintenanceRepairConsign.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "PHY_MaintenanceComposanteState.h"
#include "PHY_RoleInterface_Maintenance.h"
#include "Tools/NET_AsnException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MaintenanceRepairConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::PHY_MaintenanceRepairConsign( MIL_Agent_ABC& maintenanceAgent, const boost::shared_ptr< PHY_MaintenanceComposanteState >& state )
    : PHY_MaintenanceConsign_ABC( maintenanceAgent, state )
    , pRepairer_              ( 0 )
    , searchForUpperLevelDone_( false )
{
    EnterStateWaitingForRepairerSelection();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::PHY_MaintenanceRepairConsign()
    : PHY_MaintenanceConsign_ABC()
    , pRepairer_              ( 0 )
    , searchForUpperLevelDone_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::~PHY_MaintenanceRepairConsign()
{
    ResetRepairer();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceRepairConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this );
    file & pRepairer_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::Cancel()
{
    ResetRepairer();
    PHY_MaintenanceConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::ResetRepairer
// Created: ABR 2014-02-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::ResetRepairer()
{
    if( pRepairer_ )
    {
        GetPionMaintenance().StopUsingForLogistic( *pRepairer_ );
        pRepairer_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoReturnComposante
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::DoReturnComposante()
{
    GetComposanteState()->NotifyRepaired();
    pComposanteState_.reset();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoWaitingForParts
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::DoWaitingForParts()
{
    return GetPionMaintenance().ConsumePartsForBreakdown( GetComposanteBreakdown() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::DoWaitingForRepairer()
{
    assert( !pRepairer_ );
    const auto& breakdown = GetComposanteBreakdown();
    pRepairer_ = GetPionMaintenance().GetAvailableRepairer( breakdown );
    if( !pRepairer_ )
    {
        if( !FindAlternativeRepairTeam()
            && !GetPionMaintenance().HasUsableRepairer( breakdown ) )
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
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MaintenanceHandleComposanteForTransport( GetComposanteState() ) )
    {
        pComposanteState_.reset();
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
    SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_parts, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairerSelection
// Created: ABR 2014-02-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairerSelection()
{
    if( IsManualMode() )
        SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection, 0 );
    else
        EnterStateWaitingForParts();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer()
{
    SetState( sword::LogMaintenanceHandlingUpdate::waiting_for_repairer, 0 );
}

namespace
{
    int CeiledDivision( double a, double b )
    {
        if( b == 0. )
            return static_cast< int >( a );
        return static_cast< int >( std::ceil( a / b ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateRepairing
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateRepairing()
{
    assert( pRepairer_ );
    SetState( sword::LogMaintenanceHandlingUpdate::repairing,
              PHY_BreakdownType::GetRepairDurationInManHours()
                ? CeiledDivision( GetComposanteBreakdown().GetRepairTime(), pRepairer_->GetNbrLivingHumans() )
                : GetComposanteBreakdown().GetRepairTime() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateGoingBackToWar
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateGoingBackToWar()
{
    const auto state = GetComposanteState();
    assert( pRepairer_ );
    GetPionMaintenance().StopUsingForLogistic( *pRepairer_ );
    pRepairer_ = 0;
    SetState( sword::LogMaintenanceHandlingUpdate::moving_back,
        state->ApproximateTravelTime( GetPosition(), state->GetPionPosition() ) );
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
            {
                if( pRepairer_ )
                    EnterStateRepairing();
                else
                    EnterStateWaitingForRepairer();
            }
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_repairer:
            if( DoWaitingForRepairer() )
                EnterStateRepairing();
            break;
        case sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection:
            if( !IsManualMode() )
                EnterStateWaitingForParts();
            break;
        case sword::LogMaintenanceHandlingUpdate::repairing:
            EnterStateGoingBackToWar();
            break;
        case sword::LogMaintenanceHandlingUpdate::moving_back:
            DoReturnComposante();
            EnterStateFinished();
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
// Name: PHY_MaintenanceRepairConsign::SearchForUpperLevelNotFound
// Created: LGY 2014-02-04
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::SearchForUpperLevelNotFound() const
{
    return GetState() == sword::LogMaintenanceHandlingUpdate::searching_upper_levels && searchForUpperLevelDone_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::DoSearchForUpperLevel
// Created: LGY 2014-02-04
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::DoSearchForUpperLevel()
{
    searchForUpperLevelDone_ = true;
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager();
    if( !pLogisticManager )
        return false;
    MIL_AutomateLOG* pLogisticSuperior = pLogisticManager->GetLogisticHierarchy().GetPrimarySuperior();
    if( pLogisticSuperior && pLogisticSuperior->MaintenanceHandleComposanteForRepair( GetComposanteState() ) )
    {
        pComposanteState_.reset();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::SelectNewState
// Created: MCO 2014-01-30
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::SelectNewState()
{
    if( GetState() == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
        next_ = [&]() { EnterStateWaitingForParts(); };
    else
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::consign_already_resolved,
            "repair consign not in a waiting state" );
}

void PHY_MaintenanceRepairConsign::TransferToLogisticSuperior()
{
    if( GetState() != sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
        throw MASA_EXCEPTION( "repair consign not in a waiting state" );
    next_ = [&]() { SetState( sword::LogMaintenanceHandlingUpdate::searching_upper_levels, 0 ); };
}

void PHY_MaintenanceRepairConsign::SelectMaintenanceTransporter( const PHY_ComposanteTypePion& /*type*/, const MIL_Agent_ABC* /*destination*/ )
{
    throw MASA_EXCEPTION( "cannot select a transporter for a repair consign" );
}

void PHY_MaintenanceRepairConsign::SelectDiagnosisTeam( const PHY_ComposanteTypePion& /*type*/ )
{
    throw MASA_EXCEPTION( "cannot select a diagnosis team for a repair consign" );
}

void PHY_MaintenanceRepairConsign::SelectRepairTeam( const PHY_ComposanteTypePion& type )
{
    if( GetState() != sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::consign_already_resolved,
            "repair consign not in a waiting for repair team selection state" );
    if( pRepairer_ )
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::consign_already_resolved,
            "repair consign already has a repair team selected" );
    pRepairer_ = GetPionMaintenance().GetAvailableRepairer( GetComposanteBreakdown(), &type );
    if( pRepairer_ )
    {
        GetPionMaintenance().StartUsingForLogistic( *pRepairer_ );
        EnterStateWaitingForParts();
    }
    else if( !FindAlternativeRepairTeam( &type ) )
        throw MASA_BADPARAM_ASN( sword::ManualMaintenanceError, sword::repair_team_unavailable,
            "no component of specified type available for repair team selection" );
}

bool PHY_MaintenanceRepairConsign::FindAlternativeRepairTeam( const PHY_ComposanteTypePion* type )
{
    MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager();
    if( !pLogisticManager )
        return false;
    if( auto state = pComposanteState_.lock() )
    {
        PHY_RoleInterface_Maintenance* newPion = pLogisticManager->MaintenanceFindAlternativeRepairHandler( state, type );
        if( newPion && newPion != &GetPionMaintenance() && newPion->HandleComposanteForRepair( state ) )
        {
            if( type )
            {
                state->GetConsign()->SetState( GetState(), 0 );
                state->SelectRepairTeam( *type );
            }
            EnterStateFinished();
            pComposanteState_.reset();
            return true;
        }
    }
    return false;
}
