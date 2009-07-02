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
#include <xeumeuleu/xml.h>

MIL_MissionType_ABC::T_MissionIDMap   MIL_AutomateMissionType::missionIDs_;
MIL_MissionType_ABC::T_MissionIDMap   MIL_AutomateMissionType::missionDiaIDs_;
MIL_MissionType_ABC::T_MissionNameMap MIL_AutomateMissionType::missionNames_;

struct MIL_AutomateMissionType::LoadingWrapper
{
    void ReadMission( xml::xistream& xis )
    {
        MIL_MissionType_ABC::ReadMission< MIL_AutomateMissionType >( xis, missionIDs_, &missionDiaIDs_, missionNames_ );
    }
};

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_AutomateMissionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing automate mission types" );

    LoadingWrapper loader;
    
    xis >> xml::start( "missions" )
            >> xml::start( "automats" )
                >> xml::list( "mission", loader, &LoadingWrapper::ReadMission )
            >> xml::end()
        >> xml::end();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_AutomateMissionType::MIL_AutomateMissionType( uint nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, xis )
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
// Name: MIL_AutomateMissionType::IsAvailableForModel
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_AutomateMissionType::IsAvailableForModel( const DIA_Model& model ) const
{
    return IsMissionAvailableForModel( model, strDIAMrtBehavior_ ) && IsMissionAvailableForModel( model, strDIACdtBehavior_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
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
        default: throw std::runtime_error( __FUNCTION__ " invalid behavior phase" );
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
