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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/Formation_Diamond.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:24 $
// $Revision: 10 $
// $Workfile: Formation_Diamond.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/Formations/Formation_Diamond.h"
#include "Entities/Pawn.h"
#include "Entities/Platform.h"
#include "Tools/Position.h"

using namespace TIC;

double Formation_Diamond::rDistanceBetweenPlatforms_ = 20.0; // meters

//-----------------------------------------------------------------------------
// Name: Formation_Diamond::Formation_Diamond
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_Diamond::Formation_Diamond()
    : Formation_ABC         ()
    , rAngle_               ( 0. )
    , rRadius_              ( 0. )
    , nPlatformPositioned_  ( 0 )
{
    srand( time( 0 ) );
}

//-----------------------------------------------------------------------------
// Name: Formation_Diamond::~Formation_Diamond
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_Diamond::~Formation_Diamond()
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: Formation_Diamond::Begin
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Diamond::Begin( const Pawn& pawn, uint nPlatformNbr )
{
    pPawn_        = &pawn;
    nPlatformNbr_ = nPlatformNbr;
    
    rAngle_              = 0;
    rRadius_             = 0.0;
    nPlatformPositioned_ = 0;
    nPlatformPositionedOnLevel_ = 0;
    nPlatformPerLevel_   = 1;
}

//-----------------------------------------------------------------------------
// Name: Formation_Diamond::ApplyTo
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Diamond::ApplyTo( Platform& platform )
{
    assert( pPawn_ != 0 );

    // first platform is always at pawn's position
    if( nPlatformPositioned_ == 0 )
    {
        platform.MoveTo( pPawn_->GetPosition() );
        rRadius_ = 1.;
        rAngle_  = 0.;
        nPlatformPerLevel_ = 4;
    }
    else
    {
        geometry::Point2< double > ptTmp;
        // add some chaos so that diamond formation is not too much obviously "square"
        //double rChaos = ( ( double )rand() / RAND_MAX - 0.5 ) * 40.0; // +/- 20m
        double rChaos = 0;
        for( double rDirection = pPawn_->GetDirection() + rAngle_; rDirection >= 360.0; rDirection -= 360.0 )
            ;
        ptTmp = Position::Point2FromPolar( rRadius_ * rDistanceBetweenPlatforms_ + rChaos, rDirection );
        platform.MoveTo( pPawn_->GetPosition() + ptTmp );

        nPlatformPositionedOnLevel_++;

        // if 4 platform where positioned, start a new square containing previous
        if( nPlatformPositionedOnLevel_ == nPlatformPerLevel_ )
        {
            rRadius_ *= 1.414; // sqrt(2)
            rAngle_  = 0.;
            nPlatformPerLevel_ += 4;
            nPlatformPositionedOnLevel_ = 0;
        }
        else
            rAngle_ = rAngle_ + 360.0 / nPlatformPerLevel_;
    }
    nPlatformPositioned_++;
}

////-----------------------------------------------------------------------------
//// Name: Formation_Diamond::ApplyTo
//// Created: SBO 2005-06-20
////-----------------------------------------------------------------------------
//void Formation_Diamond::ApplyTo( Platform& platform )
//{
//    assert( pPawn_ != 0 );
//
//    // first platform is always at pawn's position
//    if( nPlatformPositioned_ == 0 )
//    {
//        platform.MoveTo( pPawn_->GetPosition() );
//        rRadius_ = 1;
//        nAngle_  = 45;
//    }
//    else
//    {
//        geometry::Point2< double > ptTmp;
//        // add some chaos so that diamond formation is not too much obviously "square"
//        //double rChaos = ( ( double )rand() / RAND_MAX - 0.5 ) * 40.0; // +/- 20m
//        double rChaos = 0;
//        ptTmp = Position::Point2FromPolar( rRadius_ * rDistanceBetweenPlatforms_ + rChaos, pPawn_->GetDirection() + nAngle_ );
//        platform.MoveTo( pPawn_->GetPosition() + ptTmp );
//
//        // if 4 platform where positioned, start a new square containing previous
//        if( nPlatformPositioned_ % 4 == 0 )
//        {
//            rRadius_ *= 1.414; // sqrt(2)
//            nAngle_  = ( nPlatformPositioned_ % 8 == 0 ) ? 45 : 0; // start angle alternates between 45° and 0°
//        }
//        else
//            nAngle_ = ( nAngle_ + 90 ) % 360;
//    }
//    nPlatformPositioned_++;
//}

//-----------------------------------------------------------------------------
// Name: Formation_Diamond::End
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Diamond::End()
{
    pPawn_ = 0;
    nPlatformPositioned_ = 0;
}

//-----------------------------------------------------------------------------
// Name: Formation_Diamond::SetDistanceInterPlatform
// Created: SBO 2005-07-18
//-----------------------------------------------------------------------------
void Formation_Diamond::SetDistanceInterPlatform( double rDistance )
{
    rDistanceBetweenPlatforms_ = rDistance;
}