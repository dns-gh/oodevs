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
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/PostureComputer_ABC.h"
#include "simulation_kernel/PostureComputerFactory_ABC.h"
#include "simulation_kernel/ConsumptionComputer_ABC.h"
#include "simulation_kernel/DetectionComputer_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include "simulation_kernel/PerceptionDistanceComputer_ABC.h"
#include "simulation_kernel/UrbanLocationComputer_ABC.h"
#include "simulation_kernel/MoveComputer_ABC.h"
#include "MIL_Random_ABC.h"
#include "MIL_Random.h"

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
    , rElongationFactor_                   ( 1. )
    , rTimingFactor_                       ( 1. )
    , rStealthFactor_                      ( 1. ) // 1. == Non furtif
    , rInstallationState_                  ( 0. )
    , bDiscreteModeEnabled_                ( false )
    , bPostureHasChanged_                  ( true  )
    , bStealthFactorHasChanged_            ( true  )
    , bPercentageHasChanged_               ( true  )
    , bAmbianceSafetyHasChanged_           ( true  )
    , bIsStealth_                          ( false )
    , bInstallationSetUpInProgress_        ( false )
    , bIsParkedOnEngineerArea_             ( false )
    , bInstallationStateHasChanged_        ( true )
    , rLastPostureCompletionPercentageSent_( 0. )
    , rLastInstallationStateSent_          ( 0. )
    , bAmbianceSafety_                     ( false )
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
         >> rElongationFactor_
         >> bDiscreteModeEnabled_
         >> rTimingFactor_
         >> rStealthFactor_
         >> bIsStealth_
         >> rLastPostureCompletionPercentageSent_
         >> rInstallationState_
         >> rLastInstallationStateSent_
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
         << rElongationFactor_
         << bDiscreteModeEnabled_
         << rTimingFactor_
         << rStealthFactor_
         << bIsStealth_
         << rLastPostureCompletionPercentageSent_
         << rInstallationState_
         << rLastInstallationStateSent_
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
    // Network
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
    std::auto_ptr< PostureComputer_ABC > computer( owner_.GetAlgorithms().postureComputerFactory_->Create( random, owner_.GetType().GetUnitType(), *pCurrentPosture_,
                                                                                                           bIsDead, bDiscreteModeEnabled_, rPostureCompletionPercentage_,
                                                                                                           rStealthFactor_, rTimingFactor_, bIsParkedOnEngineerArea_ ) );
    owner_.Execute( *computer );
    const PostureComputer_ABC::Results& result = computer->Result();
    bool changed = false;
    if( result.newPosture_ )
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
    Uninstall();
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
// Name: PHY_RolePion_Posture::Install
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Install()
{
    bInstallationSetUpInProgress_ = true;
    if( rInstallationState_ >= 1. )
        return;
    const double rTime = owner_.GetType().GetUnitType().GetInstallationTime();
    if( rTime == 0 )
        rInstallationState_ = 1.;
    else
    {
        rInstallationState_ += 1. / rTime;
        rInstallationState_ = std::min( 1., rInstallationState_ );
    }
    if( fabs( rLastInstallationStateSent_ - rInstallationState_ ) > rDeltaPercentageForNetwork || rInstallationState_ == 0. || rInstallationState_ == 1. )
        bInstallationStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Uninstall
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Uninstall()
{
    if( rInstallationState_ <= 0. || bInstallationSetUpInProgress_ )
        return;
    const double rTime = owner_.GetType().GetUnitType().GetUninstallationTime();
    if( rTime == 0 )
        rInstallationState_ = 0.;
    else
    {
        rInstallationState_ -= 1. / rTime;
        rInstallationState_ = std::max( 0., rInstallationState_ );
    }
    if( fabs( rLastInstallationStateSent_ - rInstallationState_ ) > rDeltaPercentageForNetwork || rInstallationState_ == 0. || rInstallationState_ == 1. )
        bInstallationStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bPostureHasChanged_ )
    {
        msg().set_old_posture( pLastPosture_   ->GetAsnID() );
        msg().set_new_posture( pCurrentPosture_->GetAsnID() );
    }
    if( bPercentageHasChanged_ )
    {
        msg().set_posture_transition( static_cast< unsigned int >( rPostureCompletionPercentage_ * 100. ) );
        rLastPostureCompletionPercentageSent_ = rPostureCompletionPercentage_ ;
    }
    if( bStealthFactorHasChanged_ )
        msg().set_stealth( ( rStealthFactor_ < 1. ) );
    if( bInstallationStateHasChanged_ )
    {
        msg().set_installation( static_cast< unsigned int >( rInstallationState_ * 100. ) );
        rLastInstallationStateSent_ = rInstallationState_;
    }
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
    msg().set_old_posture( pLastPosture_   ->GetAsnID() );
    msg().set_new_posture( pCurrentPosture_->GetAsnID() );
    msg().set_posture_transition( static_cast< unsigned int >( rPostureCompletionPercentage_ * 100. ) );
    rLastPostureCompletionPercentageSent_        = rPostureCompletionPercentage_;
    msg().set_stealth( ( rStealthFactor_ < 1. ) );
    msg().set_installation( static_cast< unsigned int >( rInstallationState_ * 100. ) );
    msg().set_ambiance_safety( bAmbianceSafety_ );
    rLastInstallationStateSent_                = rInstallationState_;
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
    return bPostureHasChanged_ || bPercentageHasChanged_ || bInstallationStateHasChanged_ || bAmbianceSafetyHasChanged_;
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
    bInstallationSetUpInProgress_ = false;
    bInstallationStateHasChanged_ = false;
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
// Name: PHY_RolePion_Posture::SetElongationFactor
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::SetElongationFactor( double rFactor )
{
    assert( rFactor > 0. );
    if( rFactor > 0. )
        rElongationFactor_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::GetElongationFactor
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
double PHY_RolePion_Posture::GetElongationFactor() const
{
    return rElongationFactor_;//@TODO REMOVE
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
// Name: PHY_RolePion_Posture::IsInstalled
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::IsInstalled() const
{
    return rInstallationState_ >= 1.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::IsUninstalled
// Created: NLD 2006-08-10
// -----------------------------------------------------------------------------
bool PHY_RolePion_Posture::IsUninstalled() const
{
    return rInstallationState_ <= 0.;
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
void PHY_RolePion_Posture::Execute( detection::DetectionComputer_ABC& algorithm ) const
{
    if( bIsStealth_ && algorithm.GetTarget() == owner_ )
        algorithm.NotifyStealth();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Posture::Execute
// Created: MGD 2009-10-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Posture::Execute( detection::PerceptionDistanceComputer_ABC& algorithm ) const
{
    //algorithm.AddModifier( rElongationFactor_ );
    algorithm.AddElongationFactor( rElongationFactor_ );
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
    bIsStealth_ = posture.bIsStealth_;
}
