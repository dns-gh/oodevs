// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_MissionType_ABC::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
template<typename T>
void MIL_MissionType_ABC::ReadMission( xml::xistream& xis, MIL_MissionType_ABC::T_MissionIDMap& missionIDs, MIL_MissionType_ABC::T_MissionIDMap* pMissionDiaIDs, MIL_MissionType_ABC::T_MissionNameMap& missionNames )
{
    uint nID;
    xis >> xml::attribute( "id", nID );

    const MIL_MissionType_ABC*& pMission = missionIDs[ nID ];
    if( pMission )
        xis.error( "Mission id already defined" );
    pMission = new T( nID, xis );

    if( pMissionDiaIDs )
    {
        const MIL_MissionType_ABC*& pMissionDiaID = (*pMissionDiaIDs)[ pMission->GetDIAType().GetDebugId() ];
        if( pMissionDiaID )
            xis.error( "Mission DIA ID already defined" );
        pMissionDiaID = pMission;
    }

    const MIL_MissionType_ABC*& pMissionName = missionNames[ pMission->GetName() ];
    if( pMissionName )
        xis.error( "Mission name already defined" );
    pMissionName = pMission;
}