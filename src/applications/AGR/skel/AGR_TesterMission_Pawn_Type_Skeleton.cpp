// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Pawn_Type.h"

$IncludeList$

using namespace TEST;

Mission_Pawn_Type::T_MissionPawnTypeMap Mission_Pawn_Type::missions_;

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Pawn_Type::Mission_Pawn_Type( const std::string& strName, T_MissionAllocator missionAllocator )
    : strName_          ( strName )
    , missionAllocator_ ( missionAllocator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Pawn_Type::~Mission_Pawn_Type()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::Initialize
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Pawn_Type::Initialize()
{
$PionMissionsRegistration$
}
    
// -----------------------------------------------------------------------------
// Name: Mission_Pawn_Type::Terminate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Pawn_Type::Terminate()
{
    for( CIT_MissionPawnTypeMap it = missions_.begin(); it != missions_.end(); ++it )
        delete it->second;
    missions_.clear();
}
