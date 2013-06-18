// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/DefaultPostureComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PHY_Posture.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PostureTime_ABC.h"
#include "MIL_Random_ABC.h"
#include "MIL_Random.h"

using namespace posture;

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::DefaultPostureComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultPostureComputer::DefaultPostureComputer( const MIL_Random_ABC& random, const PostureTime_ABC& time, const PHY_Posture& posture, bool bIsDead,
                                                bool bDiscreteModeEnabled, double rCompletionPercentage, double rStealthFactor,
                                                double rTimingFactor, bool isParkedOnEngineerArea )
    : random_                ( random )
    , time_                  ( time )
    , posture_               ( posture )
    , bIsDead_               ( bIsDead )
    , bDiscreteModeEnabled_  ( bDiscreteModeEnabled )
    , rCompletionPercentage_ ( rCompletionPercentage )
    , rStealthFactor_        ( rStealthFactor )
    , isParkedOnEngineerArea_( isParkedOnEngineerArea )
    , modifier_              ( 1 / rTimingFactor )
    , bMoving_               ( false )
    , bStopped_              ( false )
    , bIsLoaded_             ( false )
    , results_               ( rCompletionPercentage )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::~DefaultPostureComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultPostureComputer::~DefaultPostureComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::SetPostureMovement
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::SetPostureMovement()
{
    bMoving_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::UnsetPostureMovement
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::UnsetPostureMovement()
{
    if( !bMoving_ ) // $$$$ LDC Role action flying can force movement with 0 horizontal speed.
        bStopped_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::NotifyLoaded
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::NotifyLoaded()
{
    bIsLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::AddCoefficientModifier
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::AddCoefficientModifier( double coef )
{
    modifier_ *= coef;
}

namespace
{
    void ComputeDeathPosture( PostureComputer_ABC::Results& results )
    {
        results.newPosture_ = &PHY_Posture::arret_;
        results.postureCompletionPercentage_ = 1.;
        results.bIsStealth_ = false;
    }
    void ComputeStealthMode( PostureComputer_ABC::Results& results, const MIL_Random_ABC& random, double stealthFactor )
    {
        results.bIsStealth_ = random.rand_oi( 0., 1., MIL_Random::ePerception ) > stealthFactor;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::Result
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
const PostureComputer_ABC::Results& DefaultPostureComputer::Result()
{
    if( bIsDead_ )
    {
        ComputeDeathPosture( results_ );
        return results_;
    }
    ComputeStealthMode( results_, random_, rStealthFactor_ );
    if( bMoving_ )
        ComputeMovingPosture();
    else
        ComputeStopPosture();
    return results_;
}

namespace
{
    double Remove( double currentCompletion, double postureTime )
    {
        if( postureTime == 0 )
            return 0;
        return std::max( 0., currentCompletion - ( 1 / postureTime ) );
    }
    void ComputePreviousPosture( PostureComputer_ABC::Results& results, const PHY_Posture& previous )
    {
        results.postureCompletionPercentage_ = 1;
        results.newPosture_ = &previous;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::ComputeMovingPosture
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
void DefaultPostureComputer::ComputeMovingPosture()
{
    results_.postureCompletionPercentage_ = ComputeCompletion( &PostureTime_ABC::GetPostureTearDownTime, &Remove );
    if( results_.postureCompletionPercentage_ > 0 )
        return;
    if( posture_.GetPreviousAutoPosture() )
        return ComputePreviousPosture( results_, *posture_.GetPreviousAutoPosture() );
    else if( ! bIsLoaded_ )
        results_.newPosture_ = &PHY_Posture::posteReflexe_;
    else if( bDiscreteModeEnabled_ )
        results_.newPosture_ = &PHY_Posture::mouvementDiscret_;
    else
    {
        results_.newPosture_ = &PHY_Posture::mouvement_;
        results_.postureCompletionPercentage_ = 1;
    }
}

namespace
{
    double Add( double currentCompletion, double postureTime )
    {
        if( postureTime == 0 )
            return 1;
        return std::min( 1., currentCompletion + ( 1 / postureTime ) );
    }
    void ComputeNextPosture( PostureComputer_ABC::Results& results, const PHY_Posture& current )
    {
        const PHY_Posture* pNextAutoPosture = current.GetNextAutoPosture();
        if( !pNextAutoPosture )
            return;
        results.postureCompletionPercentage_ = 0;
        results.newPosture_ = pNextAutoPosture;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::ComputeStopPosture
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
void DefaultPostureComputer::ComputeStopPosture()
{
    if( bStopped_ && ( &posture_ == &PHY_Posture::mouvement_
                    || &posture_ == &PHY_Posture::mouvementDiscret_ ) )
    {
        results_.newPosture_ = &PHY_Posture::arret_;
        results_.postureCompletionPercentage_ = 0;
        return;
    }
    results_.postureCompletionPercentage_ = ComputeCompletion( &PostureTime_ABC::GetPostureSetupTime, &Add );
    if( results_.postureCompletionPercentage_ < 1 )
        return;
    if( isParkedOnEngineerArea_ )
    {
        results_.newPosture_ = &PHY_Posture::postePrepareGenie_;
        return;
    }
    return ComputeNextPosture( results_, posture_ );
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::ComputeCompletion
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
template< typename GetTime, typename Accumulator >
double DefaultPostureComputer::ComputeCompletion( const GetTime& time, const Accumulator& accumulator ) const
{
    const PHY_Posture* next = isParkedOnEngineerArea_ ? &PHY_Posture::postePrepareGenie_ : posture_.GetNextAutoPosture();
    if( !next )
        return 0;
    const double postureTime = (time_.*time)( *next ) * modifier_;
    return accumulator( results_.postureCompletionPercentage_, postureTime );
}
