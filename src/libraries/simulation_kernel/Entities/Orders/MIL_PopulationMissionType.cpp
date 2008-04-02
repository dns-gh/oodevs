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
                >> xml::list( "mission", loader, &LoadingWrapper::ReadMission )
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
        xis.error( "Population mission already defined" );
    pMission = new MIL_PopulationMissionType( nID, xis );

    const MIL_PopulationMissionType*& pMissionName = missionNames_[ pMission->GetName() ];
    if( pMissionName )
        xis.error( "Population mission name already defined" );
    pMissionName = pMission;
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::MIL_PopulationMissionType( uint nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, xis )
    , strDIABehavior_    ( attribute< std::string >( xis, "dia-behavior" ) )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::~MIL_PopulationMissionType()
{
    // NOTHING
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
