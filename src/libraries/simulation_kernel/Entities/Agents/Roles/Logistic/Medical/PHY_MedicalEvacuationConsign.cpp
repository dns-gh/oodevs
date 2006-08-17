//*****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalEvacuationConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:36 $
// $Revision: 3 $
// $Workfile: PHY_MedicalEvacuationConsign.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MedicalEvacuationConsign.h"
#include "PHY_RolePion_Medical.h"
#include "PHY_MedicalHumanState.h"
#include "PHY_MedicalEvacuationAmbulance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_GUID( PHY_MedicalEvacuationConsign, "PHY_MedicalEvacuationConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationConsign::PHY_MedicalEvacuationConsign( PHY_RolePionLOG_Medical& medical, PHY_MedicalHumanState& humanState )
    : PHY_MedicalConsign_ABC( medical, humanState )
    , pDoctor_              ( 0 )
    , pEvacuationAmbulance_ ( 0 )
{
    if( humanState.EvacuatedByThirdParty() )
    {
        humanState.NotifyHandledByMedical();
        ChooseStateAfterEvacuation();
    }
    else
        EnterStateWaitingForEvacuation();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationConsign::PHY_MedicalEvacuationConsign()
    : PHY_MedicalConsign_ABC()
    , pDoctor_              ( 0 )
    , pEvacuationAmbulance_ ( 0 ) 
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationConsign::~PHY_MedicalEvacuationConsign()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalEvacuationConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this )
         & pDoctor_
         & pEvacuationAmbulance_;
}

// =============================================================================
// STATES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateWaitingForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateWaitingForEvacuation()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    assert( !pEvacuationAmbulance_ );
    
    SetState( eWaitingForEvacuation );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::CreateEvacuationAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::CreateEvacuationAmbulance()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    
    if( !pEvacuationAmbulance_ )
        pEvacuationAmbulance_ = GetPionMedical().GetAvailableEvacuationAmbulance( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingTo
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingTo()
{
    // Called by PHY_MedicalEvacuationAmbulance
    assert( pHumanState_ );
    //    assert( pEvacuationAmbulance_ ); // Peut asserter quand EnterStateEvacuationGoingTo() quand consign associée à une ambulance déjà affectée à un automate
    assert( !pDoctor_ );
    assert( GetState() == eWaitingForEvacuation );
    SetState( eEvacuationGoingTo );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationLoading()
{
    // Called by PHY_MedicalEvacuationAmbulance
    assert( pHumanState_ );
    //    assert( pEvacuationAmbulance_ ); // Peut asserter quand EnterStateEvacuationGoingTo() quand consign associée à une ambulance déjà affectée à un automate
    assert( !pDoctor_ );
    assert( GetState() == eEvacuationGoingTo || GetState() == eWaitingForEvacuation );
    SetState( eEvacuationLoading );
    nTimer_ = 0;
    pHumanState_->NotifyHandledByMedical();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationWaitingForFullLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::EnterStateEvacuationWaitingForFullLoading()
{
    // Called by PHY_MedicalEvacuationAmbulance
    assert( pHumanState_ );
    assert( pEvacuationAmbulance_ );
    assert( !pDoctor_ );

    nTimer_ = 0;
    if( GetState() == eEvacuationLoading )
    {
        SetState( eEvacuationWaitingForFullLoading );
        return true;
    }
    assert( GetState() == eEvacuationWaitingForFullLoading );
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingFrom
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingFrom()
{
    // Called by PHY_MedicalEvacuationAmbulance
    assert( pHumanState_ );
    assert( pEvacuationAmbulance_ );
    assert( !pDoctor_ );
    assert( GetState() == eEvacuationWaitingForFullLoading );
    SetState( eEvacuationGoingFrom );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationUnloading()
{
    // Called by PHY_MedicalEvacuationAmbulance
    assert( pHumanState_ );
    assert( pEvacuationAmbulance_ );
    assert( !pDoctor_ );
    assert( GetState() == eEvacuationGoingFrom );
    SetState( eEvacuationUnloading );
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::ChooseStateAfterEvacuation
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::ChooseStateAfterEvacuation()
{
    // Called by PHY_MedicalEvacuationAmbulance
    assert( pHumanState_ );
    assert( !pDoctor_ );

    pHumanState_->SetHumanPosition( GetPionMedical().GetPosition() );
    pEvacuationAmbulance_ = 0;

    nTimer_ = 0;
    if( pHumanState_->NeedDiagnosis() )
        SetState( eWaitingForDiagnostic );
    else        
        SetState( eWaitingForCollection );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::DoWaitingForDiagnostic
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::DoWaitingForDiagnostic()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    assert( !pEvacuationAmbulance_ );

    pDoctor_ = GetPionMedical().GetAvailableDoctorForDiagnosing();
    if( !pDoctor_ )
        return false;
    
    GetPionMedical().StartUsingForLogistic( *pDoctor_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateDiagnosing
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateDiagnosing()
{
    assert( pHumanState_ );
    assert( pDoctor_ );
    assert( !pEvacuationAmbulance_ );

    SetState( eDiagnosing );
    nTimer_ = PHY_HumanWound::GetDiagnosticTime();    
}


// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateWaitingForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pEvacuationAmbulance_ );
    assert( pDoctor_ );

    GetPionMedical().StopUsingForLogistic( *pDoctor_ );
    pDoctor_ = 0;
    pHumanState_->NotifyDiagnosed();
    nTimer_ = 0;
    SetState( eWaitingForCollection );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::DoWaitingForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::DoWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    assert( !pEvacuationAmbulance_ );

    if( GetPionMedical().GetAutomate().MedicalHandleHumanForCollection( *pHumanState_ ) )
    {
        pHumanState_ = 0;
        SetState( eFinished );
        return true;
    }
    return false;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() ) 
    {
        case eWaitingForEvacuation             : CreateEvacuationAmbulance(); break;  // Géré par PHY_MedicalAmbulance
        case eEvacuationGoingTo                : break;                               // Géré par PHY_MedicalAmbulance
        case eEvacuationLoading                : break;                               // Géré par PHY_MedicalAmbulance
        case eEvacuationWaitingForFullLoading  : break;                               // Géré par PHY_MedicalAmbulance
        case eEvacuationGoingFrom              : break;                               // Géré par PHY_MedicalAmbulance
        case eEvacuationUnloading              : break;                               // Géré par PHY_MedicalAmbulance
        case eWaitingForDiagnostic             : if( DoWaitingForDiagnostic() ) EnterStateDiagnosing          (); break;
        case eDiagnosing                       :                                EnterStateWaitingForCollection(); break;
        case eWaitingForCollection             : if( DoWaitingForCollection() ) EnterStateFinished            (); break;
        case eFinished                         : break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::Cancel()
{
    if( pDoctor_ )
    {
        GetPionMedical().StopUsingForLogistic( *pDoctor_ );
        pDoctor_ = 0;
    }
    if( pEvacuationAmbulance_ )
    {
        pEvacuationAmbulance_->UnregisterHuman( *this );
        pEvacuationAmbulance_ = 0;
    }
    PHY_MedicalConsign_ABC::Cancel();
}
