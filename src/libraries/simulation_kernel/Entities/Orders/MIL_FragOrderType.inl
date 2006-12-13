// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_FragOrderType* MIL_FragOrderType::Find( uint nID )
{
    CIT_MissionIDMap it = missionIDs_.find( nID );
    if( it == missionIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_FragOrderType* MIL_FragOrderType::Find( const std::string& strName )
{
    CIT_MissionNameMap it = missionNames_.find( strName );
    if( it == missionNames_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::IsAvailableWithoutMission
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
inline
bool MIL_FragOrderType::IsAvailableWithoutMission() const
{
    return bAvailableWithoutMission_;
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::IsAvailableForAllMissions
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
inline
bool MIL_FragOrderType::IsAvailableForAllMissions() const
{
    return bAvailableForAllMissions_;
}
