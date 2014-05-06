// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Posture.h"
#include "ConsumptionComputer_ABC.h"
#include "DetectionComputer.h"
#include "MIL_Random.h"
#include "MIL_Random_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "DefaultPostureComputer.h"
#include "UrbanLocationComputer_ABC.h"
#include "VisionConeNotificationHandler_ABC.h"
#include "MoveComputer_ABC.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "protocol/ClientSenders.h"

using namespace posture;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Posture )

static const double rDeltaPercentageForNetwork = 0.05; //$$$ DEGUEU

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Posture::PHY_RolePion_Posture( MIL_Agent_ABC& pion )
    : owner_                               ( pion )
    , pCurrentPosture_                     ( &PHY_Posture::arret_ )
    , pLastPosture_                        ( &PHY_Posture::arret_ )
    , rPostureCompletionPercentage_        ( 0. )
    , rTimingFactor_                       ( 1. )
    , rStealthFactor_                      ( 1. ) // 1. == Non furtif
    , bDiscreteModeEnabled_                ( false )
    , bPostureHasChanged_                  ( true  )
    , bStealthFactorHasChanged_            ( true  )
    , bPercentageHasChanged_               ( true  )
    , bAmbianceSafetyHasChanged_           ( true  )
    , bIsStealth_                          ( false )
    , bIsParkedOnEngineerArea_             ( false )
    , bAmbianceSafety_                     ( false )
    , rLastPostureCompletionPercentageSent_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Posture::~PHY_RolePion_Posture()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::load
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nID;
    file >> boost::serialization::base_object< PHY_RoleInterface_Posture >( *this );
    file >> nID;
    pCurrentPosture_ = PHY_Posture::FindPosture( nID );
    file >> nID;
    pLastPosture_ = PHY_Posture::FindPosture( nID );
    file >> rPostureCompletionPercentage_
         >> bDiscreteModeEnabled_
         >> rTimingFactor_
         >> rStealthFactor_
         >> bIsStealth_
         >> rLastPostureCompletionPercentageSent_
         >> bIsParkedOnEngineerArea_
         >> bAmbianceSafety_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::save
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Posture >( *this );
    const unsigned current = pCurrentPosture_->GetID(),
             last    = pLastPosture_->GetID();
    file << current
         << last
         << rPostureCompletionPercentage_
         << bDiscreteModeEnabled_
         << rTimingFactor_
         << rStealthFactor_
         << bIsStealth_
         << rLastPostureCompletionPercentageSent_
         << bIsParkedOnEngineerArea_
         << bAmbianceSafety_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::ChangePostureCompletionPercentage
// Created: NLD 2005-07-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::ChangePostureCompletionPercentage( double rNewPercentage )
{
    if( rPostureCompletionPercentage_ == rNewPercentage )
        return;
    if( fabs( rLastPostureCompletionPercentageSent_ - rNewPercentage ) > rDeltaPercentageForNetwork || rNewPercentage == 0. || rNewPercentage == 1. )
        bPercentageHasChanged_ = true;
    rPostureCompletionPercentage_ = rNewPercentage;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::ChangePosture
// Created: NLD 2005-07-27
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::ChangePosture( const PHY_Posture& newPosture )
{
    if( pCurrentPosture_ == &newPosture )
        return false;
    pLastPosture_    = pCurrentPosture_;
    pCurrentPosture_ = &newPosture;
    bPostureHasChanged_    = true;
    bPercentageHasChanged_ = true;
    return true;
}

namespace
{
    struct Random : public MIL_Random_ABC
    {
        virtual double rand_oi( double min, double max, int ctxt ) const
        {
            return MIL_Random::rand_oi( min, max, ctxt );
        }
    } const random;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::UpdatePosture
// Created: LDC 2013-06-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::UpdatePosture( bool bIsDead )
{
    posture::DefaultPostureComputer computer(
            random, owner_.GetType().GetUnitType(), *pLastPosture_, *pCurrentPosture_,
            bIsDead, bDiscreteModeEnabled_, rPostureCompletionPercentage_,
            rStealthFactor_, rTimingFactor_, bIsParkedOnEngineerArea_ );
    owner_.Execute< PostureComputer_ABC >( computer );
    const PostureComputer_ABC::Results& result = computer.Result();
    bool changed = false;
    changed = ChangePosture( *result.newPosture_ );
    ChangePostureCompletionPercentage( result.postureCompletionPercentage_ );
    bIsStealth_ = result.bIsStealth_;
    return changed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Update( bool bIsDead )
{
    bool updated = UpdatePosture( bIsDead );
    while( 1. == rPostureCompletionPercentage_ && updated )
        updated = UpdatePosture( bIsDead );
    if( HasChanged() )
    {
        owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        owner_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetPosturePostePrepareGenie
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetPosturePostePrepareGenie()
{
    bIsParkedOnEngineerArea_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::UnsetPosturePostePrepareGenie
// Created: JVT 2004-09-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::UnsetPosturePostePrepareGenie()
{
    bIsParkedOnEngineerArea_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bPostureHasChanged_ )
    {
        msg().set_old_posture( pLastPosture_->GetAsnID() );
        msg().set_new_posture( pCurrentPosture_->GetAsnID() );
    }
    if( bPercentageHasChanged_ )
    {
        msg().set_posture_transition( static_cast< unsigned int >( rPostureCompletionPercentage_ * 100. ) );
        rLastPostureCompletionPercentageSent_ = rPostureCompletionPercentage_ ;
    }
    if( bStealthFactorHasChanged_ )
        msg().set_stealth( ( rStealthFactor_ < 1. ) );
    if( bAmbianceSafetyHasChanged_ )
    {
        msg().set_ambiance_safety( bAmbianceSafety_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_old_posture( pLastPosture_->GetAsnID() );
    msg().set_new_posture( pCurrentPosture_->GetAsnID() );
    msg().set_posture_transition( static_cast< unsigned int >( rPostureCompletionPercentage_ * 100. ) );
    rLastPostureCompletionPercentageSent_ = rPostureCompletionPercentage_;
    msg().set_stealth( rStealthFactor_ < 1. );
    msg().set_ambiance_safety( bAmbianceSafety_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetLastPosture
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_Posture& PHY_RolePion_Posture::GetLastPosture() const
{
    assert( pLastPosture_ );
    return *pLastPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetCurrentPosture
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_Posture& PHY_RolePion_Posture::GetCurrentPosture() const
{
    assert( pCurrentPosture_ );
    return *pCurrentPosture_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetPostureCompletionPercentage
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double PHY_RolePion_Posture::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::EnableDiscreteMode
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::EnableDiscreteMode()
{
    bDiscreteModeEnabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::DisableDiscreteMode
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::DisableDiscreteMode()
{
    bDiscreteModeEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::HasChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::HasChanged() const
{
    return bPostureHasChanged_ || bPercentageHasChanged_ || bAmbianceSafetyHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Clean()
{
    bPostureHasChanged_           = false;
    bPercentageHasChanged_        = false;
    bStealthFactorHasChanged_     = false;
    bAmbianceSafetyHasChanged_    = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetStealthFactor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetStealthFactor( double rValue )
{
    assert( rValue >= 0. && rValue <= 1. );
    if( rValue != rStealthFactor_ )
    {
        bStealthFactorHasChanged_ = true;
        rStealthFactor_ = rValue;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetTimingFactor
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetTimingFactor( double rFactor )
{
    assert( rFactor > 0. );
    rTimingFactor_ = rFactor == 0. ? 1. : std::abs( rFactor );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetPosture
// Created: MCO 2013-08-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetPosture( const PHY_Posture& posture )
{
    ChangePosture( posture );
    ChangePostureCompletionPercentage( 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::IsStealth
// Created: AGE 2004-12-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::IsStealth() const
{
    return bIsStealth_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SetAmbianceSafety
// Created: MMC 2013-04-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetAmbianceSafety( bool safety )
{
    bAmbianceSafetyHasChanged_ = true;
    bAmbianceSafety_ = safety;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Execute( dotation::ConsumptionComputer_ABC& algorithm ) const
{
    algorithm.SetConsumptionMode( GetCurrentPosture().GetConsumptionMode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Execute( detection::DetectionComputer& algorithm ) const
{
    if( bIsStealth_ && algorithm.GetTarget() == owner_ )
        algorithm.NotifyStealth();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Execute
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Execute( urbanLocation::UrbanLocationComputer_ABC& algorithm ) const
{
    if( pCurrentPosture_ == &PHY_Posture::poste_ )
        algorithm.SetUrbanDeployment( static_cast< float >( rPostureCompletionPercentage_ ) );
    if( pCurrentPosture_ == &PHY_Posture::posteAmenage_ )
        algorithm.SetUrbanDeployment( 1.f );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::IsMovingPosture
// Created: SLI 2013-01-25
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::IsMovingPosture() const
{
    return pCurrentPosture_ == &PHY_Posture::mouvement_
        || pCurrentPosture_ == &PHY_Posture::mouvementDiscret_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Hide
// Created: LDC 2013-05-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Show
// Created: LDC 2013-05-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Show( const MT_Vector2D& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Follow
// Created: LDC 2013-05-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Follow( const MIL_Agent_ABC& agent )
{
    const PHY_RolePion_Posture& posture = agent.GetRole< PHY_RolePion_Posture >();
    ChangePosture( posture.GetCurrentPosture() );
    ChangePostureCompletionPercentage( posture.GetPostureCompletionPercentage() );
    if( posture.bIsStealth_ )
        bIsStealth_ = posture.bIsStealth_;
}
