// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SideResolver.h"

#include "dispatcher/Model_ABC.h"
#include "clients_kernel/Karma.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include <sstream>

using namespace plugins::hla;

namespace
{
    rpr::ForceIdentifier GetForce( const kernel::Karma& karma )
    {
        // FIXME
        if( karma == kernel::Karma::friend_ )
            return rpr::Friendly;
        if( karma == kernel::Karma::enemy_ )
            return rpr::Opposing;
        if( karma == kernel::Karma::neutral_ )
            return rpr::Neutral;
        return rpr::Other;
    }
}

// -----------------------------------------------------------------------------
// Name: SideResolver constructor
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
SideResolver::SideResolver( dispatcher::Model_ABC& dynamicModel, dispatcher::Logger_ABC& logger )
    : logger_( logger )
{
    const tools::Resolver_ABC< dispatcher::Team_ABC >& sides( dynamicModel.Sides() );
    for( tools::Iterator< const dispatcher::Team_ABC& > it = sides.CreateIterator(); it.HasMoreElements(); )
    {
        const dispatcher::Team_ABC& team = it.NextElement();
        if( team.GetId() )
            teams_.insert( T_Teams::value_type( team.GetId(), GetForce( team.GetKarma() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SideResolver constructor
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
SideResolver::~SideResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideResolver constructor
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
unsigned long SideResolver::ResolveTeam( rpr::ForceIdentifier f ) const
{
    CIT_Forces it( teams_.right.find( f ) );
    if( teams_.right.end() == it )
    {
        if( teams_.empty() )
           throw std::runtime_error( "Cannot find team for force " );
        it = teams_.right.begin();
        std::stringstream ss;
        ss << "SideResolver: Using default team for force " << f;
        logger_.LogError( ss.str() );
    }
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: SideResolver constructor
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
rpr::ForceIdentifier SideResolver::ResolveForce( unsigned long t ) const
{
    CIT_Teams it( teams_.left.find( t ) );
    if( it == teams_.left.end() )
        throw std::runtime_error( "Cannot find force for team " );
    return it->second;
}
