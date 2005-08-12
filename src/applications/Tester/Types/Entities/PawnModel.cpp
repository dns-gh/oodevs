// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
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

#include "Tester_Pch.h"
#include "PawnModel.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
#include "Actions/Scheduler.h"
#include "Entities/Pawn.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: PawnModel constructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
PawnModel::PawnModel( const std::string& strName, XmlInputArchive& archive )
    : strName_      ( strName )
    , missionTypes_ ()
{
    try   
    {
        std::string strValue;

        archive.BeginList( "Missions" );
        while( archive.NextListElement() )
        {
            archive.Section( "Mission" );
            std::string strMissionName;
            archive.ReadAttribute( "nom", strMissionName );
            const Mission_Pawn_Type* missionType = Mission_Pawn_Type::Find( strMissionName );
            if( missionType )
                missionTypes_.push_back( missionType );
            else
                MT_LOG_WARNING_MSG( "Reading models: mission type \"" << strMissionName << "\" not found" );
            archive.EndSection(); // Mission
        }
        archive.EndList(); // Missions
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PawnModel destructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
PawnModel::~PawnModel()
{
    missionTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PawnModel::ScheduleAllMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void PawnModel::ScheduleAllMissions( Pawn& pawn, Scheduler& scheduler ) const
{
    for( CIT_MissionPawnTypeVector it = missionTypes_.begin(); it != missionTypes_.end(); ++it )
        scheduler.AddAction( Mission_Pawn_Type::CreateMission( **it, pawn ) );
}

// -----------------------------------------------------------------------------
// Name: PawnModel::ScheduleMission
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void PawnModel::ScheduleMission( Pawn& pawn, Scheduler& scheduler, const std::string& strMissionName ) const
{
    scheduler.AddAction( Mission_Pawn_Type::CreateMission( strMissionName, pawn ) );
}
