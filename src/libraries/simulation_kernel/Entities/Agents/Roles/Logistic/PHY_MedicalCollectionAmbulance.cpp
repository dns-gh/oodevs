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
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance constructor
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionAmbulance::PHY_MedicalCollectionAmbulance( PHY_RoleInterface_Medical& medical, PHY_ComposantePion& compAmbulance )
    : pMedical_( &medical )
    , pCompAmbulance_( &compAmbulance )
    , nState_( eWaiting )
    , nTimer_( 0 )
    , rNbrHumanHandled_( 0. )
    , pDestinationArea_( 0 )
    , rInfoTimer_( 0 )
    , bEmergencyAmbulance_( false )
    , bSort_( true )
{
    pMedical_->StartUsingForLogistic( *pCompAmbulance_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalCollectionAmbulance::PHY_MedicalCollectionAmbulance()
    : pMedical_( 0 )
    , pCompAmbulance_( 0 )
    , nState_( eWaiting )
    , nTimer_( 0 )
    , rNbrHumanHandled_( 0. )
    , pDestinationArea_( 0 )
    , rInfoTimer_( 0 )
    , bEmergencyAmbulance_( false )
    , bSort_( true )
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

void PHY_MedicalCollectionAmbulance::load( MIL_CheckPointInArchive& a, const unsigned int )
{
    a & pMedical_;
    a & pCompAmbulance_;
    a & consigns_;
    a & nState_;
    a & nTimer_;
    a & bEmergencyAmbulance_;
    a & rNbrHumanHandled_;
    a & pDestinationArea_;
    a & bSort_;
}

void PHY_MedicalCollectionAmbulance::save( MIL_CheckPointOutArchive& a, const unsigned int ) const
{
    a & pMedical_;
    a & pCompAmbulance_;
    a & consigns_;
    a & nState_;
    a & nTimer_;
    a & bEmergencyAmbulance_;
    a & rNbrHumanHandled_;
    a & pDestinationArea_;
    a & bSort_;
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

    if( !consign.HasValidHumanState() )
        return false;

    if( !pCompAmbulance_->GetType().CanCollectCasualty( consign.GetHumanState().GetHuman() ) )
        return false;

    if( consigns_.size() >= pCompAmbulance_->GetType().GetAmbulanceCollectionCapacity() )
        return false;

    if( bEmergencyAmbulance_ && !consign.IsAnEmergency() )
        return false;

    if( 0 == GetNbrHumans() )
        bSort_ = consign.GetHumanState().NeedSorting();
    else if( bSort_ != consign.GetHumanState().NeedSorting() )
        return false;

    switch( nState_ )
    {
        case eWaiting:
            break;
        case eLoading:
            consign.EnterStateCollectionLoading();
            rInfoTimer_ += 1 / pCompAmbulance_->GetType().GetNbrHumansLoadedForCollectionPerTimeStep();
            break;
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

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateLoading()
{
    nState_ = eLoading;
    nTimer_ = 0;
    rNbrHumanHandled_ = 0;
    rInfoTimer_ = consigns_.size() / pCompAmbulance_->GetType().GetNbrHumansLoadedForCollectionPerTimeStep();
    for( auto itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
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
    -- rInfoTimer_;
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
// Name: PHY_MedicalCollectionAmbulance::EnterStateSearchingForDestinationArea
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateSearchingForDestinationArea()
{
    nState_ = eSearchingForDestinationArea;
    nTimer_ = 0;
    for( auto itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateSearchingForDestinationArea();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::DoSearchForDestinationArea
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::DoSearchForDestinationArea()
{
    assert( !pDestinationArea_ );
    assert( pMedical_ );

    MIL_AutomateLOG* pLogisticManager = pMedical_->FindLogisticManager();
    if( !pLogisticManager )
        return false;
    MIL_AutomateLOG* pLogisticSuperior = pLogisticManager->GetLogisticHierarchy().GetPrimarySuperior();
    if( bSort_ )
    {
        if( !pLogisticSuperior )
            return true; // $$$ Bof : pour sortir les human states qui ne seront jamais traités
        pDestinationArea_ = pLogisticSuperior->MedicalReserveForSorting( *this );
    }
    else
    {   
        auto it = consigns_.begin();
        if( it != consigns_.end() )
            pDestinationArea_ = pLogisticManager->MedicalFindAlternativeHealingHandler( const_cast< PHY_MedicalHumanState& > ((*it)->GetHumanState() ) );
        if( !pDestinationArea_ && !pLogisticSuperior )
            return true; // $$$ Bof : pour sortir les human states qui ne seront jamais traités
    }
    return pDestinationArea_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::ChooseStateFromDestinationAreaState
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::ChooseStateFromDestinationAreaState()
{
    if( !pDestinationArea_ )
    {
        for( auto itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
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
    assert( pDestinationArea_ );
    assert( pCompAmbulance_ );
    assert( pMedical_ );

    nState_ = eGoingTo;
    nTimer_ = pCompAmbulance_->ApproximateTravelTime( pMedical_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition(), pDestinationArea_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition() );
    for( auto itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateCollectionGoingTo();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateUnloading()
{
    assert( pDestinationArea_ );
    nState_           = eUnloading;
    nTimer_           = 0;
    rNbrHumanHandled_ = 0.;
    rInfoTimer_ = consigns_.size() / pCompAmbulance_->GetType().GetNbrHumansUnloadedForCollectionPerTimeStep();
    for( auto itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateCollectionUnloading();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::DoUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalCollectionAmbulance::DoUnloading()
{
    assert( pDestinationArea_ );
    assert( pCompAmbulance_ );
    -- rInfoTimer_;
    rNbrHumanHandled_ += pCompAmbulance_->GetType().GetNbrHumansUnloadedForCollectionPerTimeStep();
    while( rNbrHumanHandled_ >= 1. && !consigns_.empty() )
    {
        PHY_MedicalCollectionConsign& consign = *consigns_.back();
        consigns_.pop_back();
        consign.TransferToDestinationArea( *pDestinationArea_ );
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
    assert( pDestinationArea_ );
    assert( consigns_.empty() );

    nState_ = eGoingFrom;
    nTimer_ = pCompAmbulance_->ApproximateTravelTime( pDestinationArea_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition(), pMedical_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition() );
    if( bSort_ )
        pDestinationArea_->CancelReservationForSorting( *this );
    pDestinationArea_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::EnterStateFinished
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalCollectionAmbulance::EnterStateFinished()
{
    if( pDestinationArea_ )
    {
        if( bSort_ )
            pDestinationArea_->CancelReservationForSorting( *this );
        pDestinationArea_ = 0;
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
        case eWaiting                     :                                    EnterStateLoading                    (); break;
        case eLoading                     : if( DoLoading() )                  EnterStateSearchingForDestinationArea(); break;
        case eSearchingForDestinationArea : if( DoSearchForDestinationArea() ) ChooseStateFromDestinationAreaState  (); break;
        case eGoingTo                     :                                    EnterStateUnloading                  (); break;
        case eUnloading                   : if( DoUnloading() )                EnterStateGoingFrom                  (); break;
        case eGoingFrom                   :                                    EnterStateFinished                   (); break;
        case eFinished                    :                                                                             break;
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
    if( nState_ == eLoading || nState_ == eUnloading )
        return static_cast< int >( rInfoTimer_ );
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
