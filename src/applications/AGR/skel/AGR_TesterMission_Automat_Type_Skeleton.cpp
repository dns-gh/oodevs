// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Mission_Automat_Type.h"

$IncludeList$

using namespace TEST;

Mission_Automat_Type::T_MissionAutomatTypeMap Mission_Automat_Type::missions_;

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Automat_Type::Mission_Automat_Type( const std::string& strName, T_MissionAllocator missionAllocator )
    : strName_          ( strName )
    , missionAllocator_ ( missionAllocator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Automat_Type::~Mission_Automat_Type()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::Initialize
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Automat_Type::Initialize()
{
$AutomateMissionsRegistration$
}
    
// -----------------------------------------------------------------------------
// Name: Mission_Automat_Type::Terminate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Automat_Type::Terminate()
{
    for( CIT_MissionAutomatTypeMap it = missions_.begin(); it != missions_.end(); ++it )
        delete it->second;
    missions_.clear();
}
