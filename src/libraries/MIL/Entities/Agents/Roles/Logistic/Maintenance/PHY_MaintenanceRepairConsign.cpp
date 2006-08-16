// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceRepairConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:57 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceRepairConsign.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MaintenanceRepairConsign.h"
#include "PHY_RolePion_Maintenance.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_MaintenanceRepairConsign, "PHY_MaintenanceRepairConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::PHY_MaintenanceRepairConsign( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState )
    : PHY_MaintenanceConsign_ABC( maintenance, composanteState )
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
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::~PHY_MaintenanceRepairConsign()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceRepairConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this )
         & pRepairer_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
    nTimer_ = 0;
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
    
    nTimer_ = 0;
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
    
    nTimer_    = 0;
    pRepairer_ = GetPionMaintenance().GetAvailableRepairer( pComposanteState_->GetComposanteBreakdown() );
    if( !pRepairer_ )
    {
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
    nTimer_ = 0;
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
    
    if( GetPionMaintenance().GetAutomate().MaintenanceHandleComposanteForTransport( *pComposanteState_ ) )
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
    nTimer_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateWaitingForRepairer()
{
    assert( pComposanteState_ );
    SetState( eWaitingForRepairer );
    nTimer_ = 0;    
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::EnterStateRepairing
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceRepairConsign::EnterStateRepairing()
{
    assert( pComposanteState_ );
    
    SetState( eRepairing );
    nTimer_ = (int)( pComposanteState_->GetComposanteBreakdown().GetRepairTime() );
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
    nTimer_    = pComposanteState_->ApproximateTravelTime( GetPionMaintenance().GetPosition(), pComposanteState_->GetPionPosition() );
    SetState( eGoingBackToWar );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::Update()
{
    if ( --nTimer_ > 0 )
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
