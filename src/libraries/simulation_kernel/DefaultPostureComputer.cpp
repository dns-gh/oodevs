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
    , rTimingFactor_         ( rTimingFactor )
    , isParkedOnEngineerArea_( isParkedOnEngineerArea )
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
    void ComputeNextPosture( PostureComputer_ABC::Results& results, const PHY_Posture& current )
    {
        const PHY_Posture* pNextAutoPosture = current.GetNextAutoPosture();
        if( !pNextAutoPosture )
            return;
        results.postureCompletionPercentage_ = 0.;
        results.newPosture_ = pNextAutoPosture;
    }
    void ComputePreviousPosture( PostureComputer_ABC::Results& results, const PHY_Posture& previous )
    {
        results.postureCompletionPercentage_ = 1.;
        results.newPosture_ = &previous;
    }
    void ComputeMovingPosture( PostureComputer_ABC::Results& results, bool isLoaded, bool isDiscreteModeEnabled, double completion, const PHY_Posture& current )
    {
        results.postureCompletionPercentage_ = completion;
        if( completion > 0. )
            return;
        if( current.GetPreviousAutoPosture() )
            return ComputePreviousPosture( results, *current.GetPreviousAutoPosture() );
        else if( !isLoaded )
            results.newPosture_ = &PHY_Posture::posteReflexe_;
        else if( isDiscreteModeEnabled )
            results.newPosture_ = &PHY_Posture::mouvementDiscret_;
        else
        {
            results.newPosture_ = &PHY_Posture::mouvement_;
            results.postureCompletionPercentage_ = 1;
        }
    }
    void ComputeStopPosture( PostureComputer_ABC::Results& results, bool isParkedOnEngineerArea, bool forceStop, double completion, const PHY_Posture& current )
    {
        results.postureCompletionPercentage_ = completion;
        if( forceStop && ( &current == &PHY_Posture::mouvement_
                        || &current == &PHY_Posture::mouvementDiscret_ ) )
        {
            results.newPosture_ = &PHY_Posture::arret_;
            results.postureCompletionPercentage_ = 0.;
            return;
        }
        if( completion < 1. )
            return;
        if( isParkedOnEngineerArea )
        {
            results.newPosture_ = &PHY_Posture::postePrepareGenie_;
            return;
        }
        return ComputeNextPosture( results, current );
    }
    double ApplyModifiers( double time, const std::vector< double >& coefficientsModifier, double timingFactor )
    {
        assert( timingFactor > 0. );
        for( auto it = coefficientsModifier.begin(); it != coefficientsModifier.end(); ++it )
            time *= *it;
        return time / timingFactor;
    }
    double Add( double currentCompletion, double postureTime )
    {
        if( postureTime == 0. )
            return 1.;
        return std::min( 1., currentCompletion + ( 1. / postureTime ) );
    }
    double Remove( double currentCompletion, double postureTime )
    {
        if( postureTime == 0. )
            return 0.;
        return std::max( 0., currentCompletion - ( 1. / postureTime ) );
    }
    template< typename Time, typename Accumulator >
    double ComputeCompletion( double currentCompletion, const PHY_Posture& current, const std::vector< double >& coefficientsModifier, double timingFactor, bool isParkedOnEngineerArea, const Time& time, const Accumulator& accumulator )
    {
        const PHY_Posture* next = isParkedOnEngineerArea ? &PHY_Posture::postePrepareGenie_ : current.GetNextAutoPosture();
        if( !next )
            return 0.;
        const double postureTime = ApplyModifiers( time( *next ), coefficientsModifier, timingFactor );
        return accumulator( currentCompletion, postureTime );
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
    if( bMoving_ )
    {
        const double completion = ComputeCompletion( results_.postureCompletionPercentage_, posture_, coefficientsModifier_, rTimingFactor_, isParkedOnEngineerArea_, boost::bind( &PostureTime_ABC::GetPostureTearDownTime, boost::cref( time_ ), _1 ), boost::bind( &Remove, _1, _2 ) );
        ComputeMovingPosture( results_, bIsLoaded_, bDiscreteModeEnabled_, completion, posture_ );
    }
    else
    {
        const double completion = ComputeCompletion( results_.postureCompletionPercentage_, posture_, coefficientsModifier_, rTimingFactor_, isParkedOnEngineerArea_, boost::bind( &PostureTime_ABC::GetPostureSetupTime, boost::cref( time_ ), _1 ), boost::bind( &Add, _1, _2 ) );
        ComputeStopPosture( results_, isParkedOnEngineerArea_, bStopped_, completion, posture_ );
    }
}
