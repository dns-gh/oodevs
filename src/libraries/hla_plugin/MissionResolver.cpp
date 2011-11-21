// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "MissionResolver.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/FragOrderType.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: MissionResolver constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
MissionResolver::MissionResolver( const tools::Resolver_ABC< kernel::MissionType, unsigned long >& missions,
                                  const tools::Resolver_ABC< kernel::FragOrderType, unsigned long >& fragOrders )
{
    for( tools::Iterator< const kernel::MissionType& > it = missions.CreateIterator(); it.HasMoreElements(); )
    {
        const kernel::MissionType& mission = it.NextElement();
        T_Mission& missionId = missions_[ mission.GetName() ];
        missionId = mission.IsAutomat() ? std::make_pair( mission.GetId(), missionId.second ) : std::make_pair( missionId.first, mission.GetId() );
    }
    for( tools::Iterator< const kernel::FragOrderType& > it = fragOrders.CreateIterator(); it.HasMoreElements(); )
    {
        const kernel::FragOrderType& fragOrder = it.NextElement();
        missions_[ fragOrder.GetName() ] = std::make_pair( fragOrder.GetId(), fragOrder.GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionResolver destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
MissionResolver::~MissionResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionResolver::ResolveAutomat
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
unsigned int MissionResolver::ResolveAutomat( const std::string& name ) const
{
    T_Missions::const_iterator mission = missions_.find( name );
    if( mission == missions_.end() )
        throw std::runtime_error( "Unknown mission name '" + name + "'" );
    if( !mission->second.first )
        throw std::runtime_error( "Mission '" + name + "' not available for Automat level" );
    return mission->second.first;
}

// -----------------------------------------------------------------------------
// Name: MissionResolver::ResolveUnit
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
unsigned int MissionResolver::ResolveUnit( const std::string& name ) const
{
    T_Missions::const_iterator mission = missions_.find( name );
    if( mission == missions_.end() )
        throw std::runtime_error( "Unknown mission name '" + name + "'" );
    if( !mission->second.second )
        throw std::runtime_error( "Mission '" + name + "' not available for unit level" );
    return mission->second.second;
}
