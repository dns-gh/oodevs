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

#include "simulation_tester_pch.h"
#include "Location.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Location constructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
Location::Location()
    : eType_  ( EnumLocationType::point )
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
Location& Location::GetTestParam_Location( const Position& pos, ASN1T_EnumLocationType eType /* = ( ASN1T_EnumLocationType )-1 */ )
{
    Location& loc = *new Location();
    // 6 types of Location
    if( eType == ( ASN1T_EnumLocationType )-1 )
    {
        uint nTypeLoc = ( uint )( rand() * 5.0 / RAND_MAX );
        loc.eType_ = ( ASN1T_EnumLocationType )nTypeLoc;
    }
    else
        loc.eType_ = eType;        

    Position* pNode = new Position( pos.GetMgrsCoordinate() );
    // always same first point
    loc.points_.push_back( pNode );

    Position* pNodeTmp;

    switch( loc.eType_ )
    {
    case EnumLocationType::point:
        break;
    case EnumLocationType::circle:
        // 5km radius point
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX() + 5000, pNode->GetSimY() );
        loc.points_.push_back( pNodeTmp );
        break;
    case EnumLocationType::ellipse:
        // 2km small axis
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX(), pNode->GetSimY() + 2000 );
        loc.points_.push_back( pNodeTmp );
        // 5km big axis
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX() + 5000, pNode->GetSimY() );
        loc.points_.push_back( pNodeTmp );
        break;
    case EnumLocationType::line:
        // line second point 5km to the right
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX() + 5000, pNode->GetSimY() );
        loc.points_.push_back( pNodeTmp );
        break;
    case EnumLocationType::polygon:
        // triangle
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX() - 3000, pNode->GetSimY() + 2000 );
        loc.points_.push_back( pNodeTmp );
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX() + 3000, pNode->GetSimY() + 3000 );
        loc.points_.push_back( pNodeTmp );
        break;
    case EnumLocationType::sector:
        // 90° sector around North ( 315° -> 45° )
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX() - 3000, pNode->GetSimY() + 3000 );
        loc.points_.push_back( pNodeTmp );
        pNodeTmp = new Position();
        pNodeTmp->SetSimCoordinates( pNode->GetSimX() + 3000, pNode->GetSimY() + 3000 );
        loc.points_.push_back( pNodeTmp );
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
                                                       ASN1T_EnumLocationType eType /*= ( ASN1T_EnumLocationType )-1*/ )
{
    T_LocationVector& locations = *new T_LocationVector();
    Position pt;

    double rX = pos.GetSimX();
    double rY = pos.GetSimY();

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
