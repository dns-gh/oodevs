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
MissionFactory::MissionFactory( const tools::Resolver_ABC< MissionType, std::string >& unitMissions, const tools::Resolver_ABC< MissionType, std::string >& automatMissions
                              , const tools::Resolver_ABC< MissionType, std::string >& populationMissions, const tools::Resolver_ABC< FragOrderType, std::string >& fragOrders )
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
    MissionType* type = unitMissions_.Find( name );
    if( !type )
        throw std::runtime_error( "unknown agent mission '" + name + "'" );
    return AddFragOrders( new Mission( *type ) );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreateAutomatMission( const std::string& name )
{
    MissionType* type = automatMissions_.Find( name );
    if( !type )
        throw std::runtime_error( "unknown automat mission '" + name + "'" );
    return AddFragOrders( new Mission( *type ) );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreatePopulationMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission* MissionFactory::CreatePopulationMission( const std::string& name )
{
    MissionType* type = populationMissions_.Find( name );
    if( !type )
        throw std::runtime_error( "unknown population mission '" + name + "'" );
    return new Mission( *type );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::CreateFragOrder
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
FragOrder* MissionFactory::CreateFragOrder( const std::string& name )
{
    FragOrderType* type = fragOrders_.Find( name );
    if( !type )
        throw std::runtime_error( "unknown frag order '" + name + "'" );
    return new FragOrder( *type );
}

// -----------------------------------------------------------------------------
// Name: MissionFactory::AddFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
Mission* MissionFactory::AddFragOrders( Mission* mission )
{
    tools::Iterator< const FragOrderType& > it = fragOrders_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const FragOrderType& type = it.NextElement();
        if( !type.IsMissionRequired() )
        {
            FragOrder* newOrder = CreateFragOrder( type.GetName() );
            static_cast< tools::Resolver< FragOrder >* >( mission )->Register( newOrder->GetId(), *newOrder );
        }
    }
    return mission;
}
