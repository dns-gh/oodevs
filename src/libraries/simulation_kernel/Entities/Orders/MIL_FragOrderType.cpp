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
#include "xeumeuleu/xml.h"

using namespace xml;

MIL_FragOrderType::T_MissionIDMap   MIL_FragOrderType::missionIDs_;
MIL_FragOrderType::T_MissionNameMap MIL_FragOrderType::missionNames_;

// =============================================================================
// FACTORY
// =============================================================================

struct MIL_FragOrderType::LoadingWrapper
{
    void ReadFragorder( xml::xistream& xis )
    {
        MIL_FragOrderType::ReadFragorder( xis );
    }
};

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType::Initialize
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
void MIL_FragOrderType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing frag orders types" );

    LoadingWrapper loader;
    
    xis >> start( "missions" )
            >> start( "fragorders" )
                >> list( "fragorder", loader, &LoadingWrapper::ReadFragorder )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::ReadFragorder
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void MIL_FragOrderType::ReadFragorder( xml::xistream& xis )
{
    uint nID;
    xis >> attribute( "id", nID );

    const MIL_FragOrderType*& pMission = missionIDs_[ nID ];
    if( pMission )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Frag order already defined" ); // $$$$ ABL 2007-07-25: error context
    pMission = new MIL_FragOrderType( nID, xis );

    const MIL_FragOrderType*& pMissionName = missionNames_[ pMission->GetName() ];
    if( pMissionName )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automat mission name already defined" ); // $$$$ ABL 2007-07-25: error context
    pMissionName = pMission;
}

// =============================================================================
// INSTANCE
// =============================================================================    

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_FragOrderType::MIL_FragOrderType( uint nID, xml::xistream& xis )
    : MIL_OrderType_ABC        ( nID, xis )   
    , bAvailableWithoutMission_( false )
    , bAvailableForAllMissions_( false )
{
    xis >> optional() >> attribute( "available-without-mission", bAvailableWithoutMission_ )
        >> optional() >> attribute( "available-for-all-mission", bAvailableForAllMissions_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_FragOrderType::~MIL_FragOrderType()
{
    // NOTHING
}



