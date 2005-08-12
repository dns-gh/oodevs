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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/PositionManager.inl $
// $Author: Sbo $
// $Modtime: 23/05/05 16:56 $
// $Revision: 1 $
// $Workfile: PositionManager.inl $
//
// *****************************************************************************

namespace TEST {

// -----------------------------------------------------------------------------
// Name: PositionManager::GetWorldWidth
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
double PositionManager::GetWorldWidth() const
{
    return rWorldWidth_;
}

// -----------------------------------------------------------------------------
// Name: PositionManager::GetWorldHeight
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
inline
double PositionManager::GetWorldHeight() const
{
    return rWorldHeight_;
}

//-----------------------------------------------------------------------------
// Name: PositionManager::RadianFromDegree
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
inline
double  PositionManager::RadianFromDegree( double rDegree )
{
    return PI / 180.0 * rDegree;
}

//-----------------------------------------------------------------------------
// Name: PositionManager::RadianFromDegree
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
inline
double  PositionManager::DegreeFromRadian( double rRadian )
{
    return rRadian * 180.0 / PI;
}

} // end namespace TEST