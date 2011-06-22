// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ObjectResolver.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ObjectResolver constructor
// Created: SLI 2011-06-22
// -----------------------------------------------------------------------------
ObjectResolver::ObjectResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectResolver destructor
// Created: SLI 2011-06-22
// -----------------------------------------------------------------------------
ObjectResolver::~ObjectResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectResolver::Register
// Created: SLI 2011-06-22
// -----------------------------------------------------------------------------
void ObjectResolver::Register( const std::string& identifier, const std::string& name )
{
    objects_[ identifier ] = name;
}

// -----------------------------------------------------------------------------
// Name: ObjectResolver::Unregister
// Created: SLI 2011-06-22
// -----------------------------------------------------------------------------
void ObjectResolver::Unregister( const std::string& identifier )
{
    objects_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: ObjectResolver::Resolve
// Created: SLI 2011-06-22
// -----------------------------------------------------------------------------
std::string ObjectResolver::Resolve( const std::string& identifier ) const
{
    T_Objects::const_iterator it = objects_.find( identifier );
    if( it == objects_.end() )
        throw std::runtime_error( "Unknown hla object: '" + identifier + "'" );
    return it->second;
}
