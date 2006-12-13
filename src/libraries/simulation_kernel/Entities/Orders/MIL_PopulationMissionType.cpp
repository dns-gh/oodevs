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

MIL_PopulationMissionType::T_MissionIDMap   MIL_PopulationMissionType::missionIDs_;
MIL_PopulationMissionType::T_MissionNameMap MIL_PopulationMissionType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_PopulationMissionType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing population mission types" );
    
    archive.Section( "missions" );
    archive.BeginList( "populations" );
    while( archive.NextListElement() )
    {
        archive.BeginList( "mission" );
        uint nID;
        archive.ReadAttribute( "id", nID );

        const MIL_PopulationMissionType*& pMission = missionIDs_[ nID ];
        if( pMission )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Population mission already defined", archive.GetContext() );
        pMission = new MIL_PopulationMissionType( nID, archive );

        const MIL_PopulationMissionType*& pMissionName = missionNames_[ pMission->GetName() ];
        if( pMissionName )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Population mission name already defined", archive.GetContext() );
        pMissionName = pMission;

        archive.EndList(); // mission
    }
    archive.EndList(); // populations
    archive.EndSection(); // missions
}

// =============================================================================
// INSTANCE
// =============================================================================    

//-----------------------------------------------------------------------------
// Name: MIL_PopulationMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_PopulationMissionType::MIL_PopulationMissionType( uint nID, MIL_InputArchive& archive )
    : MIL_MissionType_ABC( nID, archive )
    , strDIABehavior_    ()
{
    archive.ReadAttribute( "dia-behavior", strDIABehavior_ );
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
