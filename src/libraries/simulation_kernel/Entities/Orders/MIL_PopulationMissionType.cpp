// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PopulationMissionType.h"
#include "xeumeuleu/xml.h"

using namespace xml;

MIL_PopulationMissionType::T_MissionIDMap   MIL_PopulationMissionType::missionIDs_;
MIL_PopulationMissionType::T_MissionNameMap MIL_PopulationMissionType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

struct MIL_PopulationMissionType::LoadingWrapper
{
    void ReadMission( xml::xistream& xis )
    {
        MIL_PopulationMissionType::ReadMission( xis );
    }
};

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_PopulationMissionType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing population mission types" );

    LoadingWrapper loader;

    xis >> start( "missions" )
            >> start( "populations" )
                >> list( "mission", loader, &LoadingWrapper::ReadMission )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::ReadMission
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void MIL_PopulationMissionType::ReadMission( xml::xistream& xis )
{
    uint nID;
    xis >> attribute( "id", nID );

    const MIL_PopulationMissionType*& pMission = missionIDs_[ nID ];
    if( pMission )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Population mission already defined" ); // $$$$ ABL 2007-07-25: error context
    pMission = new MIL_PopulationMissionType( nID, xis );

    const MIL_PopulationMissionType*& pMissionName = missionNames_[ pMission->GetName() ];
    if( pMissionName )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Population mission name already defined" ); // $$$$ ABL 2007-07-25: error context
    pMissionName = pMission;
}

// =============================================================================
// INSTANCE
// =============================================================================    

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::MIL_PopulationMissionType( uint nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, xis )
    , strDIABehavior_    ()
{
    xis >> attribute( "dia-behavior", strDIABehavior_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::~MIL_PopulationMissionType()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::IsAvailableForModel
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PopulationMissionType::IsAvailableForModel( const DIA_Model& model ) const
{
    DIA_BehaviorPart& behaviorPart = static_cast< DIA_BehaviorPart& >( const_cast< DIA_Model& >( model ).GetBehaviorTool() );
    return behaviorPart.FindBehavior( strDIABehavior_ ) != 0;
}
