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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Path.cpp $
// $Author: Sbo $
// $Modtime: 19/07/05 11:45 $
// $Revision: 3 $
// $Workfile: Path.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Types.h"
#include "Tools/Path.h"
#include "Tools/Position.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: Path::Path
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
Path::Path()
    : path_ ()
{
}

//-----------------------------------------------------------------------------
// Name: Path::~Path
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
Path::~Path()
{
    Clear();
}

//-----------------------------------------------------------------------------
// Name: Path::operator=
// Created: SBO 2005-07-19
//-----------------------------------------------------------------------------
Path& Path::operator=( const Path& path )
{
    Clear();
    for( CIT_PositionVector it = path.path_.begin(); it != path.path_.end(); ++it )
    {
        Position& pos = *new Position( ( *it )->GetMgrsCoordinate() );
        AddNode( pos );
    }
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Path::GetTestParam_Path
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
Path& Path::GetTestParam_Path( const Position& startPoint )
{
    Path& path = *new Path();

    double rX = startPoint.X();
    double rY = startPoint.Y();

    for( uint i = 0; i < 5; ++i )
    {
        rX += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
        rY += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
        Position& node = *new Position();
        node.SetSimCoordinates( rX, rY );
        path.AddNode( node );
    }
    return path;
}

// -----------------------------------------------------------------------------
// Name: Path::GetTestParam_PathList
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
T_PathVector& Path::GetTestParam_PathList( const Position& startPoint )
{
    T_PathVector& paths = *new T_PathVector();

    // generate 5 pathfinds
    for( uint i = 0; i < 4; ++i )
        paths.push_back( &GetTestParam_Path( startPoint ) );
    return paths;
}
