//******************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalHealingConsign.cpp $
// $Author: Jvt $
// $Modtime: 28/04/05 17:45 $
// $Revision: 3 $
// $Workfile: PHY_MedicalHealingConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalHealingConsign.h"
#include "PHY_RoleInterface_Medical.h"
#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MedicalHealingConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHealingConsign::PHY_MedicalHealingConsign( PHY_RoleInterface_Medical& medical, PHY_MedicalHumanState& humanState )
    : PHY_MedicalConsign_ABC( medical, humanState )
    , pDoctor_( 0 )
{
    EnterStateWaitingForHealing();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalHealingConsign::PHY_MedicalHealingConsign()
    : PHY_MedicalConsign_ABC()
    , pDoctor_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHealingConsign::~PHY_MedicalHealingConsign()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalHealingConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this )
         & pDoctor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::EnterStateWaitingForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::EnterStateWaitingForHealing()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    SetState( eWaitingForHealing );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::DoWaitingForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_MedicalHealingConsign::DoWaitingForHealing()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );

    pDoctor_ = GetPionMedical().GetAvailableDoctorForHealing( pHumanState_->GetHuman() );
    if( !pDoctor_ )
    {
        // Find alternative healing unit
        MIL_AutomateLOG* pLogisticManager = GetPionMedical().GetPion().FindLogisticManager();
        if( pLogisticManager )
        {
            PHY_RoleInterface_Medical* newPion = pLogisticManager->MedicalFindAlternativeHealingHandler( *pHumanState_ );
            if( newPion != &GetPionMedical() && newPion->HandleHumanForHealing( *pHumanState_ ) )
            {
                EnterStateFinished();
                pHumanState_ = 0; // Crade
            }
        }
        return false;
    }

    GetPionMedical().StartUsingForLogistic( *pDoctor_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::EnterStateHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::EnterStateHealing()
{
    assert( pHumanState_ );
    assert( pDoctor_ );
    SetState( eHealing );
    ResetTimer( pDoctor_->GetHealingTime( pHumanState_->GetHuman() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::EnterStateResting
// Created: NLD 2006-07-17
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::EnterStateResting()
{
    assert( pHumanState_ );
    assert( pDoctor_ );

    // Healing time elapsed
    ResetTimer( pHumanState_->Heal( *pDoctor_ ) ); // Returns resting time
    GetPionMedical().StopUsingForLogistic( *pDoctor_ );
    pDoctor_ = 0;
    SetState( eResting );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::ChooseStateAfterResting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::ChooseStateAfterResting()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );

    // Resting time elapsed
    if( pHumanState_->GetHuman().NeedMedical() )
        EnterStateSearchingForHealingArea();
    else
    {
        if( pHumanState_->ShouldGoBackToWar() )
            DoReturnHuman();
        EnterStateFinished();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::EnterStateSearchingForHealingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::EnterStateSearchingForHealingArea()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    SetState( eSearchingForHealingArea );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::DoSearchForHealingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::DoSearchForHealingArea()
{
    MIL_AutomateLOG* pLogisticManager = GetPionMedical().GetPion().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MedicalHandleHumanForHealing( *pHumanState_ ) )
    {
        SetState( eFinished );
        ResetTimer( 0 );
        pHumanState_ = 0;
        return;
    }
    EnterStateWaitingForCollection();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::EnterStateWaitingForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::EnterStateWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    pHumanState_->SetHumanPosition( GetPionMedical().GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );
    ResetTimer( 0 );
    SetState( eWaitingForCollection );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::DoWaitingForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalHealingConsign::DoWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );

    MIL_AutomateLOG* pLogisticManager = GetPionMedical().GetPion().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MedicalHandleHumanForCollection( *pHumanState_ ) )
    {
        pHumanState_ = 0;
        SetState( eFinished );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::DoReturnHuman
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::DoReturnHuman()
{
    if( pHumanState_->GoBackToWar() )
        pHumanState_ = 0;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MedicalHealingConsign::Update()
{
    if( DecrementTimer() )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eWaitingForHealing      : if( DoWaitingForHealing() )    EnterStateHealing      (); break;
        case eHealing                :                                EnterStateResting      (); break;
        case eResting                :                                ChooseStateAfterResting(); break;
        case eSearchingForHealingArea: DoSearchForHealingArea();                                 break;
        case eWaitingForCollection   : if( DoWaitingForCollection() ) EnterStateFinished     (); break;
        case eFinished               :                                                           break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::Cancel()
{
    if( pDoctor_ )
    {
        GetPionMedical().StopUsingForLogistic( *pDoctor_ );
        pDoctor_ = 0;
    }
    PHY_MedicalConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::CouldNeedCollectionAmbulance
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_MedicalHealingConsign::CouldNeedCollectionAmbulance() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::IsATransportConsign
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
bool PHY_MedicalHealingConsign::IsATransportConsign() const
{
    return false;
}
