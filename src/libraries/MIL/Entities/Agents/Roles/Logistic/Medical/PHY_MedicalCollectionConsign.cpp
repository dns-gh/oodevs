// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalCollectionConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:04 $
// $Revision: 2 $
// $Workfile: PHY_MedicalCollectionConsign.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_MedicalCollectionConsign.h"
#include "PHY_RolePion_Medical.h"
#include "PHY_MedicalHumanState.h"
#include "PHY_MedicalCollectionAmbulance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"

BOOST_CLASS_EXPORT_GUID( PHY_MedicalCollectionConsign, "PHY_MedicalCollectionConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalCollectionConsign::PHY_MedicalCollectionConsign( PHY_RolePionLOG_Medical& medical, PHY_MedicalHumanState& humanState )
    : PHY_MedicalConsign_ABC( medical, humanState )
    , pCollectionAmbulance_ ( 0 )
{
    EnterStateWaitingForCollection();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionConsign::PHY_MedicalCollectionConsign()
    : PHY_MedicalConsign_ABC()
    , pCollectionAmbulance_ ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalCollectionConsign::~PHY_MedicalCollectionConsign()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalCollectionConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this )
         & pCollectionAmbulance_;
}

// =============================================================================
// STATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateWaitingForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pCollectionAmbulance_ );
    pHumanState_->SetHumanPosition( GetPionMedical().GetPosition() );
    nTimer_ = 0;
    SetState( eWaitingForCollection );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::CreateCollectionAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::CreateCollectionAmbulance()
{
    assert( pHumanState_ );
    if( !pCollectionAmbulance_ )
        pCollectionAmbulance_ = GetPionMedical().GetAvailableCollectionAmbulance( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionLoading
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateCollectionLoading()
{
    // Called by PHY_MedicalCollectionAmbulance
    assert( pHumanState_ );
//    assert( pCollectionAmbulance_ );
    assert( GetState() == eWaitingForCollection );
    SetState( eCollectionLoading );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionWaitingForFullLoading
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionConsign::EnterStateCollectionWaitingForFullLoading()
{
    // Called by PHY_MedicalCollectionAmbulance
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );

    nTimer_ = 0;
    if( GetState() == eCollectionLoading )
    {
        SetState( eCollectionWaitingForFullLoading );
        return true;
    }
    assert( GetState() == eCollectionWaitingForFullLoading );
    return false;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateSearchingForSortingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateSearchingForSortingArea()
{
    // Called by PHY_MedicalCollectionAmbulance
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == eCollectionWaitingForFullLoading );
    SetState( eSearchingForSortingArea );
    nTimer_ = 0;
}
 
// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionGoingTo
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateCollectionGoingTo()
{
    // Called by PHY_MedicalCollectionAmbulance
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == eSearchingForSortingArea );
    SetState( eCollectionGoingTo );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::EnterStateCollectionUnloading
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::EnterStateCollectionUnloading()
{
    // Called by PHY_MedicalCollectionAmbulance
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == eCollectionGoingTo );
    SetState( eCollectionUnloading );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::TransferToSortingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::TransferToSortingArea( PHY_RolePion_Medical& sortingArea )
{
    assert( pHumanState_ );
    assert( pCollectionAmbulance_ );
    assert( GetState() == eCollectionUnloading );
    SetState( eFinished );
    nTimer_               = 0;
    sortingArea.HandleHumanForSorting( *pCollectionAmbulance_, *pHumanState_ );
    pCollectionAmbulance_ = 0;    
    pHumanState_          = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::NotifyOutOfMedicalSystem
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionConsign::NotifyOutOfMedicalSystem()
{
    assert( pHumanState_ );
    SetState( eFinished );
    pCollectionAmbulance_ = 0;
    nTimer_               = 0;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() ) 
    {
        case eWaitingForCollection             : CreateCollectionAmbulance(); break; // Géré par PHY_MedicalAmbulance
        case eCollectionGoingTo                : break;                              // Géré par PHY_MedicalAmbulance
        case eSearchingForSortingArea          : break;                              // Géré par PHY_MedicalAmbulance
        case eCollectionLoading                : break;                              // Géré par PHY_MedicalAmbulance
        case eCollectionWaitingForFullLoading  : break;                              // Géré par PHY_MedicalAmbulance
        case eCollectionUnloading              : break;                              // Géré par PHY_MedicalAmbulance
        case eFinished                         : break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
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
