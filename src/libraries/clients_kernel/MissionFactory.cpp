// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MissionFactory.h"
#include "Mission.h"
#include "FragOrder.h"
#include "MissionType.h"
#include "FragOrderType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionFactory constructor
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
MissionFactory::MissionFactory( const tools::Resolver_ABC< MissionType, std::string >& unitMissions,
                                const tools::Resolver_ABC< MissionType, std::string >& automatMissions,
                                const tools::Resolver_ABC< MissionType, std::string >& populationMissions,
                                const tools::Resolver_ABC< FragOrderType, std::string >& fragOrders )
    : unitMissions_( unitMissions )
    , automatMissions_( automatMissions )
    , populationMissions_( populationMissions )
    , fragOrders_( fragOrders )
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
Mission* MissionFactory::CreateAgentMission( const std::string& name ) const
{
    MissionType* type = unitMissions_.Find( name );
    if( !type )
        throw MASA_EXCEPTION( "unknown agent mission '" + name + "'" );
    return new Mission( *type );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreateAutomatMission( const std::string& name ) const
{
    MissionType* type = automatMissions_.Find( name );
    if( !type )
        throw MASA_EXCEPTION( "unknown automat mission '" + name + "'" );
    return new Mission( *type );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreatePopulationMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreatePopulationMission( const std::string& name ) const
{
    MissionType* type = populationMissions_.Find( name );
    if( !type )
        throw MASA_EXCEPTION( "unknown population mission '" + name + "'" );
    return new Mission( *type );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateFragOrder
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
FragOrder* MissionFactory::CreateFragOrder( const std::string& name ) const
{
    FragOrderType* type = fragOrders_.Find( name );
    if( !type )
        throw MASA_EXCEPTION( "unknown frag order '" + name + "'" );
    return new FragOrder( *type );
}
