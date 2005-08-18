// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Position.cpp $
// $Author: Sbo $
// $Modtime: 18/07/05 17:55 $
// $Revision: 15 $
// $Workfile: Position.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Position.h"
#include "PositionManager.h"

using namespace TEST;

PositionManager* Position::pPositionManager_;

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::Position()
    : rX_ ( 0.0 )
    , rY_ ( 0.0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
Position::Position( const Position& position )
    : rX_ ( position.rX_ )
    , rY_ ( position.rY_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position::operator=
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
Position& Position::operator=( const Position& position )
{
    rX_ = position.rX_;
    rY_ = position.rY_;
    return *this;
}

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::Position( const std::string& strMgrs )
    : rX_ ( 0.0 )
    , rY_ ( 0.0 )
{
    Position& pos = pPositionManager_->PositionFromMgrs( strMgrs );
    rX_ = pos.rX_;
    rY_ = pos.rY_;
    delete &pos;
}

//-----------------------------------------------------------------------------
// Name: Position::Position
// Created: SBO 2005-06-29
//-----------------------------------------------------------------------------
Position::Position( double rLatitude, double rLongitude )
    : rX_ ( 0.0 )
    , rY_ ( 0.0 )
{
    Position& pos = pPositionManager_->PositionFromWGS( rLatitude, rLongitude );
    rX_ = pos.rX_;
    rY_ = pos.rY_;
    delete &pos;
}

//-----------------------------------------------------------------------------
// Name: Position::~Position
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
Position::~Position()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position::SetSimCoordinates
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
void Position::SetSimCoordinates( double rX, double rY )
{
    rX_ = rX;
    rY_ = rY;
}
