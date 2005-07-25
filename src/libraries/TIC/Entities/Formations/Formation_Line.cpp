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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/Formation_Line.cpp $
// $Author: Sbo $
// $Modtime: 18/07/05 18:50 $
// $Revision: 4 $
// $Workfile: Formation_Line.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/Formations/Formation_Line.h"
#include "Entities/Pawn.h"
#include "Entities/Platform.h"
#include "Tools/Position.h"

using namespace TIC;

double Formation_Line::rDistanceBetweenPlatforms_ = 20.0; // meters

//-----------------------------------------------------------------------------
// Name: Formation_Line::Formation_Line
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_Line::Formation_Line()
    : Formation_ABC ()
    , rCurrentPos_  ( 0.0 )
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: Formation_Line::~Formation_Line
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_Line::~Formation_Line()
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: Formation_Line::Begin
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Line::Begin( const Pawn& pawn, uint nPlatformNbr )
{
    pPawn_        = &pawn;
    nPlatformNbr_ = nPlatformNbr;
    
    // platforms position's range is [-n/2; n/2]
    rCurrentPos_  = nPlatformNbr_ / 2;
}

//-----------------------------------------------------------------------------
// Name: Formation_Line::Apply
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Line::ApplyTo( Platform& platform )
{
    assert( pPawn_ != 0 );

    // get next point on line orthogonal to direction vector
    geometry::Point2< double > ptTmp = Position::Point2FromPolar( rCurrentPos_ * rDistanceBetweenPlatforms_, pPawn_->GetDirection() - 90 );
    // update platform position and speed
    platform.MoveTo( pPawn_->GetPosition() + ptTmp );

    rCurrentPos_ -= 1.0;
}

//-----------------------------------------------------------------------------
// Name: Formation_Line::End
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Line::End()
{
    pPawn_ = 0;
}

//-----------------------------------------------------------------------------
// Name: Formation_Line::SetDistanceInterPlatform
// Created: SBO 2005-07-18
//-----------------------------------------------------------------------------
void Formation_Line::SetDistanceInterPlatform( double rDistance )
{
    rDistanceBetweenPlatforms_ = rDistance;
}