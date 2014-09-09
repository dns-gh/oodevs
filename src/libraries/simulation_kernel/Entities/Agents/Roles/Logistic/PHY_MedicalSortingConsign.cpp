//*****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalSortingConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:35 $
// $Revision: 3 $
// $Workfile: PHY_MedicalSortingConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalSortingConsign.h"
#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MedicalSortingConsign )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalSortingConsign::PHY_MedicalSortingConsign( MIL_Agent_ABC& medical, PHY_MedicalHumanState& humanState )
    : PHY_MedicalConsign_ABC( medical, humanState )
    , pDoctor_( 0 )
{
    if( humanState.NeedSorting() )
        EnterStateWaitingForSorting();
    else
        EnterStateSearchingForHealingArea();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalSortingConsign::PHY_MedicalSortingConsign()
    : PHY_MedicalConsign_ABC()
    , pDoctor_( 0 )
{
    if( pDoctor_ )
    {
        GetPionMedical().StopUsingForLogistic( *pDoctor_ );
        pDoctor_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalSortingConsign::~PHY_MedicalSortingConsign()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalSortingConsign::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this );
    file & pDoctor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::EnterStateWaitingForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::EnterStateWaitingForSorting()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    SetState( sword::LogMedicalHandlingUpdate::waiting_for_triage, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::DoWaitingForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_MedicalSortingConsign::DoWaitingForSorting()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );

    pDoctor_ = GetPionMedical().GetAvailableDoctorForSorting();
    if( !pDoctor_ )
    {
        // Find alternative sorting unit
        MIL_AutomateLOG* pLogisticManager = GetPionMedical().GetPion().FindLogisticManager();
        if( pLogisticManager )
        {
            PHY_RoleInterface_Medical* newPion = pLogisticManager->MedicalFindAlternativeSortingHandler( *pHumanState_ );
            if( newPion && newPion != &GetPionMedical() )
            {
                newPion->HandleHumanForSorting( *pHumanState_ );
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
// Name: PHY_MedicalSortingConsign::EnterStateSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::EnterStateSorting()
{
    assert( pHumanState_ );
    assert( pDoctor_ );
    SetState( sword::LogMedicalHandlingUpdate::triaging, PHY_HumanWound::GetSortingTime() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::EnterStateSearchingForHealingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::EnterStateSearchingForHealingArea()
{
    assert( pHumanState_ );

    SetState( sword::LogMedicalHandlingUpdate::looking_for_medical_attention, 0 );

    // Sorting
    if( pDoctor_ )
    {
        pHumanState_->NotifySorted();
        GetPionMedical().StopUsingForLogistic( *pDoctor_ );
        pDoctor_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::DoSearchForHealingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::DoSearchForHealingArea()
{
    MIL_AutomateLOG* pLogisticManager = GetPionMedical().FindLogisticManager();
    if( pLogisticManager && pLogisticManager->MedicalHandleHumanForHealing( *pHumanState_ ) )
    {
        EnterStateFinished();
        pHumanState_ = 0;
        return;
    }
    EnterStateWaitingForCollection();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::EnterStateWaitingForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::EnterStateWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    pHumanState_->SetHumanPosition( GetPionMedical().GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );
    SetState( sword::LogMedicalHandlingUpdate::waiting_for_collection, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::DoWaitingForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalSortingConsign::DoWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );

    MIL_AutomateLOG* pLogisticManager = GetPionMedical().FindLogisticManager();
    if( !pLogisticManager )
        return false;

    MIL_AutomateLOG* pLogisticSuperior = pLogisticManager->GetLogisticHierarchy().GetPrimarySuperior();
    if( pLogisticSuperior && pLogisticSuperior->MedicalHandleHumanForCollection( *pHumanState_ ) ||
        pLogisticManager->MedicalHandleHumanForCollection( *pHumanState_ ) )
    {
        pHumanState_ = 0;
        EnterStateFinished();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MedicalSortingConsign::Update()
{
    if( DecrementTimer() )
        return GetState() == sword::LogMedicalHandlingUpdate::finished;

    switch( GetState() )
    {
        case sword::LogMedicalHandlingUpdate::waiting_for_triage:
            if( DoWaitingForSorting() )
                EnterStateSorting();
            break;
        case sword::LogMedicalHandlingUpdate::triaging:
            EnterStateSearchingForHealingArea();
            break;
        case sword::LogMedicalHandlingUpdate::looking_for_medical_attention:
            DoSearchForHealingArea();
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
    return GetState() == sword::LogMedicalHandlingUpdate::finished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::Cancel()
{
    if( pDoctor_ )
    {
        GetPionMedical().StopUsingForLogistic( *pDoctor_ );
        pDoctor_ = 0;
    }
    PHY_MedicalConsign_ABC::Cancel();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::CouldNeedCollectionAmbulance
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_MedicalSortingConsign::CouldNeedCollectionAmbulance() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::IsATransportConsign
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
bool PHY_MedicalSortingConsign::IsATransportConsign() const
{
    return false;
}
