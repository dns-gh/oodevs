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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Position.inl $
// $Author: Sbo $
// $Modtime: 18/07/05 17:42 $
// $Revision: 9 $
// $Workfile: Position.inl $
//
// *****************************************************************************

#include "PositionManager.h"

namespace TEST
{

// -----------------------------------------------------------------------------
// Name: Position::SetPositionManager
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
void Position::SetPositionManager( PositionManager& positionManager )
{
    pPositionManager_ = &positionManager;
}

//-----------------------------------------------------------------------------
// Name: Position::GetMgrsCoordinate
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
std::string Position::GetMgrsCoordinate() const
{
    return pPositionManager_->MgrsFromPosition( *this );
}

// -----------------------------------------------------------------------------
// Name: Position::GetSimX
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
double Position::GetSimX() const
{
    return rX_;
}

// -----------------------------------------------------------------------------
// Name: Position::GetSimY
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
inline
double Position::GetSimY() const
{
    return rY_;
}

} // end namespace TEST
