// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MissionFactory.h"
#include "ENT/ENT_Tr.h"
#include "Mission.h"
#include "FragOrder.h"

// -----------------------------------------------------------------------------
// Name: MissionFactory constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
MissionFactory::MissionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionFactory destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
MissionFactory::~MissionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateAgentMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreateAgentMission( const std::string& name )
{
    E_UnitMission mission = ENT_Tr::ConvertToUnitMission( name );
    if( mission == -1 )
        throw std::runtime_error( "unknown agent mission '" + name + "'" );
    return new Mission( name, mission, false );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreateAutomatMission( const std::string& name )
{
    E_AutomataMission mission = ENT_Tr::ConvertToAutomataMission( name );
    if( mission == -1 )
        throw std::runtime_error( "unknown automat mission '" + name + "'" );
    return new Mission( name, mission, true );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreatePopulationMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreatePopulationMission( const std::string& name )
{
    E_PopulationMission mission = ENT_Tr::ConvertToPopulationMission( name );
    if( mission == -1 )
        throw std::runtime_error( "unknown population mission '" + name + "'" );
    return new Mission( name, mission, false );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateFragOrder
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
FragOrder* MissionFactory::CreateFragOrder( const std::string& name )
{
    E_FragOrder order = ENT_Tr::ConvertToFragOrder( name );
    if( order == -1 )
        throw std::runtime_error( "unknown frag order '" + name + "'" );
    return new FragOrder( name, order );
}
