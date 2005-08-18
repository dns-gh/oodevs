// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-09 $
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
#include "TacticalLineManager.h"
#include "TacticalLines/TacticalLine_ABC.h"
#include "TacticalLines/TacticalLine_Limit.h"
#include "Tools/PositionManager.h"
#include "Tools/Position.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: TacticalLineManager constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLineManager::TacticalLineManager( const PositionManager& posMgr )
    : lines_ ()
{
    CreateDefaultTacticalLines( posMgr );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLineManager::~TacticalLineManager()
{
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        delete *it;
    lines_.clear();
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::Initialize
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::CreateDefaultTacticalLines( const PositionManager& posMgr )
{
    // Create default limits: world borders
    Position topLeft;
    Position topRight;
    Position bottomRight;
    Position bottomLeft;
    topLeft    .SetSimCoordinates(                          0, posMgr.GetWorldHeight() - 1 );
    topRight   .SetSimCoordinates( posMgr.GetWorldWidth() - 1, posMgr.GetWorldHeight() - 1 );
    bottomRight.SetSimCoordinates( posMgr.GetWorldWidth() - 1,                           0 );
    bottomLeft .SetSimCoordinates(                          0,                           0 );

    T_PositionVector pointsLeft;
    pointsLeft.push_back( &bottomLeft );
    pointsLeft.push_back( &topLeft    );
    Register( *new TacticalLine_Limit( pointsLeft ) );

    T_PositionVector pointsRight;
    pointsRight.push_back( &bottomRight );
    pointsRight.push_back( &topRight    );
    Register( *new TacticalLine_Limit( pointsRight ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::Register
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::Register( TacticalLine_ABC& line )
{
    lines_.insert( &line );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::UnRegister
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::UnRegister( TacticalLine_ABC& line )
{
    lines_.erase( &line );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::Find
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
TacticalLine_ABC* TacticalLineManager::Find( T_EntityId nId  )
{
    for( IT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        if( ( *it )->GetId() == nId )
            return *it;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: TacticalLineManager::GetLimitIdExcluding
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
T_EntityId TacticalLineManager::GetLimitIdExcluding( T_EntityId nId )
{
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        if( ( *it )->GetLineType() == TacticalLine_ABC::eLimit && ( *it )->GetId() != nId )
            return ( *it )->GetId();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineManager::UpdateToSim
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void TacticalLineManager::UpdateToSim()
{
    for( CIT_TacticalLineSet it = lines_.begin(); it != lines_.end(); ++it )
        ( *it )->UpdateToSim();
}
