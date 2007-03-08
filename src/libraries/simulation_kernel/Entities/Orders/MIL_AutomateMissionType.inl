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
const MIL_AutomateMissionType* MIL_AutomateMissionType::Find( uint nID )
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
const MIL_AutomateMissionType* MIL_AutomateMissionType::Find( const std::string& strName )
{
    CIT_MissionNameMap it = missionNames_.find( strName );
    if( it == missionNames_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetDIAMrtBehavior
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateMissionType::GetDIAMrtBehavior() const
{
    return strDIAMrtBehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetDIACdtBehavior
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateMissionType::GetDIACdtBehavior() const
{
    return strDIACdtBehavior_;
}
