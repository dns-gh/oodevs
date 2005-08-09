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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Path.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:08 $
// $Revision: 2 $
// $Workfile: Path.inl $
//
// *****************************************************************************

#include "Tools/Position.h"

namespace TEST
{

//-----------------------------------------------------------------------------
// Name: Path::AddNode
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
inline
void Path::AddNode( Position& position )
{
    path_.push_back( &position );
}

//-----------------------------------------------------------------------------
// Name: Path::Clear
// Created: SBO 2005-07-06
//-----------------------------------------------------------------------------
inline
void Path::Clear()
{
    for( CIT_PositionVector it = path_.begin(); it != path_.end(); ++it )
        delete *it;
    path_.clear();
}

// -----------------------------------------------------------------------------
// Name: Path::GetPath
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
inline
const T_PositionVector& Path::GetPath() const
{
    return path_;
}

} // end namespace TEST
