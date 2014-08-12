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
#include <boost/make_shared.hpp>

#include <sstream>
#include <set>

using namespace plugins::hla;

namespace
{
    struct Helper : boost::noncopyable
    {
        Helper( std::set< rpr::ForceIdentifier >& ids, rpr::ForceIdentifier defVal )
            : rprIds( ids )
            , defaultValue( defVal )
        {
            // NOTHING
        }
        bool apply( rpr::ForceIdentifier& fId )
        {
            bool retval (true );
            if( rprIds.empty() )
            {
                fId = defaultValue;
                retval = false;
            }
            else
            {
                fId = *rprIds.begin();
                rprIds.erase( rprIds.begin() );
            }
            return retval;
        }
        std::set< rpr::ForceIdentifier >& rprIds;
        rpr::ForceIdentifier defaultValue;
    };
}

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
    std::map< kernel::Karma, boost::shared_ptr< Helper > > helpers;
    helpers[ kernel::Karma::friend_ ] = boost::make_shared< Helper >( friends, rpr::Friendly );
    helpers[ kernel::Karma::neutral_ ] = boost::make_shared< Helper >( neutrals, rpr::Neutral );
    helpers[ kernel::Karma::enemy_ ] = boost::make_shared< Helper >( enemies, rpr::Opposing );

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
            std::map< kernel::Karma, boost::shared_ptr< Helper > >::iterator itH( helpers.find( karma ) );
            if( helpers.end() == itH )
            {
                logger_.LogError( std::string( "Unknown karma " ) + karma.GetId() + " for team " + team.GetName().toStdString() );
                continue;
            }
            if( !itH->second->apply( fId ) )
            {
                logger_.LogError( std::string( "Too many ") + itH->first.GetId()  + (" teams in Sword exercice, using default for team ") + team.GetName().toStdString() );
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
           throw MASA_EXCEPTION( "Cannot find team for force " );
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
        throw MASA_EXCEPTION( "Cannot find force for team " );
    return it->second;
}
