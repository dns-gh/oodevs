// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_FragOrderType.h"

MIL_FragOrderType::T_MissionIDMap   MIL_FragOrderType::missionIDs_;
MIL_FragOrderType::T_MissionNameMap MIL_FragOrderType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_FragOrderType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing frag orders types" );
    
    archive.Section( "missions" );
    archive.BeginList( "fragorders" );
    while( archive.NextListElement() )
    {
        archive.BeginList( "fragorder" );
        uint nID;
        archive.ReadAttribute( "id", nID );

        const MIL_FragOrderType*& pMission = missionIDs_[ nID ];
        if( pMission )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Frag order already defined", archive.GetContext() );
        pMission = new MIL_FragOrderType( nID, archive );

        const MIL_FragOrderType*& pMissionName = missionNames_[ pMission->GetName() ];
        if( pMissionName )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automat mission name already defined", archive.GetContext() );
        pMissionName = pMission;

        archive.EndList(); // fragorder
    }
    archive.EndList(); // fragorders
    archive.EndSection(); // missions
}

// =============================================================================
// INSTANCE
// =============================================================================    

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_FragOrderType::MIL_FragOrderType( uint nID, MIL_InputArchive& archive )
    : MIL_OrderType_ABC        ( nID, archive )   
    , bAvailableWithoutMission_( false )
    , bAvailableForAllMissions_( false )
{
    archive.ReadAttribute( "available-without-mission", bAvailableWithoutMission_, MIL_InputArchive::eNothing );
    archive.ReadAttribute( "available-for-all-mission", bAvailableForAllMissions_, MIL_InputArchive::eNothing );
}

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_FragOrderType::~MIL_FragOrderType()
{
}



