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
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this );
    file & pDoctor_;
    file & pEvacuationAmbulance_;
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
    if( GetState() == sword::LogMedicalHandlingUpdate::finished || !pHumanState_ )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
        return;
    }
    SetState( sword::LogMedicalHandlingUpdate::waiting_for_evacuation, 0 );
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
    assert( GetState() == sword::LogMedicalHandlingUpdate::waiting_for_evacuation );
    if( GetState() != sword::LogMedicalHandlingUpdate::waiting_for_evacuation )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
        return;
    }
    SetState( sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_in, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationConsign::EnterStateEvacuationLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationConsign::EnterStateEvacuationLoading()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    assert( GetState() == sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_in || GetState() == sword::LogMedicalHandlingUpdate::waiting_for_evacuation );
    SetState( sword::LogMedicalHandlingUpdate::evacuation_ambulance_loading, 0 );
    if( !pHumanState_ )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
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
    if( GetState() == sword::LogMedicalHandlingUpdate::evacuation_ambulance_loading )
    {
        SetState( sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion, 0 );
        return true;
    }
    assert( GetState() == sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion );
    if( GetState() != sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
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
    assert( GetState() == sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion );
    if( GetState() != sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
        return;
    }
    SetState( sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_out, 0 );
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
    assert( GetState() == sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_out );
    if( GetState() != sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_out )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
        return;
    }
    SetState( sword::LogMedicalHandlingUpdate::evacuation_ambulance_unloading, 0 );
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
    if( pHumanState_->NeedDiagnosis() )
        SetState( sword::LogMedicalHandlingUpdate::waiting_for_diagnostic, 0 );
    else
        SetState( sword::LogMedicalHandlingUpdate::waiting_for_collection, 0 );
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
    if( GetState() == sword::LogMedicalHandlingUpdate::finished )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
        return;
    }
    SetState( sword::LogMedicalHandlingUpdate::diagnosing, PHY_HumanWound::GetDiagnosticTime() );
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
    if( GetState() == sword::LogMedicalHandlingUpdate::finished || !pHumanState_ )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__  ": Bad human state." );
        EnterStateFinished();
        return;
    }
    GetPionMedical().StopUsingForLogistic( *pDoctor_ );
    pDoctor_ = 0;
    pHumanState_->NotifyDiagnosed();
    SetState( sword::LogMedicalHandlingUpdate::waiting_for_collection, 0 );
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
        EnterStateFinished();
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
        return GetState() == sword::LogMedicalHandlingUpdate::finished;
    switch( GetState() )
    {
        case sword::LogMedicalHandlingUpdate::waiting_for_evacuation:
            CreateEvacuationAmbulance();
            break;
        case sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_in:
        case sword::LogMedicalHandlingUpdate::evacuation_ambulance_loading:
        case sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion:
        case sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_out:
        case sword::LogMedicalHandlingUpdate::evacuation_ambulance_unloading:
            break; // Géré par PHY_MedicalAmbulance
        case sword::LogMedicalHandlingUpdate::waiting_for_diagnostic:
            if( DoWaitingForDiagnostic() )
                EnterStateDiagnosing();
            break;
        case sword::LogMedicalHandlingUpdate::diagnosing:
            EnterStateWaitingForCollection();
            break;
        case sword::LogMedicalHandlingUpdate::waiting_for_collection:
            if( DoWaitingForCollection() )
                EnterStateFinished();
            break;
        case sword::LogMedicalHandlingUpdate::finished:
            break;
        default:
            assert( false );
    }
    if( pEvacuationAmbulance_ )
        SendExternalTimerValue( pEvacuationAmbulance_->GetTimer() );
    return GetState() == sword::LogMedicalHandlingUpdate::finished;
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
