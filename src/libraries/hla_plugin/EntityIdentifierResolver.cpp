// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASAGroup
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "EntityIdentifierResolver.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver constructor
// Created: AHC 2012-04-23
// -----------------------------------------------------------------------------
EntityIdentifierResolver::EntityIdentifierResolver( unsigned short siteId, unsigned short applicationId )
    : siteId_( siteId )
    , applicationId_( applicationId )
    , sequenceId_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver destructor
// Created: AHC 2012-04-23
// -----------------------------------------------------------------------------
EntityIdentifierResolver::~EntityIdentifierResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver::Register
// Created: AHC 2012-04-23
// -----------------------------------------------------------------------------
void EntityIdentifierResolver::Register( const rpr::EntityIdentifier& identifier, const std::string& name )
{
    ids_.left.insert( T_IdMap::left_value_type( name, identifier ) );
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver::Unregister
// Created: AHC 2012-04-23
// -----------------------------------------------------------------------------
void EntityIdentifierResolver::Unregister( const rpr::EntityIdentifier& identifier )
{
    T_IdMap::right_iterator it( ids_.right.find( identifier ) );
    if( ids_.right.end() != it )
        ids_.right.erase( it );
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver::Resolve
// Created: AHC 2012-04-23
// -----------------------------------------------------------------------------
const std::string& EntityIdentifierResolver::Resolve( const rpr::EntityIdentifier& identifier ) const
{
    static const std::string DEFVAL("unknown");
    T_IdMap::right_const_iterator it( ids_.right.find( identifier ) );
    if( ids_.right.end() != it )
        return it->second;
    return DEFVAL;
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver::Resolve
// Created: AHC 2012-04-23
// -----------------------------------------------------------------------------
const rpr::EntityIdentifier& EntityIdentifierResolver::Resolve( const std::string& name ) const
{
    static const rpr::EntityIdentifier DEFVAL;
    T_IdMap::left_const_iterator it( ids_.left.find( name ) );
    if( ids_.left.end() != it )
        return it->second;
    return DEFVAL;
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver::Unregister
// Created: AHC 2012-04-23
// -----------------------------------------------------------------------------
void EntityIdentifierResolver::Unregister( const std::string& name )
{
    T_IdMap::left_iterator it( ids_.left.find( name ) );
    if( ids_.left.end() != it )
        ids_.left.erase( it );
}

// -----------------------------------------------------------------------------
// Name: EntityIdentifierResolver::Create
// Created: AHC 2012-10-23
// -----------------------------------------------------------------------------
void EntityIdentifierResolver::Create( const std::string& name, rpr::EntityIdentifier& id )
{
    ++sequenceId_;
    id = rpr::EntityIdentifier( siteId_, applicationId_, sequenceId_ );
    Register( id, name );
}
