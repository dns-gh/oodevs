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

#include <boost/lexical_cast.hpp>

#include <sstream>
#include <set>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: SideResolver constructor
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
SideResolver::SideResolver( dispatcher::Model_ABC& dynamicModel, dispatcher::Logger_ABC& logger )
    : logger_( logger )
{
    std::set< rpr::ForceIdentifier > friends;
    std::set< rpr::ForceIdentifier > neutrals;
    std::set< rpr::ForceIdentifier > enemies;
    for( unsigned int i=0; i<10; ++i)
    {
        friends.insert( static_cast< rpr::ForceIdentifier >( i*3 + 1 ) );
        enemies.insert( static_cast< rpr::ForceIdentifier >( i*3 + 2 ) );
        neutrals.insert( static_cast< rpr::ForceIdentifier >( i*3 + 3 ) );
    }

    const tools::Resolver_ABC< dispatcher::Team_ABC >& sides( dynamicModel.Sides() );
    for( tools::Iterator< const dispatcher::Team_ABC& > it = sides.CreateIterator(); it.HasMoreElements(); )
    {
        const dispatcher::Team_ABC& team = it.NextElement();
        std::string forceExt;
        if( team.GetExtension( "HlaForceIdentifier", forceExt ) )
        {
            unsigned long id = boost::lexical_cast< unsigned long >( forceExt );
            rpr::ForceIdentifier fId = static_cast< rpr::ForceIdentifier >( id );
            teams_.insert( T_Teams::value_type( team.GetId(), fId ) );
            friends.erase( fId ) ;
            enemies.erase( fId ) ;
            neutrals.erase( fId ) ;
            continue;
        }

        if( team.GetId() )
        {
            rpr::ForceIdentifier fId;
            const kernel::Karma& karma( team.GetKarma() );
            if( karma == kernel::Karma::friend_ )
            {
                if( friends.empty() )
                {
                    logger_.LogError( std::string( "Too many friend teams in Sword exercice, using 'Friendly' for team ") + team.GetName().toStdString() );
                    fId = rpr::Friendly;
                }
                else
                {
                    fId = *friends.begin();
                    friends.erase( friends.begin() );
                }
            }
            else if( karma == kernel::Karma::neutral_ )
            {
                if( friends.empty() )
                {
                    logger_.LogError( std::string( "Too many neutral teams in Sword exercice, using 'Neutral' for team " ) + team.GetName().toStdString() );
                    fId = rpr::Neutral;
                }
                else
                {
                    fId = *neutrals.begin();
                    neutrals.erase( neutrals.begin() );
                }
            }
            else if( karma == kernel::Karma::enemy_ )
            {
                if( friends.empty() )
                {
                    logger_.LogError( std::string( "Too many enemy teams in Sword exercice, using 'Opposing' for team ") + team.GetName().toStdString() );
                    fId = rpr::Opposing;
                }
                else
                {
                    fId = *enemies.begin();
                    enemies.erase( enemies.begin() );
                }
            }
            teams_.insert( T_Teams::value_type( team.GetId(), fId ) );
        }
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
