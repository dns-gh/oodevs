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
#include "xeumeuleu/xml.h"



MIL_AutomateMissionType::T_MissionIDMap   MIL_AutomateMissionType::missionIDs_;
MIL_AutomateMissionType::T_MissionIDMap   MIL_AutomateMissionType::missionDiaIDs_;
MIL_AutomateMissionType::T_MissionNameMap MIL_AutomateMissionType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

struct MIL_AutomateMissionType::LoadingWrapper
{
    void ReadMission( xml::xistream& xis )
    {
        MIL_AutomateMissionType::ReadMission( xis );
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

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void MIL_AutomateMissionType::ReadMission( xml::xistream& xis )
{
    uint nID;
    xis >> xml::attribute( "id", nID );
    
    const MIL_AutomateMissionType*& pMission = missionIDs_[ nID ];
    if( pMission )
        xis.error( "Automat mission already defined" );
    pMission = new MIL_AutomateMissionType( nID, xis );
    
    const MIL_AutomateMissionType*& pMissionDiaID = missionDiaIDs_[ pMission->GetDIAType().GetDebugId() ];
    if( pMissionDiaID )
        xis.error( "Automat mission DIA ID already defined" );
    pMissionDiaID = pMission;       

    const MIL_AutomateMissionType*& pMissionName = missionNames_[ pMission->GetName() ];
    if( pMissionName )
        xis.error( "Automat mission name already defined" );
    pMissionName = pMission;       
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::InitializeAutomaticMission
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const MIL_AutomateMissionType& MIL_AutomateMissionType::InitializeAutomaticMission( xml::xistream& xis, const std::string& strTagName )
{
    xis >> xml::start( strTagName );

    std::string strMissionName;
    xis >> xml::attribute( "name", strMissionName );
    const MIL_AutomateMissionType* pType = MIL_AutomateMissionType::Find( strMissionName );
    if( !pType )
        xis.error( "Unknown mission" );
    xis >> xml::end();
    return *pType;
}

// =============================================================================
// INSTANCE
// =============================================================================    

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

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::IsAvailableForModel
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_AutomateMissionType::IsAvailableForModel( const DIA_Model& model ) const
{
    DIA_BehaviorPart& behaviorPart = static_cast< DIA_BehaviorPart& >( const_cast< DIA_Model& >( model ).GetBehaviorTool() );
    return behaviorPart.FindBehavior( strDIAMrtBehavior_ ) != 0 && behaviorPart.FindBehavior( strDIACdtBehavior_ ) != 0;
}
