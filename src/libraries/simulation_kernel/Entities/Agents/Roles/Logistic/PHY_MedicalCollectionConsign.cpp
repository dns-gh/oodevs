// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalCollectionConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:04 $
// $Revision: 2 $
// $Workfile: PHY_MedicalCollectionConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalCollectionConsign.h"
#include "PHY_RoleInterface_Medical.h"
#include "PHY_MedicalHumanState.h"
#include "PHY_MedicalCollectionAmbulance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MedicalCollectionConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalCollectionConsign::PHY_MedicalCollectionConsign( MIL_Agent_ABC& medical, PHY_MedicalHumanState& humanState )
    : PHY_MedicalConsign_ABC( medical, humanState )
    , pCollectionAmbulance_( 0 )
{
    EnterStateWaitingForCollection();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionConsign::PHY_MedicalCollectionConsign()
    : pCollectionAmbulance_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalCollectionConsign::~PHY_MedicalCollectionConsign()
{
    if( pCollectionAmbulance_ )
        pCollectionAmbulance_->UnregisterHuman( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalCollectionConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this )
         & pCollectionAmbulance_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateWaitingForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateWaitingForCollection()
{
    assert( pHumanState_ );
    if( pCollectionAmbulance_ )
    {
        pCollectionAmbulance_->UnregisterHuman( *this );
        pCollectionAmbulance_ = 0;
    }
    pHumanState_->SetHumanPosition( GetPionMedical().GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );
    ResetTimer( 0 );
    SetState( sword::LogMedicalHandlingUpdate::waiting_for_collection );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::CreateCollectionAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::CreateCollectionAmbulance()
{
    assert( pHumanState_ );
    if( pCollectionAmbulance_ )
        return;
    pCollectionAmbulance_ = GetPionMedical().GetAvailableCollectionAmbulance( *this );
    if( pCollectionAmbulance_ )
        return;
    // Find alternative evacuation unit
    MIL_AutomateLOG* pLogisticManager = GetPionMedical().GetPion().FindLogisticManager();
    if( ! pLogisticManager )
        return;
    PHY_RoleInterface_Medical* newPion = pLogisticManager->MedicalFindAlternativeCollectionHandler( *pHumanState_ );
    if( newPion && newPion != &GetPionMedical() && newPion->HandleHumanForCollection( *pHumanState_ ) )
    {
        EnterStateFinished();
        pHumanState_ = 0; // Crade
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionLoading
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateCollectionLoading()
{
    assert( pHumanState_ );
    assert( GetState() == sword::LogMedicalHandlingUpdate::waiting_for_collection );
    SetState( sword::LogMedicalHandlingUpdate::collection_ambulance_loading );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionWaitingForFullLoading
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionConsign::EnterStateCollectionWaitingForFullLoading()
{
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    ResetTimer( 0 );
    if( GetState() == sword::LogMedicalHandlingUpdate::collection_ambulance_loading )
    {
        SetState( sword::LogMedicalHandlingUpdate::waiting_for_collection_loading_completion );
        return true;
    }
    assert( GetState() == sword::LogMedicalHandlingUpdate::waiting_for_collection_loading_completion );
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateSearchingForDestinationArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateSearchingForDestinationArea()
{
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == sword::LogMedicalHandlingUpdate::waiting_for_collection_loading_completion );
    SetState( pHumanState_->NeedSorting()
              ? sword::LogMedicalHandlingUpdate::looking_for_triage
              : sword::LogMedicalHandlingUpdate::looking_for_medical_attention );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionGoingTo
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateCollectionGoingTo()
{
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == sword::LogMedicalHandlingUpdate::looking_for_triage ||
            GetState() == sword::LogMedicalHandlingUpdate::looking_for_medical_attention );
    SetState( sword::LogMedicalHandlingUpdate::collection_ambulance_moving_in );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionUnloading
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateCollectionUnloading()
{
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == sword::LogMedicalHandlingUpdate::collection_ambulance_moving_in );
    SetState( sword::LogMedicalHandlingUpdate::collection_ambulance_unloading );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::TransferToDestinationArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::TransferToDestinationArea( PHY_RoleInterface_Medical& destinationArea )
{
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == sword::LogMedicalHandlingUpdate::collection_ambulance_unloading );
    SetState( sword::LogMedicalHandlingUpdate::finished );
    ResetTimer( 0 );
    if( pHumanState_->NeedSorting() )
        destinationArea.HandleHumanForSorting( *pHumanState_ );
    else
        destinationArea.HandleHumanForHealing( *pHumanState_ );
    pCollectionAmbulance_ = 0;
    pHumanState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::NotifyOutOfMedicalSystem
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::NotifyOutOfMedicalSystem()
{
    assert( pHumanState_ );
    SetState( sword::LogMedicalHandlingUpdate::finished );
    pCollectionAmbulance_ = 0;
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionConsign::Update()
{
    if( DecrementTimer() )
        return GetState() == sword::LogMedicalHandlingUpdate::finished;

    switch( GetState() )
    {
        case sword::LogMedicalHandlingUpdate::waiting_for_collection:
            CreateCollectionAmbulance();
            break;
        case sword::LogMedicalHandlingUpdate::collection_ambulance_moving_in:
        case sword::LogMedicalHandlingUpdate::looking_for_triage:
        case sword::LogMedicalHandlingUpdate::looking_for_medical_attention:
        case sword::LogMedicalHandlingUpdate::collection_ambulance_loading:
        case sword::LogMedicalHandlingUpdate::waiting_for_collection_loading_completion:
        case sword::LogMedicalHandlingUpdate::collection_ambulance_unloading:
        case sword::LogMedicalHandlingUpdate::finished:
            // Géré par PHY_MedicalAmbulance
            break;
        default:
            assert( false );
    }
    if( pCollectionAmbulance_ )
        SendExternalTimerValue( pCollectionAmbulance_->GetTimer() );
    return GetState() == sword::LogMedicalHandlingUpdate::finished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::Cancel()
{
    if( pCollectionAmbulance_ )
    {
        pCollectionAmbulance_->UnregisterHuman( *this );
        pCollectionAmbulance_ = 0;
    }
    PHY_MedicalConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::CouldNeedCollectionAmbulance
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionConsign::CouldNeedCollectionAmbulance() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::IsATransportConsign
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionConsign::IsATransportConsign() const
{
    return true;
}
