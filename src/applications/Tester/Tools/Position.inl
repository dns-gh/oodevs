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

namespace TEST
{

//-----------------------------------------------------------------------------
// Name: Position::GetMgrsCoordinate
// Created: SBO 2005-05-24
//-----------------------------------------------------------------------------
inline
const  std::string& Position::GetMgrsCoordinate() const
{
    return strMgrs_;
}

//-----------------------------------------------------------------------------
// Name: Position::GetLatitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
inline
double Position::GetLatitude() const
{
    return DegreeFromRadian( rLatitude_ );
}

//-----------------------------------------------------------------------------
// Name: Position::GetLongitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
inline
double Position::GetLongitude() const
{
    return DegreeFromRadian( rLongitude_ );
}

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

} // end namespace TEST
