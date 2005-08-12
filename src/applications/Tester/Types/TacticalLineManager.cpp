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
    Position* pTopLeft     = new Position();
    Position* pTopRight    = new Position();
    Position* pBottomRight = new Position();
    Position* pBottomLeft  = new Position();
    pTopLeft    ->SetSimCoordinates(                          0, posMgr.GetWorldHeight() - 1 );
    pTopRight   ->SetSimCoordinates( posMgr.GetWorldWidth() - 1, posMgr.GetWorldHeight() - 1 );
    pBottomRight->SetSimCoordinates( posMgr.GetWorldWidth() - 1,                           0 );
    pBottomLeft ->SetSimCoordinates(                          0,                           0 );

    T_PositionVector* pPointsLeft = new T_PositionVector();
    pPointsLeft->push_back( pBottomLeft );
    pPointsLeft->push_back( pTopLeft    );
    Register( *new TacticalLine_Limit( *pPointsLeft ) );

    T_PositionVector* pPointsRight = new T_PositionVector();
    pPointsRight->push_back( pBottomRight );
    pPointsRight->push_back( pTopRight    );
    Register( *new TacticalLine_Limit( *pPointsRight ) );
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
