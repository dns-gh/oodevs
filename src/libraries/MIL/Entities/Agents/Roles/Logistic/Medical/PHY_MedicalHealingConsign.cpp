//******************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalHealingConsign.cpp $
// $Author: Jvt $
// $Modtime: 28/04/05 17:45 $
// $Revision: 3 $
// $Workfile: PHY_MedicalHealingConsign.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MedicalHealingConsign.h"
#include "PHY_RolePion_Medical.h"
#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Units/Logistic/PHY_LogWorkTime.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_MedicalHealingConsign, "PHY_MedicalHealingConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHealingConsign::PHY_MedicalHealingConsign( PHY_RolePionLOG_Medical& medical, PHY_MedicalHumanState& humanState )
    : PHY_MedicalConsign_ABC( medical, humanState )
    , pDoctor_              ( 0 )
{
    EnterStateWaitingForHealing();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalHealingConsign::PHY_MedicalHealingConsign()
    : PHY_MedicalConsign_ABC()
    , pDoctor_              ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHealingConsign::~PHY_MedicalHealingConsign()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalHealingConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this )
         & pDoctor_;
}

// =============================================================================
// STATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::EnterStateWaitingForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::EnterStateWaitingForHealing()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    SetState( eWaitingForHealing );
    nTimer_ = 0; 
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
        return false;
    
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
    nTimer_ = (uint)( pDoctor_->GetHealingTime( pHumanState_->GetHuman() ) * GetPionMedical().GetWorkTime().GetHealingTimeFactor() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::ChooseStateAfterHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::ChooseStateAfterHealing()
{
    assert( pHumanState_ );
    assert( pDoctor_ );
    
    pHumanState_->Heal( *pDoctor_ );

    if ( pHumanState_->GetHuman().NeedMedical() )
        EnterStateSearchingForHealingArea();
    else
    {
        GetPionMedical().StopUsingForLogistic( *pDoctor_ );
        pDoctor_ = 0;
        nTimer_  = 0;
        
        if ( pHumanState_->ShouldGoBackToWar() )
            SetState( eGoingBackToWar );
        else
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
    assert( pDoctor_ );
    SetState( eSearchingForHealingArea );
    nTimer_ = 0;
    GetPionMedical().StopUsingForLogistic( *pDoctor_ );
    pDoctor_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHealingConsign::DoSearchForHealingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHealingConsign::DoSearchForHealingArea()
{
    if( GetPionMedical().GetAutomate().MedicalHandleHumanForHealing( *pHumanState_ ) )
    {
        SetState( eFinished );
        nTimer_ = 0;
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
    pHumanState_->SetHumanPosition( GetPionMedical().GetPosition() );
    nTimer_ = 0;
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

    if( GetPionMedical().GetAutomate().MedicalHandleHumanForCollection( *pHumanState_ ) )
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
    pHumanState_->GoBackToWar();
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
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() ) 
    {
        case eWaitingForHealing      : if( DoWaitingForHealing() )    EnterStateHealing       (); break;
        case eHealing                :                                ChooseStateAfterHealing (); break;
        case eSearchingForHealingArea: DoSearchForHealingArea();                                  break;
        case eWaitingForCollection   : if( DoWaitingForCollection() ) EnterStateFinished      (); break;
        case eGoingBackToWar         : DoReturnHuman();               EnterStateFinished      (); break;
        case eFinished               :                                                            break;
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
