// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceTransportConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:58 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceTransportConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaintenanceTransportConsign.h"
#include "PHY_RolePion_Maintenance.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_MaintenanceTransportConsign, "PHY_MaintenanceTransportConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::PHY_MaintenanceTransportConsign( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState )
    : PHY_MaintenanceConsign_ABC( maintenance, composanteState )
    , pCarrier_                 ( 0 )
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
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::~PHY_MaintenanceTransportConsign()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceTransportConsign::serialize( Archive& file, const uint )
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
    
    nTimer_ = 0;
    pCarrier_ = GetPionMaintenance().GetAvailableHauler( GetComposanteType() );
    if( pCarrier_ )
        GetPionMaintenance().StartUsingForLogistic( *pCarrier_ );
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
    
    nTimer_ = 0;
    MIL_AutomateLOG* pMaintenanceSuperior = GetPionMaintenance().GetAutomate().GetMaintenanceSuperior();
    if( !pMaintenanceSuperior )
        return true; // Si pas de sup�rieur => la 'composante state' ne sera g�r�e par aucun pion, mais sera toujours active

    if( pMaintenanceSuperior->MaintenanceHandleComposanteForTransport( *pComposanteState_ ) )
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

    SetState( eWaitingForCarrier );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateGoingFrom
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateGoingFrom()
{
    assert( pComposanteState_ );
    assert( !pCarrier_ );
    
    SetState( eGoingFrom );
    nTimer_ = pComposanteState_->ApproximateTravelTime( pComposanteState_->GetComposantePosition(), GetPionMaintenance().GetPosition() );
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
    
    SetState( eCarrierGoingTo );
    nTimer_ = pCarrier_->ApproximateTravelTime( GetPionMaintenance().GetPosition(), pComposanteState_->GetComposantePosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierLoading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierLoading()
{
    assert( pComposanteState_ );
    assert( pCarrier_ );
    
    SetState( eCarrierLoading );
    nTimer_ = ( int ) pCarrier_->GetType().GetHaulerLoadingTime();
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
    
    SetState( eCarrierGoingFrom );
    nTimer_ = pCarrier_->ApproximateTravelTime( pComposanteState_->GetComposantePosition(), GetPionMaintenance().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::EnterStateCarrierUnloading
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceTransportConsign::EnterStateCarrierUnloading()
{
    assert( pComposanteState_ );
    assert( pCarrier_ );
    
    SetState( eCarrierUnloading );
    nTimer_ = ( int ) pCarrier_->GetType().GetHaulerUnloadingTime();
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
        SetState( eDiagnosing );
        nTimer_ = PHY_BreakdownType::GetDiagnosticTime();    
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
    pComposanteState_->SetComposantePosition( GetPionMaintenance().GetPosition() );
    nTimer_ = 0;

    if( GetPionMaintenance().GetAutomate().MaintenanceHandleComposanteForRepair( *pComposanteState_ ) )
    {
        pComposanteState_ = 0;
        SetState( eFinished ); // Managed by a 'repair consign'
    }
    else
        SetState( eSearchingForUpperLevel );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eGoingFrom             :                               EnterStateDiagnosing        (); break;
        case eWaitingForCarrier     : if( DoWaitingForCarrier() )   EnterStateCarrierGoingTo    (); break;
        case eCarrierGoingTo        :                               EnterStateCarrierLoading    (); break;
        case eCarrierLoading        :                               EnterStateCarrierGoingFrom  (); break;
        case eCarrierGoingFrom      :                               EnterStateCarrierUnloading  (); break;
        case eCarrierUnloading      :                               EnterStateDiagnosing        (); break;
        case eDiagnosing            :                               ChooseStateAfterDiagnostic  (); break;
        case eSearchingForUpperLevel: if( DoSearchForUpperLevel() ) EnterStateFinished          (); break; 
        case eFinished              :                                                               break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}
