// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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
#include "Mission_Pawn_Type.h"

#include "Pawn/Mission_Pawn_Test_MoveTo.h"
#include "Pawn/Mission_Pawn_Test_CreateObject.h"
#include "Pawn/Mission_Pawn_Test_Fire.h"
#include "Pawn/Mission_Pawn_Test_Heliporter.h"

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
    Register< Mission_Pawn_Test_MoveTo >      ( "Pion Test MoveTo" );
    Register< Mission_Pawn_Test_CreateObject >( "Pion Test CreateObject" );
    Register< Mission_Pawn_Test_Fire >        ( "Pion Test Fire" );
    Register< Mission_Pawn_Test_Heliporter >  ( "Pion Test Heliporter" );
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
