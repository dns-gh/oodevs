// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateMissionType.h"
#include "MT_Tools/MT_Logger.h"

MIL_MissionType_ABC::T_MissionIDMap MIL_AutomateMissionType::missionIDs_;
MIL_MissionType_ABC::T_MissionNameMap MIL_AutomateMissionType::missionDiaIDs_;
MIL_MissionType_ABC::T_MissionNameMap MIL_AutomateMissionType::missionNames_;

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_AutomateMissionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing automate mission types" );
    xis >> xml::start( "missions" )
            >> xml::start( "automats" )
                >> xml::list( "mission", boost::bind( &MIL_MissionType_ABC::ReadMission< MIL_AutomateMissionType >, _1, boost::ref( missionIDs_ ), &missionDiaIDs_, boost::ref( missionNames_ ) ) )
            >> xml::end
        >> xml::end;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_AutomateMissionType::MIL_AutomateMissionType( unsigned int nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, 4, xis )
    , strDIAMrtBehavior_ ()
    , strDIACdtBehavior_ ()
{
    xis >> xml::attribute( "mrt-dia-behavior", strDIAMrtBehavior_ )
        >> xml::attribute( "cdt-dia-behavior", strDIACdtBehavior_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_AutomateMissionType::~MIL_AutomateMissionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC* MIL_AutomateMissionType::Find( unsigned int nID )
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
const MIL_MissionType_ABC* MIL_AutomateMissionType::FindFromDiaID( const std::string& nID )
{
    CIT_MissionNameMap it = missionDiaIDs_.find( nID );
    if( it == missionDiaIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
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
const std::string& MIL_AutomateMissionType::GetDIABehavior( Phase phase ) const
{
    switch( phase )
    {
        case ePhaseMRT : return strDIAMrtBehavior_;
        case ePhaseCDT : return strDIACdtBehavior_;
        default: throw MASA_EXCEPTION( "Invalid behavior phase." );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::MissionNames
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC::T_MissionNameMap& MIL_AutomateMissionType::MissionNames()
{
    return missionNames_;
}