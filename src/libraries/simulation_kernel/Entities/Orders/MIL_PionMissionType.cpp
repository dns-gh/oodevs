// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PionMissionType.h"
#include "xeumeuleu/xml.h"

using namespace xml;

MIL_PionMissionType::T_MissionIDMap   MIL_PionMissionType::missionIDs_;
MIL_PionMissionType::T_MissionIDMap   MIL_PionMissionType::missionDiaIDs_;
MIL_PionMissionType::T_MissionNameMap MIL_PionMissionType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

struct MIL_PionMissionType::LoadingWrapper
{
    void ReadMission( xml::xistream& xis )
    {
        MIL_PionMissionType::ReadMission( xis );
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
    
    xis >> start( "missions" )
            >> start( "units" )
                >> xml::list( "mission", loader, &LoadingWrapper::ReadMission )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void MIL_PionMissionType::ReadMission( xml::xistream& xis )
{
    uint nID;
    xis >> attribute( "id", nID );

    const MIL_PionMissionType*& pMission = missionIDs_[ nID ];
    if( pMission )
        xis.error( "Unit mission id already defined" );
    pMission = new MIL_PionMissionType( nID, xis );

    const MIL_PionMissionType*& pMissionDiaID = missionDiaIDs_[ pMission->GetDIAType().GetDebugId() ];
    if( pMissionDiaID )
        xis.error( "Unit mission DIA ID already defined" );
    pMissionDiaID = pMission;       

    const MIL_PionMissionType*& pMissionName = missionNames_[ pMission->GetName() ];
    if( pMissionName )
        xis.error( "Unit mission name already defined" );
    pMissionName = pMission;
}

// =============================================================================
// INSTANCE
// =============================================================================    

//-----------------------------------------------------------------------------
// Name: MIL_PionMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PionMissionType::MIL_PionMissionType( uint nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, xis )
    , strDIABehavior_    ()
{
    xis >> attribute( "dia-behavior", strDIABehavior_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_PionMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PionMissionType::~MIL_PionMissionType()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionMissionType::IsAvailableForModel
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PionMissionType::IsAvailableForModel( const DIA_Model& model ) const
{
    DIA_BehaviorPart& behaviorPart = static_cast< DIA_BehaviorPart& >( const_cast< DIA_Model& >( model ).GetBehaviorTool() );
    return behaviorPart.FindBehavior( strDIABehavior_ ) != 0;
}
