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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionFactory constructor
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
MissionFactory::MissionFactory( const Resolver_ABC< Mission, QString >& unitMissions, const Resolver_ABC< Mission, QString >& automatMissions
                              , const Resolver_ABC< Mission, QString >& populationMissions, const Resolver_ABC< FragOrder, QString >& fragOrders )
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
Mission* MissionFactory::CreateAgentMission( const std::string& name )
{
    Mission* mission = unitMissions_.Find( name.c_str() );
    if( !mission )
        throw std::runtime_error( "unknown agent mission '" + name + "'" );
    return AddFragOrders( new Mission( *mission ) );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreateAutomatMission( const std::string& name )
{
    Mission* mission = automatMissions_.Find( name.c_str() );
    if( !mission )
        throw std::runtime_error( "unknown automat mission '" + name + "'" );
    return AddFragOrders( new Mission( *mission ) );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreatePopulationMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreatePopulationMission( const std::string& name )
{
    Mission* mission = populationMissions_.Find( name.c_str() );
    if( !mission )
        throw std::runtime_error( "unknown population mission '" + name + "'" );
    return new Mission( *mission );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateFragOrder
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
FragOrder* MissionFactory::CreateFragOrder( const std::string& name )
{
    FragOrder* order = fragOrders_.Find( name.c_str() );
    if( !order )
        throw std::runtime_error( "unknown frag order '" + name + "'" );
    return new FragOrder( *order );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::AddFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
Mission* MissionFactory::AddFragOrders( Mission* mission )
{
    Iterator< const FragOrder& > it = fragOrders_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const FragOrder& order = it.NextElement();
        if( !order.IsMissionRequired() )
        {
            FragOrder* newOrder = CreateFragOrder( order.GetName().ascii() );
            static_cast< Resolver< FragOrder >* >( mission )->Register( newOrder->GetId(), *newOrder );
        }
    }
    return mission;
}
