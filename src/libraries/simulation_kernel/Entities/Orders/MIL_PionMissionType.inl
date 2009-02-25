// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType* MIL_PionMissionType::Find( uint nID )
{
    CIT_MissionIDMap it = missionIDs_.find( nID );
    if( it == missionIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::FindFromDiaID
// Created: NLD 2006-12-04
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType* MIL_PionMissionType::FindFromDiaID( uint nID )
{
    CIT_MissionIDMap it = missionDiaIDs_.find( nID );
    if( it == missionDiaIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_MissionType_ABC* MIL_PionMissionType::Find( const std::string& strName )
{
    CIT_MissionNameMap it = missionNames_.find( strName );
    if( it == missionNames_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::GetDIABehavior
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PionMissionType::GetDIABehavior( Phase ) const
{
    return strDIABehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::MissionNames
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
inline
const MIL_MissionType_ABC::T_MissionNameMap& MIL_PionMissionType::MissionNames()
{
    return missionNames_;
}