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

      MIL_AutomateMissionType::T_MissionIDMap   MIL_AutomateMissionType::missionIDs_;
      MIL_AutomateMissionType::T_MissionNameMap MIL_AutomateMissionType::missionNames_;
const MIL_AutomateMissionType*                  MIL_AutomateMissionType::pSurrenderingMissionType_      = 0;
const MIL_AutomateMissionType*                  MIL_AutomateMissionType::pMoveToRefugeeCampMissionType_ = 0;

// =============================================================================
// FACTORY
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_AutomateMissionType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing automate mission types" );
    
    archive.Section( "missions" );
    archive.BeginList( "automats" );
    while( archive.NextListElement() )
    {
        archive.BeginList( "mission" );
        uint nID;
        archive.ReadAttribute( "id", nID );

        const MIL_AutomateMissionType*& pMission = missionIDs_[ nID ];
        if( pMission )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automat mission already defined", archive.GetContext() );
        pMission = new MIL_AutomateMissionType( nID, archive );

        const MIL_AutomateMissionType*& pMissionName = missionNames_[ pMission->GetName() ];
        if( pMissionName )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automat mission name already defined", archive.GetContext() );
        pMissionName = pMission;

        archive.EndList(); // mission
    }
    archive.EndList(); // automats

    archive.Section( "misc" );
    pSurrenderingMissionType_       = &InitializeAutomaticMission( archive, "mission-automat-surrender" );
    pMoveToRefugeeCampMissionType_  = &InitializeAutomaticMission( archive, "mission-automat-go-to-refugee-camp" );    
    archive.EndSection(); // misc

    archive.EndSection(); // missions
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType::InitializeAutomaticMission
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const MIL_AutomateMissionType& MIL_AutomateMissionType::InitializeAutomaticMission( MIL_InputArchive& archive, const std::string& strTagName )
{
    archive.Section( strTagName );

    std::string strMissionName;
    archive.ReadAttribute( "name", strMissionName );
    const MIL_AutomateMissionType* pType = MIL_AutomateMissionType::Find( strMissionName );
    if( !pType )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown mission", archive.GetContext() );
    archive.EndSection();
    return *pType;
}

// =============================================================================
// INSTANCE
// =============================================================================    

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_AutomateMissionType::MIL_AutomateMissionType( uint nID, MIL_InputArchive& archive )
    : MIL_MissionType_ABC( nID, archive )
    , strDIAMrtBehavior_ ()
    , strDIACdtBehavior_ ()
{
    archive.ReadAttribute( "mrt-dia-behavior", strDIAMrtBehavior_ );
    archive.ReadAttribute( "cdt-dia-behavior", strDIACdtBehavior_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMissionType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_AutomateMissionType::~MIL_AutomateMissionType()
{
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
