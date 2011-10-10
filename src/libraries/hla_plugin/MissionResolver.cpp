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

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: MissionResolver constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
MissionResolver::MissionResolver( const tools::Resolver_ABC< kernel::MissionType, unsigned long >& missions )
{
    for( tools::Iterator< const kernel::MissionType& > it = missions.CreateIterator(); it.HasMoreElements(); )
    {
        const kernel::MissionType& mission = it.NextElement();
        missions_[ mission.GetName() ] = mission.GetId();
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
// Name: MissionResolver::Resolve
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
unsigned int MissionResolver::Resolve( const std::string& name ) const
{
    T_Missions::const_iterator mission = missions_.find( name );
    if( mission == missions_.end() )
        throw std::runtime_error( "Unknown mission name '" + name + "'" );
    return mission->second;
}
