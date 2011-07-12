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
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <boost/format.hpp>

MIL_FragOrderType::T_MissionIDMap MIL_FragOrderType::missionIDs_;
MIL_FragOrderType::T_MissionNameMap MIL_FragOrderType::missionNames_;
MIL_FragOrderType::T_MissionNameMap MIL_FragOrderType::fragOrderDiaTypes_;

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
    xis >> xml::start( "missions" )
            >> xml::start( "fragorders" )
                >> xml::list( "fragorder", loader, &LoadingWrapper::ReadFragorder )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::ReadFragorder
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void MIL_FragOrderType::ReadFragorder( xml::xistream& xis )
{
    unsigned int nID;
    xis >> xml::attribute( "id", nID );

    const MIL_FragOrderType*& pMission = missionIDs_[ nID ];
    if( pMission )
        xis.error( "Frag order already defined" );
    pMission = new MIL_FragOrderType( nID, xis );

    const MIL_FragOrderType*& pMissionName = missionNames_[ pMission->GetName() ];
    if( pMissionName )
        xis.error( "Entity fragOrder name already defined" );
    pMissionName = pMission;

    const MIL_FragOrderType*& pMissionDiaType = fragOrderDiaTypes_[ pMission->GetDIAType() ];
    if( pMissionDiaType )
        MT_LOG_WARNING_MSG( boost::format( "Entity fragOrder for diaType %s defined multiple times" ) % pMission->GetDIAType() );
    pMissionDiaType = pMission;
}

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_FragOrderType::MIL_FragOrderType( unsigned int nID, xml::xistream& xis )
    : MIL_OrderType_ABC        ( nID, 0, xis )
    , bAvailableWithoutMission_( false )
    , bAvailableForAllMissions_( false )
{
    xis >> xml::optional >> xml::attribute( "available-without-mission", bAvailableWithoutMission_ )
        >> xml::optional >> xml::attribute( "available-for-all-mission", bAvailableForAllMissions_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_FragOrderType destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_FragOrderType::~MIL_FragOrderType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_FragOrderType* MIL_FragOrderType::Find( unsigned int nID )
{
    CIT_MissionIDMap it = missionIDs_.find( nID );
    if( it == missionIDs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::Find
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_FragOrderType* MIL_FragOrderType::Find( const std::string& strName )
{
    CIT_MissionNameMap it = missionNames_.find( strName );
    if( it == missionNames_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::FindByDiaType
// Created: MGD 2011-01-20
// -----------------------------------------------------------------------------
const MIL_FragOrderType* MIL_FragOrderType::FindByDiaType( const std::string& strName )
{
    CIT_MissionNameMap it = fragOrderDiaTypes_.find( strName );
    if( it == fragOrderDiaTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_FragOrderType::IsAvailableWithoutMission
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
bool MIL_FragOrderType::IsAvailableWithoutMission() const
{
    return bAvailableWithoutMission_;
}
