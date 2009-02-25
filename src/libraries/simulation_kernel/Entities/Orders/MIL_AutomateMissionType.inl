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
const MIL_MissionType_ABC* MIL_AutomateMissionType::Find( uint nID )
{
    CIT_MissionIDMap it = missionIDs_.find( nID );
    if( it == missionIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::FindFromDiaID
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
inline
const MIL_MissionType_ABC* MIL_AutomateMissionType::FindFromDiaID( uint nID )
{
    CIT_MissionIDMap it = missionDiaIDs_.find( nID );
    if( it == missionDiaIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_MissionType_ABC* MIL_AutomateMissionType::Find( const std::string& strName )
{
    CIT_MissionNameMap it = missionNames_.find( strName );
    if( it == missionNames_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::GetDIABehavior
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
inline
const std::string& MIL_AutomateMissionType::GetDIABehavior( Phase phase ) const
{
    switch( phase ) 
    {
        case ePhaseMRT : return strDIAMrtBehavior_;  
        case ePhaseCDT : return strDIACdtBehavior_; 
        default: throw( std::runtime_error( "invalid PHASE" ) ); 
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::MissionNames
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
inline
const MIL_MissionType_ABC::T_MissionNameMap& MIL_AutomateMissionType::MissionNames()
{
    return missionNames_;
}
