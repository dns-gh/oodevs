// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Platform.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:10 $
// $Revision: 23 $
// $Workfile: Platform.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/Platform.h"
#include "Entities/Pawn.h"
#include "Entities/Types/PawnType.h"

using namespace TIC;

uint Platform::nNextId_ = 0;

const double Platform::rMaxDistanceForSmoothMove_ = 10000.0;

//-----------------------------------------------------------------------------
// Name: Platform::Platform
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Platform::Platform( const PawnPlatform& platformType, const Pawn& pawn )
    : ConcreteEntity    ()
    , pType_            ( &platformType )
    , nId_              ( ++ nNextId_ )
    , bConfigDisaggreg_ ( platformType.GetType().CanBeDisaggregated() )
    , pPawn_            ( &pawn )
{
}

//-----------------------------------------------------------------------------
// Name: Platform::~Platform
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Platform::~Platform()
{
}

//-----------------------------------------------------------------------------
// Name: Platform::MustBeDisaggregated
// Created: SBO 2005-06-30
//-----------------------------------------------------------------------------
bool Platform::MustBeDisaggregated() const
{
    assert( pPawn_ );
    return bConfigDisaggreg_ && !( IsEmbarquable() && pPawn_->IsLoaded() );
}

//-----------------------------------------------------------------------------
// Name: Platform::Follow
// Created: SBO 2005-06-16
//-----------------------------------------------------------------------------
void Platform::Follow( const Pawn& pawn )
{
    position_ = pawn.GetPosition();
    rSpeed_   = pPawn_->GetSpeed();
}

//-----------------------------------------------------------------------------
// Name: Platform::Follow
// Created: SBO 2005-06-16
//-----------------------------------------------------------------------------
void Platform::Follow( const Platform& platform )
{
    position_ = platform.GetPosition();
    rSpeed_   = pPawn_->GetSpeed();
}

//-----------------------------------------------------------------------------
// Name: Platform::MoveTo
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Platform::MoveTo( const Position& position )
{
    double rDistanceToDestination = position_.GetDistanceTo( position );
    double rNeededSpeed           = rDistanceToDestination / TicManager::GetTicManager().GetTickDuration();

    // nothing to be done
    if( rNeededSpeed == 0 )
    {
        rSpeed_ = 0.;
        return;
    }

    // instant move
    if( rDistanceToDestination > rMaxDistanceForSmoothMove_ )
    {
        position_ = position;
        rSpeed_   = pPawn_->GetSpeed();
    }
    // smooth move
    else
    {
        // check that acceleration isn't more than 1m/s²
        double rAccel = ( rNeededSpeed - rSpeed_ ) / ( double )TicManager::GetTicManager().GetTickDuration();
        if( rAccel > 1 )
            rAccel = 1;
        rSpeed_ += ( T_Speed )( rAccel * ( double )TicManager::GetTicManager().GetTickDuration() );
        
        // if speed is 0, don't move
        if( rSpeed_ == 0 )
            return;

        // check that speed isn't > max : speed limiter
        if( rSpeed_ > pType_->GetType().GetMaxSpeed() )
            rSpeed_ = pType_->GetType().GetMaxSpeed();

        double rRatio = rNeededSpeed / rSpeed_;
        position_.Set( ( position.X() - position_.X() ) / rRatio + position_.X()
                     , ( position.Y() - position_.Y() ) / rRatio + position_.Y() );
        assert( !_isnan( position_.X() ) );
    }
}