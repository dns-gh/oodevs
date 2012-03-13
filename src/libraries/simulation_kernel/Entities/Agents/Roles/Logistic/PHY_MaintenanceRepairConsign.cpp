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
    ResetTimer( 0 );
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
    ResetTimer( 0 );
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

    ResetTimer( 0 );
    pRepairer_ = GetPionMaintenance().GetAvailableRepairer( pComposanteState_->GetComposanteBreakdown() );
    if( !pRepairer_ )
    {
        // Find alternative repair unit
        MIL_AutomateLOG* pLogisticManager = GetPionMaintenance().GetPion().FindLogisticManager();
        if( pLogisticManager )
        {
            PHY_RoleInterface_Maintenance* newPion = pLogisticManager->MaintenanceFindAlternativeRepairHandler( *pComposanteState_ );
            if( newPion != &GetPionMaintenance() && newPion->HandleComposanteForRepair( *pComposanteState_ ) )
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
    assert( pComposanteState_ );
    assert( !pRepairer_ );
    ResetTimer( 0 );
    SetState( eWaitingForCarrier );
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
        SetState( eFinished );
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
    SetState( eWaitingForParts );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer()
{
    assert( pComposanteState_ );
    SetState( eWaitingForRepairer );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateRepairing
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateRepairing()
{
    assert( pComposanteState_ );
    SetState( eRepairing );
    ResetTimer( pComposanteState_->GetComposanteBreakdown().GetRepairTime() );
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
    SetState( eGoingBackToWar );
    ResetTimer( pComposanteState_->ApproximateTravelTime( pMaintenance_->GetRole< PHY_RoleInterface_Location>().GetPosition(), pComposanteState_->GetPionPosition() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::Update()
{
    if( DecrementTimer() )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eWaitingForCarrier       : if( DoSearchForCarrier  () ) EnterStateFinished          (); break;
        case eWaitingForParts         : if( DoWaitingForParts   () ) EnterStateWaitingForRepairer(); break;
        case eWaitingForRepairer      : if( DoWaitingForRepairer() ) EnterStateRepairing         (); break;
        case eRepairing               :                              EnterStateGoingBackToWar    (); break;
        case eGoingBackToWar          :      DoReturnComposante();   EnterStateFinished          (); break;
        case eFinished                :                                                              break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}
