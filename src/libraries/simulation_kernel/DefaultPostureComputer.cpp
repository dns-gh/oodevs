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
                                                double rTimingFactor )
    : random_               ( random )
    , time_                 ( time )
    , posture_              ( posture )
    , bIsDead_              ( bIsDead )
    , bDiscreteModeEnabled_ ( bDiscreteModeEnabled )
    , rCompletionPercentage_( rCompletionPercentage )
    , rStealthFactor_       ( rStealthFactor )
    , rTimingFactor_        ( rTimingFactor )
    , bForceMovement_       ( false )
    , bForceStop_           ( false )
    , bIsLoaded_            ( false )
    , results_              ( rCompletionPercentage )
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
    bForceMovement_ = true;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::UnsetPostureMovement
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::UnsetPostureMovement()
{
    if( !bForceMovement_ ) // $$$$ LDC Role action flying can force movement with 0 horizontal speed.
        bForceStop_ = true;
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
// Name: DefaultPostureComputer::MustBeForce
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::AddCoefficientModifier( double coef )
{
    coefficientsModifier_.push_back( coef );
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::MustBeForce
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void DefaultPostureComputer::AddUrbanCoefficientModifier( double coef )
{
    if( &( posture_ ) == &PHY_Posture::poste_ )
        coefficientsModifier_.push_back( coef );
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::GetPosture
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
PostureComputer_ABC::Results& DefaultPostureComputer::Result()
{
    Update();
    return results_;
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
    void ComputeMovingPosture( PostureComputer_ABC::Results& results, bool isLoaded, bool isDiscreteModeEnabled )
    {
        if( !isLoaded )
            results.newPosture_ = &PHY_Posture::posteReflexe_;
        else if( isDiscreteModeEnabled )
            results.newPosture_ = &PHY_Posture::mouvementDiscret_;
        else
            results.newPosture_ = &PHY_Posture::mouvement_;
    }
    void ComputeStopPosture( PostureComputer_ABC::Results& results, bool forceStop, const PHY_Posture& current )
    {
        if( forceStop && ( &current == &PHY_Posture::mouvement_
                        || &current == &PHY_Posture::mouvementDiscret_ ) )
            results.newPosture_ = &PHY_Posture::arret_;
    }
    void ComputeNextPosture( PostureComputer_ABC::Results& results, const PHY_Posture& current )
    {
        const PHY_Posture* pNextAutoPosture = current.GetNextAutoPosture();
        if( !pNextAutoPosture )
            return;
        results.newPosture_ = pNextAutoPosture;
    }
    double ComputeCompletion( double currentCompletion, double postureTime )
    {
        if( postureTime == 0. )
            return 1.;
        return std::min( 1., currentCompletion + ( 1. / postureTime ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::Update
// Created: MGD 2009-09-22
// -----------------------------------------------------------------------------
void DefaultPostureComputer::Update()
{
    if( bIsDead_ )
        return ComputeDeathPosture( results_ );
    ComputeStealthMode( results_, random_, rStealthFactor_ );
    if( bForceMovement_ )
        return ComputeMovingPosture( results_, bIsLoaded_, bDiscreteModeEnabled_ );
    ComputeStopPosture( results_, bForceStop_, posture_ );
    if( results_.postureCompletionPercentage_ == 1. )
        return ComputeNextPosture( results_, posture_ );
    results_.postureCompletionPercentage_ = ComputeCompletion( results_.postureCompletionPercentage_, GetPostureTime() );
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::GetPostureTime
// Created: MGD 2009-09-22
// -----------------------------------------------------------------------------
double DefaultPostureComputer::GetPostureTime() const
{
    assert( rTimingFactor_ > 0. );
    double postureTime = time_.GetPostureTime( posture_ );
    for( auto it = coefficientsModifier_.begin(); it != coefficientsModifier_.end(); ++it )
        postureTime *= *it;
    return postureTime / rTimingFactor_;
}
