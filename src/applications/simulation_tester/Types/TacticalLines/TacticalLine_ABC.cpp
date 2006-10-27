// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "TacticalLine_ABC.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC()
    : nId_            ( 0 )
    , points_         ()
    , bIsSyncWithSim_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_ABC::TacticalLine_ABC( const T_PositionVector& points )
    : nId_            ( 0 )
    , points_         ()
    , bIsSyncWithSim_ ( false )
{
    for( CIT_PositionVector it = points.begin(); it != points.end(); ++it )
        points_.push_back( new Position( **it ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_ABC::~TacticalLine_ABC()
{
    for( CIT_PositionVector it = points_.begin(); it != points_.end(); ++it )
        delete *it;
    points_.clear();
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::ReadPoints
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void TacticalLine_ABC::ReadPoints( XmlInputArchive& archive )
{
    double rX, rY;
    while( archive.NextListElement() )
    {
        archive.Section( "point" );
        archive.ReadAttribute( "x", rX );
        archive.ReadAttribute( "y", rY );
        Position& pos = *new Position();
        pos.SetSimCoordinates( rX, rY );
        points_.push_back( &pos );
        archive.EndSection();
    }
}
