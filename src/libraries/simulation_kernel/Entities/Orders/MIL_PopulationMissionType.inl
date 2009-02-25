// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_PopulationMissionType* MIL_PopulationMissionType::Find( uint nID )
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
inline
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
inline
const std::string& MIL_PopulationMissionType::GetDIABehavior( Phase ) const
{
    return strDIABehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::MissionNames
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
inline
MIL_MissionType_ABC::T_MissionNameMap& MIL_PopulationMissionType::MissionNames()
{
    return missionNames_;
}
