// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-10 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Location.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
Location::Location()
    : eType_  ( EnumTypeLocalisation::point )
    , points_ ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Location destructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
Location::~Location()
{
    for( CIT_PositionVector it = points_.begin(); it != points_.end(); ++it )
        delete *it;
    points_.clear();
}

// -----------------------------------------------------------------------------
// Name: Location::GetTestParam_Location
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
Location& Location::GetTestParam_Location( const Position& pos, ASN1T_EnumTypeLocalisation eType /* = ( ASN1T_EnumTypeLocalisation )-1 */ )
{
    Location& loc = *new Location();
    // 6 types of Location
    if( eType == ( ASN1T_EnumTypeLocalisation )-1 )
    {
        uint nTypeLoc = ( uint )( rand() * 5.0 / RAND_MAX );
        loc.eType_ = ( ASN1T_EnumTypeLocalisation )nTypeLoc;
    }
    else
        loc.eType_ = eType;        

    Position* pNode = new Position( pos.GetMgrsCoordinate() );
    // always same first point
    loc.points_.push_back( pNode );

    switch( loc.eType_ )
    {
    case EnumTypeLocalisation::point:
        break;
    case EnumTypeLocalisation::circle:
        // 5km radius point
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X() + 5000, pNode->Y() );
        loc.points_.push_back( pNode );
        break;
    case EnumTypeLocalisation::ellipse:
        // 2km small axis
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X(), pNode->Y() + 2000 );
        loc.points_.push_back( pNode );
        // 5km big axis
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X() + 5000, pNode->Y() );
        loc.points_.push_back( pNode );
        break;
    case EnumTypeLocalisation::line:
        // line second point 5km to the right
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X() + 5000, pNode->Y() );
        loc.points_.push_back( pNode );
        break;
    case EnumTypeLocalisation::polygon:
        // triangle
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X() + 5000, pNode->Y() + 2000 );
        loc.points_.push_back( pNode );
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X() + 6000, pNode->Y() + 3000 );
        loc.points_.push_back( pNode );
        break;
    case EnumTypeLocalisation::sector:
        // 90° sector around North ( 315° -> 45° )
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X() - 3000, pNode->Y() + 3000 );
        loc.points_.push_back( pNode );
        pNode = new Position();
        pNode->SetSimCoordinates( pNode->X() + 3000, pNode->Y() + 3000 );
        loc.points_.push_back( pNode );
        break;
    default:
        assert( false );
    }
    return loc;
}


// -----------------------------------------------------------------------------
// Name: Location::GetTestParam_LocationList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_LocationVector& Location::GetTestParam_LocationList( const Position& pos, uint nLocation, 
                                                       ASN1T_EnumTypeLocalisation eType /*= ( ASN1T_EnumTypeLocalisation )-1*/ )
{
    T_LocationVector& locations = *new T_LocationVector();
    Position pt;

    double rX = pos.X();
    double rY = pos.Y();

    for( uint i = 0; i < nLocation - 1; ++i )
    {
        // +/- 10km from previous point on X and Y axis
        rX += 20000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
        rY += 20000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
        pt.SetSimCoordinates( rX, rY );
        Location& loc = GetTestParam_Location( pt, eType );
        locations.push_back( &loc );
    }
    return locations;
}
