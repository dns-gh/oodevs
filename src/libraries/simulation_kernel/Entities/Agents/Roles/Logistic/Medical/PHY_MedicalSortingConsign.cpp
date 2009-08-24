//*****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalSortingConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:35 $
// $Revision: 3 $
// $Workfile: PHY_MedicalSortingConsign.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalSortingConsign.h"
#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_MedicalSortingConsign, "PHY_MedicalSortingConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalSortingConsign::PHY_MedicalSortingConsign( PHY_RoleInterface_Medical& medical, PHY_MedicalHumanState& humanState )
    : PHY_MedicalConsign_ABC( medical, humanState )
    , pDoctor_              ( 0 )
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
    , pDoctor_              ( 0 )
{
    // NOTHING
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
void PHY_MedicalSortingConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_MedicalConsign_ABC >( *this )
         & pDoctor_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::EnterStateWaitingForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::EnterStateWaitingForSorting()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    SetState( eWaitingForSorting );
    nTimer_ = 0; 
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
    if( !pDoctor_ ) // $$$ TODO if !HasUsable => changer de secteur de tri
        return false;
    
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
    SetState( eSorting );
    nTimer_ = (uint)( PHY_HumanWound::GetSortingTime() );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::EnterStateSearchingForHealingArea
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::EnterStateSearchingForHealingArea()
{
    assert( pHumanState_ );
    
    SetState( eSearchingForHealingArea );
    nTimer_ = 0;

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
// Name: PHY_MedicalSortingConsign::EnterStateWaitingForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_MedicalSortingConsign::EnterStateWaitingForCollection()
{
    assert( pHumanState_ );
    assert( !pDoctor_ );
    pHumanState_->SetHumanPosition( GetPionMedical().GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );
    nTimer_ = 0;
    SetState( eWaitingForCollection );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::DoWaitingForCollection
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalSortingConsign::DoWaitingForCollection()
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
// Name: PHY_MedicalSortingConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MedicalSortingConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() ) 
    {
        case eWaitingForSorting      : if( DoWaitingForSorting() )    EnterStateSorting                (); break;
        case eSorting                :                                EnterStateSearchingForHealingArea(); break;
        case eSearchingForHealingArea: DoSearchForHealingArea();                                           break;
        case eWaitingForCollection   : if( DoWaitingForCollection() ) EnterStateFinished               (); break;
        case eFinished               :                                                                     break;

        default:
            assert( false );
    }
    return GetState() == eFinished;
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
