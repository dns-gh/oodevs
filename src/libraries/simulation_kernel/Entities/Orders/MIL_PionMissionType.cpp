// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PionMissionType.h"
#include <xeumeuleu/xml.h>

MIL_MissionType_ABC::T_MissionIDMap   MIL_PionMissionType::missionIDs_;
MIL_MissionType_ABC::T_MissionIDMap   MIL_PionMissionType::missionDiaIDs_;
MIL_MissionType_ABC::T_MissionNameMap MIL_PionMissionType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

struct MIL_PionMissionType::LoadingWrapper
{
    void ReadMission( xml::xistream& xis )
    {
        MIL_MissionType_ABC::ReadMission< MIL_PionMissionType >( xis, missionIDs_, &missionDiaIDs_, missionNames_ );
    }
};

//-----------------------------------------------------------------------------
// Name: MIL_PionMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_PionMissionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing pion mission types" );

    LoadingWrapper loader;
    
    xis >> xml::start( "missions" )
            >> xml::start( "units" )
                >> xml::list( "mission", loader, &LoadingWrapper::ReadMission )
            >> xml::end()
        >> xml::end();
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PionMissionType::MIL_PionMissionType( uint nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, xis )
    , strDIABehavior_    ()
{
    xis >> xml::attribute( "dia-behavior", strDIABehavior_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PionMissionType::~MIL_PionMissionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::IsAvailableForModel
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PionMissionType::IsAvailableForModel( const DIA_Model& model ) const
{
    return IsMissionAvailableForModel( model, strDIABehavior_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC* MIL_PionMissionType::Find( uint nID )
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
const MIL_MissionType_ABC* MIL_PionMissionType::FindFromDiaID( uint nID )
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
const std::string& MIL_PionMissionType::GetDIABehavior( Phase ) const
{
    return strDIABehavior_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::MissionNames
// Created: LDC 2009-02-24
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC::T_MissionNameMap& MIL_PionMissionType::MissionNames()
{
    return missionNames_;
}