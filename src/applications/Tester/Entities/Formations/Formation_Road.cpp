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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Formations/Formation_Road.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:10 $
// $Revision: 7 $
// $Workfile: Formation_Road.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/Formations/Formation_Road.h"
#include "Entities/Pawn.h"
#include "Entities/Platform.h"
#include "Tools/Position.h"

using namespace TIC;

double Formation_Road::rDistanceBetweenPlatforms_ = 20.0; // meters

//-----------------------------------------------------------------------------
// Name: Formation_Road::Formation_Road
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_Road::Formation_Road()
    : Formation_ABC        ()
    , pLastPlatform_       ( 0 )
    , nPlatformPositioned_ ( 0 )
{
    bIsReverse_ = true;
}

//-----------------------------------------------------------------------------
// Name: Formation_Road::~Formation_Road
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
Formation_Road::~Formation_Road()
{
    // nothing
}

//-----------------------------------------------------------------------------
// Name: Formation_Road::Begin
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Road::Begin( const Pawn& pawn, uint nPlatformNbr )
{
    pPawn_               = &pawn;
    nPlatformNbr_        = nPlatformNbr;
    pLastPlatform_       = 0;
    nPlatformPositioned_ = 0;
}

//-----------------------------------------------------------------------------
// Name: Formation_Road::ApplyTo
// Created: SBO 2005-06-20
// Complex Road formation => follow pawn's pathfind
//-----------------------------------------------------------------------------
void Formation_Road::ApplyTo( Platform& platform )
{
    assert( pPawn_ != 0 );

    try
    {
        Path::const_reverse_iterator it( pPawn_->GetPath(), pPawn_->GetPosition() );
        it = it + ( nPlatformPositioned_ * rDistanceBetweenPlatforms_ );
        assert( !_isnan( ( *it ).X() ) );
        platform.MoveTo( *it );
    }
    catch( std::exception& /*e*/ )
    {
        // unable to perform move on path, don't move
        // it happens at the start of every path and until every platform can be placed on the path
        //std::cout << "cannot compute path, out of bound..." << std::endl;
    }
    nPlatformPositioned_++;
}

////-----------------------------------------------------------------------------
//// Name: Formation_Road::ApplyTo
//// Created: SBO 2005-06-20
//// Simple Road formation => follow preceding platform/pawn
////-----------------------------------------------------------------------------
//void Formation_Road::ApplyTo( Platform& platform )
//{
//    assert( pPawn_ != 0 );
//
//    if( nPlatformPositioned_ > 0 )
//    {
//        pLastPlatform_->Follow( platform );
//        if( nPlatformPositioned_ == nPlatformNbr_ - 1 )
//            platform.Follow( *pPawn_ );
//    }
//    nPlatformPositioned_++;
//    pLastPlatform_ = &platform;
//}

//-----------------------------------------------------------------------------
// Name: Formation_Road::End
// Created: SBO 2005-06-20
//-----------------------------------------------------------------------------
void Formation_Road::End()
{
    pPawn_ = 0;
}

//-----------------------------------------------------------------------------
// Name: Formation_Road::SetDistanceInterPlatform
// Created: SBO 2005-07-18
//-----------------------------------------------------------------------------
void Formation_Road::SetDistanceInterPlatform( double rDistance )
{
    rDistanceBetweenPlatforms_ = rDistance;
}