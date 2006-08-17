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

#include "simulation_tester_pch.h"
#include "PopulationModel.h"
#include "Actions/Missions/Mission_Population_Type.h"
#include "Actions/Scheduler.h"
#include "Entities/Population.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: PopulationModel constructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
PopulationModel::PopulationModel( const std::string& strName, XmlInputArchive& archive )
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
            const Mission_Population_Type* missionType = Mission_Population_Type::Find( strMissionName );
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
// Name: PopulationModel destructor
// Created: SBO 2005-08-11
// -----------------------------------------------------------------------------
PopulationModel::~PopulationModel()
{
    missionTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PopulationModel::ScheduleAllMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void PopulationModel::ScheduleAllMissions( Population& population, Scheduler& scheduler, uint nIteration /* = 1 */ ) const
{
    for( CIT_MissionPopulationTypeVector it = missionTypes_.begin(); it != missionTypes_.end(); ++it )
        scheduler.AddActions( Mission_Population_Type::CreateMission( **it, population ), nIteration );
}

// -----------------------------------------------------------------------------
// Name: PopulationModel::ScheduleMission
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void PopulationModel::ScheduleMission( Population& population, Scheduler& scheduler, const std::string& strMissionName, uint nIteration /* = 1 */ ) const
{
    scheduler.AddActions( Mission_Population_Type::CreateMission( strMissionName, population ), nIteration );
}
