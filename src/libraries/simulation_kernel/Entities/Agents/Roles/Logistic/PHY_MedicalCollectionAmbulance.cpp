// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalCollectionAmbulance.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 14:55 $
// $Revision: 2 $
// $Workfile: PHY_MedicalCollectionAmbulance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalCollectionAmbulance.h"
#include "PHY_MedicalCollectionConsign.h"
#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MedicalCollectionAmbulance )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance constructor
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionAmbulance::PHY_MedicalCollectionAmbulance( PHY_RoleInterface_Medical& medical, PHY_ComposantePion& compAmbulance )
    : pMedical_           ( &medical )
    , pCompAmbulance_     ( &compAmbulance )
    , consigns_           ()
    , nState_             ( eWaiting )
    , nTimer_             ( 0 )
    , rNbrHumanHandled_   ( 0. )
    , bEmergencyAmbulance_( false )
    , pSortingArea_       ( 0 )
{
    pMedical_->StartUsingForLogistic( *pCompAmbulance_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionAmbulance::PHY_MedicalCollectionAmbulance()
    : pMedical_           ( 0 )
    , pCompAmbulance_     ( 0 )
    , consigns_           ()
    , nState_             ( eWaiting )
    , nTimer_             ( 0 )
    , rNbrHumanHandled_   ( 0. )
    , bEmergencyAmbulance_( false )
    , pSortingArea_       ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance destructor
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionAmbulance::~PHY_MedicalCollectionAmbulance()
{
    assert( pMedical_ );
    assert( pCompAmbulance_ );

    pMedical_->StopUsingForLogistic( *pCompAmbulance_ );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalCollectionAmbulance::serialize( Archive& file, const unsigned int )
{
    file & pMedical_
         & pCompAmbulance_
         & consigns_
         & nState_
         & nTimer_
         & bEmergencyAmbulance_
         & rNbrHumanHandled_
         & pSortingArea_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::RegisterHuman
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::RegisterHuman( PHY_MedicalCollectionConsign& consign )
{
    assert( std::find( consigns_.begin(), consigns_.end(), &consign ) == consigns_.end() );
    assert( pCompAmbulance_ );

    if( !pCompAmbulance_->GetType().CanCollectCasualty( consign.GetHumanState().GetHuman() ) )
        return false;

    if( consigns_.size() >= pCompAmbulance_->GetType().GetAmbulanceCollectionCapacity() )
        return false;

    if( bEmergencyAmbulance_ && !consign.IsAnEmergency() )
        return false;

    switch( nState_ )
    {
        case eWaiting: break;
        case eLoading: consign.EnterStateCollectionLoading(); break;
        default:
            return false;
    }

    consigns_.push_back( &consign );
    bEmergencyAmbulance_ |= consign.IsAnEmergency();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::UnregisterHuman
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::UnregisterHuman( PHY_MedicalCollectionConsign& consign )
{
    IT_ConsignVector itConsign = std::find( consigns_.begin(), consigns_.end(), &consign );
    if( itConsign != consigns_.end() )
        consigns_.erase( itConsign );
    if( consigns_.empty() )
        EnterStateFinished();
}

// =============================================================================
//
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateLoading()
{
    nState_           = eLoading;
    nTimer_           = 0;
    rNbrHumanHandled_ = 0.;

    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateCollectionLoading();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::DoLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::DoLoading()
{
    assert( pCompAmbulance_ );
    assert( pMedical_ );

    rNbrHumanHandled_ += pCompAmbulance_->GetType().GetNbrHumansLoadedForCollectionPerTimeStep();

    CIT_ConsignVector itConsign;
    for( itConsign = consigns_.begin(); itConsign != consigns_.end() && rNbrHumanHandled_ >= 1.; ++itConsign )
    {
        PHY_MedicalCollectionConsign& consign = **itConsign;
        if( consign.EnterStateCollectionWaitingForFullLoading() )
            rNbrHumanHandled_ -= 1.;
    }
    if( rNbrHumanHandled_ >= 1. )
        rNbrHumanHandled_ = 0.;

    if( itConsign != consigns_.end() )
        return false;

    if( consigns_.size() == pCompAmbulance_->GetType().GetAmbulanceCollectionCapacity() )
        return true;

    MIL_AutomateLOG* pLogisticManager = pMedical_->FindLogisticManager();
    if( !pLogisticManager )
        return false;
    return pLogisticManager->MedicalCanCollectionAmbulanceGo( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateSearchingForSortingArea
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateSearchingForSortingArea()
{
    nState_ = eSearchingForSortingArea;
    nTimer_ = 0;
    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateSearchingForSortingArea();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::DoSearchForSortingArea
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::DoSearchForSortingArea()
{
    assert( !pSortingArea_ );
    assert( pMedical_ );

    MIL_AutomateLOG* pLogisticManager = pMedical_->FindLogisticManager();
    if( !pLogisticManager )
        return false;
    MIL_AutomateLOG* pLogisticSuperior = pLogisticManager->GetLogisticHierarchy().GetPrimarySuperior();
    if( !pLogisticSuperior )
        return true; // $$$ Bof : pour sortir les human states qui ne seront jamais traités
    pSortingArea_ = pLogisticSuperior->MedicalReserveForSorting( *this );
    return pSortingArea_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::ChooseStateFromSortingAreaState
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::ChooseStateFromSortingAreaState()
{
    if( !pSortingArea_ )
    {
        for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
            (**itConsign).NotifyOutOfMedicalSystem();
        consigns_.clear();
        EnterStateFinished();
        return;
    }
    EnterStateGoingTo();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateGoingTo
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateGoingTo()
{
    assert( !consigns_.empty() );
    assert( pSortingArea_ );
    assert( pCompAmbulance_ );
    assert( pMedical_ );

    nState_ = eGoingTo;
    nTimer_ = pCompAmbulance_->ApproximateTravelTime( pMedical_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition(), pSortingArea_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition() );
    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateCollectionGoingTo();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateUnloading()
{
    assert( pSortingArea_ );
    nState_           = eUnloading;
    nTimer_           = 0;
    rNbrHumanHandled_ = 0.;

    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateCollectionUnloading();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::DoUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::DoUnloading()
{
    assert( pSortingArea_ );
    assert( pCompAmbulance_ );

    rNbrHumanHandled_ += pCompAmbulance_->GetType().GetNbrHumansUnloadedForCollectionPerTimeStep();
    while( rNbrHumanHandled_ >= 1. && !consigns_.empty() )
    {
        PHY_MedicalCollectionConsign& consign = *consigns_.back();
        consigns_.pop_back();
        consign.TransferToSortingArea( *pSortingArea_ );
        rNbrHumanHandled_ -= 1.;
    }
    return consigns_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateGoingFrom
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateGoingFrom()
{
    assert( pMedical_ );
    assert( pCompAmbulance_ );
    assert( pSortingArea_ );
    assert( consigns_.empty() );

    nState_ = eGoingFrom;
    nTimer_ = pCompAmbulance_->ApproximateTravelTime( pSortingArea_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition(), pMedical_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition() );
    pSortingArea_->CancelReservationForSorting( *this );
    pSortingArea_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateFinished
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateFinished()
{
    if( pSortingArea_ )
    {
        pSortingArea_->CancelReservationForSorting( *this );
        pSortingArea_ = 0;
    }
    assert( consigns_.empty() );
    nState_ = eFinished;
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::Update
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::Update()
{
    if( --nTimer_ > 0 )
        return nState_ == eFinished;

    switch( nState_ )
    {
        case eWaiting                 :                                EnterStateLoading                (); break;
        case eLoading                 : if( DoLoading() )              EnterStateSearchingForSortingArea(); break;
        case eSearchingForSortingArea : if( DoSearchForSortingArea() ) ChooseStateFromSortingAreaState  (); break;
        case eGoingTo                 :                                EnterStateUnloading              (); break;
        case eUnloading               : if( DoUnloading() )            EnterStateGoingFrom              (); break;
        case eGoingFrom               :                                EnterStateFinished               (); break;
        case eFinished                :                                                                     break;
        default:
            assert( false );
    }
    return nState_ == eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::GetNbrHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
unsigned int PHY_MedicalCollectionAmbulance::GetNbrHumans() const
{
    return static_cast< unsigned int >( consigns_.size() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::GetNbrHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
int PHY_MedicalCollectionAmbulance::GetTimer() const
{
    return nTimer_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::IsAnEmergency
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::IsAnEmergency() const
{
    return bEmergencyAmbulance_;
}


// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::GetNbrHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::Cancel()
{
    T_ConsignVector tmpConsigns = consigns_;
    for( CIT_ConsignVector itConsign = tmpConsigns.begin(); itConsign != tmpConsigns.end(); ++itConsign )
        (**itConsign).EnterStateWaitingForCollection();
    EnterStateFinished();
}
