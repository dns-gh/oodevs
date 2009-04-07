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
#include <xeumeuleu/xml.h>



MIL_MissionType_ABC::T_MissionIDMap   MIL_PopulationMissionType::missionIDs_;
MIL_MissionType_ABC::T_MissionNameMap MIL_PopulationMissionType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

struct MIL_PopulationMissionType::LoadingWrapper
{
    void ReadMission( xml::xistream& xis )
    {
        MIL_MissionType_ABC::ReadMission< MIL_PopulationMissionType >( xis, missionIDs_, 0, missionNames_ );
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

    xis >> xml::start( "missions" )
            >> xml::start( "populations" )
                >> xml::list( "mission", loader, &LoadingWrapper::ReadMission )
            >> xml::end()
        >> xml::end();
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::MIL_PopulationMissionType( uint nID, xml::xistream& xis )
    : MIL_MissionType_ABC( nID, xis )
    , strDIABehavior_    ( xml::attribute< std::string >( xis, "dia-behavior" ) )
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
    return IsMissionAvailableForModel( model, strDIABehavior_ );
}
