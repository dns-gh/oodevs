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
#include "simulation_kernel/Entities/Agents/Units/PHY_UnitType.h"

using namespace posture;

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::DefaultPostureComputer
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
DefaultPostureComputer::DefaultPostureComputer( Parameters& params )
    : params_( &params )
    , bForceMovement_( false )
    , bForceStop_( false )
    , bIsLoaded_( false )
    , results_( params.rCompletionPercentage_ )
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
    if( &( params_->posture_ ) == &PHY_Posture::poste_ )
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

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::Update
// Created: MGD 2009-09-22
// -----------------------------------------------------------------------------
void DefaultPostureComputer::Update()
{
    if( params_->bIsDead_ )
    {
        results_.newPosture_ = &PHY_Posture::arret_;
        results_.postureCompletionPercentage_ = 1.;
        results_.bIsStealth_ = false;
        return;
    }

    if( bForceMovement_ )
    {
        if( !bIsLoaded_ )
            results_.newPosture_ = &PHY_Posture::posteReflexe_;
        else if( params_->bDiscreteModeEnabled_ )
            results_.newPosture_ = &PHY_Posture::mouvementDiscret_;
        else
            results_.newPosture_ = &PHY_Posture::mouvement_;
        return;
    }

    if( bForceStop_ && ( &params_->posture_ == &PHY_Posture::mouvement_
                      || &params_->posture_ == &PHY_Posture::mouvementDiscret_ ) )
            results_.newPosture_ = &PHY_Posture::arret_;

    // Mode furtif
    results_.bIsStealth_ = !( MIL_Random::rand_oi( 0., 1., MIL_Random::ePerception ) <= params_->rStealthFactor_ );

    if( results_.postureCompletionPercentage_ == 1. )
    {
        const PHY_Posture* pNextAutoPosture = params_->posture_.GetNextAutoPosture();
        if( !pNextAutoPosture )
            return;
        results_.newPosture_ = pNextAutoPosture;
    }
    else
    {
        const double rPostureTime = GetPostureTime();
        double rNewPostureCompletionPercentage = results_.postureCompletionPercentage_;
        if( rPostureTime )
        {
            rNewPostureCompletionPercentage += ( 1. / rPostureTime );
            if( rNewPostureCompletionPercentage > 1. )
                rNewPostureCompletionPercentage = 1.;
        }
        else
            rNewPostureCompletionPercentage = 1.;

        results_.postureCompletionPercentage_ = rNewPostureCompletionPercentage;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultPostureComputer::GetPostureTime
// Created: MGD 2009-09-22
// -----------------------------------------------------------------------------
double DefaultPostureComputer::GetPostureTime() const
{
    assert( params_->rTimingFactor_ > 0. );

    double postureTime = params_->unitType_.GetPostureTime( params_->posture_ );
    for( std::vector< double >::const_iterator it = coefficientsModifier_.begin(); it != coefficientsModifier_.end(); it++ )
    {
        postureTime *= *it;
    }
    return postureTime / params_->rTimingFactor_;
}