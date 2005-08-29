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

namespace TIC
{

//-----------------------------------------------------------------------------
// Name: Position::RadianFromDegree
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
inline
double  Position::RadianFromDegree( double rDegree )
{
    return PI / 180.0 * rDegree;
}

//-----------------------------------------------------------------------------
// Name: Position::RadianFromDegree
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
inline
double  Position::DegreeFromRadian( double rRadian )
{
    return rRadian * 180.0 / PI;
}

//-----------------------------------------------------------------------------
// Name: Position::RadianFromDegree
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
inline
double Position::GetDistanceTo( const Position& position ) const
{
    return sqrt( ( position.X() - X() ) * ( position.X() - X() ) + ( position.Y() - Y() ) * ( position.Y() - Y() ) );
}

// -----------------------------------------------------------------------------
// Name: Position::GetSimX
// Created: SBO 2005-08-29
// -----------------------------------------------------------------------------
inline
double Position::GetSimX() const
{
    return X();
}
    
// -----------------------------------------------------------------------------
// Name: Position::GetSimY
// Created: SBO 2005-08-29
// -----------------------------------------------------------------------------
inline
double Position::GetSimY() const
{
    return Y();
}

} // end namespace TIC