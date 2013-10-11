//*****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalEvacuationConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:36 $
// $Revision: 3 $
// $Workfile: PHY_MedicalEvacuationConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalEvacuationConsign.h"
#include "PHY_RoleInterface_Medical.h"
#include "PHY_MedicalHumanState.h"
#include "PHY_MedicalEvacuationAmbulance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MedicalEvacuationConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationConsign::PHY_MedicalEvacuationConsign( MIL_Agent_ABC& medical, PHY_MedicalHumanState& humanState )
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
    : pDoctor_             ( 0 )
    , pEvacuationAmbulance_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationConsign::~PHY_MedicalEvacuationConsign()
{
    if( pDoctor_ )
        GetPionMedical().StopUsingForLogistic( *pDoctor_ );
    if( pEvacuationAmbulance_ )
        pEvacuationAmbulance_->UnregisterHuman( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalEvacuationConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this )
         & pDoctor_
         & pEvacuationAmbulance_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateWaitingForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateWaitingForEvacuation()
{
    assert( pHumanState_ );
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
    if( GetState() == eFinished || !pHumanState_ )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
        return;
    }
    SetState( eWaitingForEvacuation );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::CreateEvacuationAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::CreateEvacuationAmbulance()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    if( pEvacuationAmbulance_ )
        return;
    pEvacuationAmbulance_ = GetPionMedical().GetAvailableEvacuationAmbulance( *this );
    if( pEvacuationAmbulance_ )
        return;
    // Find alternative evacuation unit
    MIL_AutomateLOG* pLogisticManager = GetPionMedical().GetPion().FindLogisticManager();
    if( ! pLogisticManager )
        return;
    PHY_RoleInterface_Medical* newPion = pLogisticManager->MedicalFindAlternativeEvacuationHandler( *pHumanState_ );
    if( newPion && newPion != &GetPionMedical() && newPion->HandleHumanForEvacuation( *pHumanState_ ) )
    {
        EnterStateFinished();
        pHumanState_ = 0; // Crade
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingTo
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingTo()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    assert( GetState() == eWaitingForEvacuation );
    if( GetState() != eWaitingForEvacuation )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
        return;
    }
    SetState( eEvacuationGoingTo );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationLoading()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    assert( GetState() == eEvacuationGoingTo || GetState() == eWaitingForEvacuation );
    SetState( eEvacuationLoading );
    ResetTimer( 0 );
    if( !pHumanState_ )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
    }
    else
        pHumanState_->NotifyHandledByMedical();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationWaitingForFullLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::EnterStateEvacuationWaitingForFullLoading()
{
    assert( pHumanState_ );
    assert( pEvacuationAmbulance_ );
    assert( !pDoctor_ );
    ResetTimer( 0 );
    if( GetState() == eEvacuationLoading )
    {
        SetState( eEvacuationWaitingForFullLoading );
        return true;
    }
    assert( GetState() == eEvacuationWaitingForFullLoading );
    if( GetState() != eEvacuationWaitingForFullLoading )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingFrom
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationGoingFrom()
{
    assert( pHumanState_ );
    assert( pEvacuationAmbulance_ );
    assert( !pDoctor_ );
    assert( GetState() == eEvacuationWaitingForFullLoading );
    if( GetState() != eEvacuationWaitingForFullLoading )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
        return;
    }
    SetState( eEvacuationGoingFrom );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationUnloading()
{
    assert( pHumanState_ );
    assert( pEvacuationAmbulance_ );
    assert( !pDoctor_ );
    assert( GetState() == eEvacuationGoingFrom );
    if( GetState() != eEvacuationGoingFrom )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
        return;
    }
    SetState( eEvacuationUnloading );
    ResetTimer( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::ChooseStateAfterEvacuation
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::ChooseStateAfterEvacuation()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    pHumanState_->SetHumanPosition( GetPionMedical().GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );
    pEvacuationAmbulance_ = 0;
    ResetTimer( 0 );
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
    if( GetState() == eFinished )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
        return;
    }
    SetState( eDiagnosing );
    ResetTimer( PHY_HumanWound::GetDiagnosticTime() );
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
    if( GetState() == eFinished || !pHumanState_ )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        SetState( eFinished );
        return;
    }
    GetPionMedical().StopUsingForLogistic( *pDoctor_ );
    pDoctor_ = 0;
    pHumanState_->NotifyDiagnosed();
    ResetTimer( 0 );
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
// Name: PHY_MedicalEvacuationConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::Update()
{
    if( DecrementTimer() )
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
    if( pEvacuationAmbulance_ )
        SendExternalTimerValue( pEvacuationAmbulance_->GetTimer() );
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

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::CouldNeedCollectionAmbulance
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::CouldNeedCollectionAmbulance() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::IsATransportConsign
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationConsign::IsATransportConsign() const
{
    return true;
}
