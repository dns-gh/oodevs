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
        Position* pPos = new Position( ( *it )->GetMgrsCoordinate() );
        AddNode( *pPos );
    }
    return *this;
}
