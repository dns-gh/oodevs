// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationMissionType.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

MIL_MissionType_ABC::T_MissionIDMap MIL_PopulationMissionType::missionIDs_;
MIL_MissionType_ABC::T_MissionNameMap MIL_PopulationMissionType::missionNames_;

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_PopulationMissionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing population mission types" );
    xis >> xml::start( "missions" )
            >> xml::start( "populations" )
            >> xml::list( "mission", boost::bind( &MIL_MissionType_ABC::ReadMission< MIL_PopulationMissionType >, _1, boost::ref( missionIDs_ ),
                static_cast< MIL_MissionType_ABC::T_MissionNameMap* >( 0 ), boost::ref( missionNames_ ) ) )
            >> xml::end
        >> xml::end;
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::MIL_PopulationMissionType( unsigned int nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, 0, xis )
    , strDIABehavior_    ( xis.attribute< std::string >( "dia-behavior" ) )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::~MIL_PopulationMissionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC* MIL_PopulationMissionType::Find( unsigned int nID )
{
    CIT_MissionIDMap it = missionIDs_.find( nID );
    if( it == missionIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC* MIL_PopulationMissionType::Find( const std::string& strName )
{
    CIT_MissionNameMap it = missionNames_.find( strName );
    if( it == missionNames_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::GetDIABehavior
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const std::string& MIL_PopulationMissionType::GetDIABehavior( Phase ) const
{
    return strDIABehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::MissionNames
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
MIL_MissionType_ABC::T_MissionNameMap& MIL_PopulationMissionType::MissionNames()
{
    return missionNames_;
}
