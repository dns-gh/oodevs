// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DefaultPostureComputer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Units/Postures/PostureTime_ABC.h"
#include "MIL_Random_ABC.h"
#include "MIL_Random.h"

using namespace posture;

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::DefaultPostureComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultPostureComputer::DefaultPostureComputer( const MIL_Random_ABC& random, const PostureTime_ABC& time, const PHY_Posture& previousPosture,
                                                const PHY_Posture& newPosture, bool bIsDead,
                                                bool bDiscreteModeEnabled, double rCompletionPercentage, double rStealthFactor,
                                                double rTimingFactor, bool isParkedOnEngineerArea )
    : random_                ( random )
    , time_                  ( time )
    , previousPosture_       ( previousPosture )
    , currentPosture_        ( &newPosture )
    , results_               ( rCompletionPercentage )
    , rStealthFactor_        ( rStealthFactor )
    , modifier_              ( 1 / rTimingFactor )
    , bIsDead_               ( bIsDead )
    , bDiscreteModeEnabled_  ( bDiscreteModeEnabled )
    , isParkedOnEngineerArea_( isParkedOnEngineerArea )
    , bMoving_               ( false )
    , bStopped_              ( false )
    , bIsLoaded_             ( false )
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

namespace
{
    template< typename Container >
    struct ReinforcementVisitor : posture::PostureComputer_ABC
    {
        ReinforcementVisitor( MIL_Agent_ABC& pion, Container& container )
            : container_( container )
            , result_   ( 1 )
        {
            pion.Execute< posture::PostureComputer_ABC >( *this );
            container.push_back( std::make_pair( &pion.GetType().GetUnitType(), result_ ) );
        }
    private:
        virtual void ApplyOnReinforcement( MIL_Agent_ABC& pion )
        {
            ReinforcementVisitor( pion, container_ );
        }
        virtual void AddCoefficientModifier( double coef )
        {
            result_ *= coef;
        }
        virtual void SetPostureMovement()
        {}
        virtual void UnsetPostureMovement()
        {}
        virtual void NotifyLoaded()
        {}
        virtual const Results& Result()
        {
            throw __FUNCTION__;
        }
        Container& container_;
        double result_;
    };
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::ApplyOnReinforcement
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
void DefaultPostureComputer::ApplyOnReinforcement( MIL_Agent_ABC& pion )
{
    ReinforcementVisitor< T_Reinforcements >( pion, reinforcements_ );
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
        results.postureCompletionPercentage_ = 1;
        results.bIsStealth_ = false;
    }
    void ComputeStealthMode( PostureComputer_ABC::Results& results, const MIL_Random_ABC& random, double stealthFactor )
    {
        results.bIsStealth_ = random.rand_oi( 0, 1, MIL_Random::ePerception ) > stealthFactor;
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
    const PHY_Posture* nextPosture = results_.newPosture_;
    double previousPercentage = results_.postureCompletionPercentage_;
    double timeSpent = 0;
    do
    {
        nextPosture = results_.newPosture_;
        previousPercentage = results_.postureCompletionPercentage_;
        if( bMoving_ )
            timeSpent += ComputeMovingPosture();
        else
            timeSpent += ComputeStopPosture();
        if( results_.postureCompletionPercentage_ == 1 )
            currentPosture_ = results_.newPosture_;
    }
    while( timeSpent < 1 && ( nextPosture != results_.newPosture_ || results_.postureCompletionPercentage_ != previousPercentage ) );
    return results_;
}

namespace
{
    const PHY_Posture* ComputePreviousPosture( const PHY_Posture& currentPosture_, bool bIsLoaded_, bool bDiscreteModeEnabled_ )
    {
        if( currentPosture_.GetPreviousAutoPosture() )
            return currentPosture_.GetPreviousAutoPosture();
        if( ! bIsLoaded_ )
            return &PHY_Posture::posteReflexe_;
        else if( bDiscreteModeEnabled_ )
            return &PHY_Posture::mouvementDiscret_;
        else
            return &PHY_Posture::mouvement_;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::ComputeMovingPosture
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
double DefaultPostureComputer::ComputeMovingPosture()
{
    if( !results_.newPosture_ && previousPosture_ < *currentPosture_ )
    {
        results_.newPosture_ = ComputePreviousPosture( *currentPosture_, bIsLoaded_, bDiscreteModeEnabled_ );
        results_.postureCompletionPercentage_ = 1 - results_.postureCompletionPercentage_;
    }
    else if( results_.postureCompletionPercentage_ == 1 || *currentPosture_ == previousPosture_ )
    {
        if( bIsLoaded_ || results_.newPosture_ != &PHY_Posture::posteReflexe_ )
            results_.newPosture_ = results_.newPosture_ ? results_.newPosture_->GetPreviousAutoPosture() : currentPosture_->GetPreviousAutoPosture();
        if( !results_.newPosture_  )
        {
            results_.newPosture_ = ComputePreviousPosture( *currentPosture_, bIsLoaded_, bDiscreteModeEnabled_ );
            results_.postureCompletionPercentage_ = 1;
            return 0;
        }
        results_.postureCompletionPercentage_ = 0;
    }
    else
        results_.newPosture_ = currentPosture_;
    double timeSpent = 0;
    results_.postureCompletionPercentage_ = ComputeCompletion( &PostureTime_ABC::GetPostureTearDownTime, timeSpent );
    return timeSpent;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::ComputeStopPosture
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
double DefaultPostureComputer::ComputeStopPosture()
{
    if( bStopped_ && ( currentPosture_ == &PHY_Posture::mouvement_
                    || currentPosture_ == &PHY_Posture::mouvementDiscret_ ) )
    {
        results_.newPosture_ = &PHY_Posture::arret_;
        results_.postureCompletionPercentage_ = 1;
        return 1;
    }
    if( !results_.newPosture_ && *currentPosture_ < previousPosture_ )
    {
        results_.newPosture_ = &previousPosture_;
        results_.postureCompletionPercentage_ = 1 - results_.postureCompletionPercentage_;
    }
    if( !results_.newPosture_ )
        results_.newPosture_ = currentPosture_;
    if( results_.postureCompletionPercentage_ == 1 || *currentPosture_ == previousPosture_ )
    {
        if( !results_.newPosture_->GetNextAutoPosture() )
            return 0;
        results_.newPosture_ = results_.newPosture_->GetNextAutoPosture();
        results_.postureCompletionPercentage_ = 0;
    }
    if( isParkedOnEngineerArea_ )
        results_.newPosture_ = &PHY_Posture::postePrepareGenie_;
    double timeSpent = 0;
    results_.postureCompletionPercentage_ = ComputeCompletion( &PostureTime_ABC::GetPostureSetupTime, timeSpent );
    return timeSpent;
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::ComputeCompletion
// Created: MCO 2013-06-12
// -----------------------------------------------------------------------------
template< typename GetTime >
double DefaultPostureComputer::ComputeCompletion( const GetTime& time, double& timeSpent ) const
{
    double postureTime = (time_.*time)( *results_.newPosture_ ) * modifier_;
    for( auto it = reinforcements_.begin(); it != reinforcements_.end(); ++it )
        postureTime = std::max( postureTime, (it->first->*time)( *results_.newPosture_ ) * it->second );
    if( postureTime == 0 )
    {
        timeSpent = 0;
        return 1;
    }
    double result = results_.postureCompletionPercentage_ + ( 1 / postureTime );
    if( result > 1 )
    {
        timeSpent = ( 1 - results_.postureCompletionPercentage_ ) * postureTime;
        return 1;
    }
    timeSpent = 1;
    return result;
}
