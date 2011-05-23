// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalEvacuationAmbulance.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 14:55 $
// $Revision: 3 $
// $Workfile: PHY_MedicalEvacuationAmbulance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalEvacuationAmbulance.h"
#include "PHY_MedicalEvacuationConsign.h"
#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/log/MIL_AgentPionLOG_ABC.h"
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MedicalEvacuationAmbulance )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance constructor
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationAmbulance::PHY_MedicalEvacuationAmbulance( PHY_RoleInterface_Medical& medical, PHY_ComposantePion& compAmbulance )
    : pMedical_        ( &medical )
    , pCompAmbulance_  ( &compAmbulance )
    , nState_          ( eWaiting )
    , nTimer_          ( 0 )
    , rNbrHumanHandled_( 0. )
    , consigns_        ()
{
    pMedical_->StartUsingForLogistic( *pCompAmbulance_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationAmbulance::PHY_MedicalEvacuationAmbulance()
    : pMedical_        ( 0 )
    , pCompAmbulance_  ( 0 )
    , nState_          ( eWaiting )
    , nTimer_          ( 0 )
    , rNbrHumanHandled_( 0. )
    , consigns_        ()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance destructor
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_MedicalEvacuationAmbulance::~PHY_MedicalEvacuationAmbulance()
{
    assert( pMedical_ );
    assert( pCompAmbulance_ );

    pMedical_->StopUsingForLogistic( *pCompAmbulance_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalEvacuationAmbulance::serialize( Archive& file, const unsigned int )
{
    file & pMedical_
         & pCompAmbulance_
         & consigns_
         & nState_
         & nTimer_
         & rNbrHumanHandled_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::RegisterHuman
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationAmbulance::RegisterHuman( PHY_MedicalEvacuationConsign& consign )
{
    assert( pCompAmbulance_ );
    assert( std::find( consigns_.begin(), consigns_.end(), &consign ) == consigns_.end() );

    if( !pCompAmbulance_->GetType().CanEvacuateCasualty( consign.GetHumanState().GetHuman() ) )
        return false;

    if( consigns_.size() >= pCompAmbulance_->GetType().GetAmbulanceEvacuationCapacity() )
        return false;

    switch( nState_ )
    {
        case eWaiting: break;
        case eGoingTo: consign.EnterStateEvacuationGoingTo(); break;
        case eLoading: consign.EnterStateEvacuationLoading(); break;
        default:
            return false;
    }
    consigns_.push_back( &consign );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::UnregisterHuman
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationAmbulance::UnregisterHuman( PHY_MedicalEvacuationConsign& consign )
{
    IT_ConsignVector itConsign = std::find( consigns_.begin(), consigns_.end(), &consign );
    assert( itConsign != consigns_.end() );
    consigns_.erase( itConsign );
    if( consigns_.empty() )
        EnterStateFinished();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::EnterStateGoingTo
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationAmbulance::EnterStateGoingTo()
{
    assert( !consigns_.empty() );
    assert( pMedical_ );
    assert( pCompAmbulance_ );

    MT_Vector2D vHumansBarycenter;
    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
    {
        (**itConsign).EnterStateEvacuationGoingTo();
        vHumansBarycenter += (**itConsign).GetHumanState().GetHumanPosition();
    }
    vHumansBarycenter /= static_cast< double >( consigns_.size() );

    nState_ = eGoingTo;
    nTimer_ = pCompAmbulance_->ApproximateTravelTime( pMedical_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition(), vHumansBarycenter );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::EnterStateLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationAmbulance::EnterStateLoading()
{
    nState_           = eLoading;
    nTimer_           = 0;
    rNbrHumanHandled_ = 0.;

    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateEvacuationLoading();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::DoLoading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationAmbulance::DoLoading()
{
    assert( pCompAmbulance_ );

    rNbrHumanHandled_ += pCompAmbulance_->GetType().GetNbrHumansLoadedForEvacuationPerTimeStep();
    CIT_ConsignVector itConsign;
    for( itConsign = consigns_.begin(); itConsign != consigns_.end() && rNbrHumanHandled_ >= 1.; ++itConsign )
    {
        PHY_MedicalEvacuationConsign& consign = **itConsign;
        if( consign.EnterStateEvacuationWaitingForFullLoading() )
            rNbrHumanHandled_ -= 1.;
    }
    return itConsign == consigns_.end();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::EnterStateGoingFrom
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationAmbulance::EnterStateGoingFrom()
{
    assert( !consigns_.empty() );
    assert( pMedical_ );
    assert( pCompAmbulance_ );

    MT_Vector2D vHumansBarycenter;
    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
    {
        (**itConsign).EnterStateEvacuationGoingFrom();
        vHumansBarycenter += (**itConsign).GetHumanState().GetHumanPosition();
    }
    vHumansBarycenter /= static_cast< double >( consigns_.size() );

    nState_ = eGoingFrom;
    nTimer_ = pCompAmbulance_->ApproximateTravelTime( vHumansBarycenter, pMedical_->GetPion().GetRole< PHY_RoleInterface_Location>().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::EnterStateUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationAmbulance::EnterStateUnloading()
{
    nState_           = eUnloading;
    nTimer_           = 0;
    rNbrHumanHandled_ = 0.;

    for( CIT_ConsignVector itConsign = consigns_.begin(); itConsign != consigns_.end(); ++itConsign )
        (**itConsign).EnterStateEvacuationUnloading();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::DoUnloading
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationAmbulance::DoUnloading()
{
    assert( pCompAmbulance_ );

    rNbrHumanHandled_ += pCompAmbulance_->GetType().GetNbrHumansUnloadedForEvacuationPerTimeStep();
    while( rNbrHumanHandled_ >= 1. && !consigns_.empty() )
    {
        PHY_MedicalEvacuationConsign& consign = *consigns_.back();
        consign.ChooseStateAfterEvacuation();
        consigns_.pop_back();
        rNbrHumanHandled_ -= 1.;
    }
    return consigns_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::EnterStateFinished
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_MedicalEvacuationAmbulance::EnterStateFinished()
{
    assert( consigns_.empty() );
    nState_ = eFinished;
    nTimer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalEvacuationAmbulance::Update
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalEvacuationAmbulance::Update()
{
    if( --nTimer_ > 0 )
        return nState_ == eFinished;

    switch( nState_ )
    {
        case eWaiting   :                     EnterStateGoingTo  (); break;
        case eGoingTo   :                     EnterStateLoading  (); break;
        case eLoading   : if( DoLoading() )   EnterStateGoingFrom(); break;
        case eGoingFrom :                     EnterStateUnloading(); break;
        case eUnloading : if( DoUnloading() ) EnterStateFinished (); break;
        case eFinished  : break;
        default:
            assert( false );
    }
    return nState_ == eFinished;
}
