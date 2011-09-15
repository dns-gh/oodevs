// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe

#include "EntityTypeResolver.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace rpr;

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver constructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
EntityTypeResolver::EntityTypeResolver( xml::xisubstream xis )
{
    std::string defaultType;
    std::string threshold;
    xis >> xml::start( "dis-mapping" )
            >> xml::attribute( "default-type", defaultType )
            >> xml::attribute( "default-name", defaultName_ )
            >> xml::attribute( "threshold", threshold )
            >> xml::list( "entry", *this, &EntityTypeResolver::ReadEntry );
    defaultType_ = rpr::EntityType( defaultType );
    threshold_ = rpr::EntityType( threshold );
}

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver destructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
EntityTypeResolver::~EntityTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver::Find
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
rpr::EntityType EntityTypeResolver::Find( const std::string& name ) const
{
    rpr::EntityType& type = resolvedTypes_[ name ];
    if( type == rpr::EntityType() )
    {
        const rpr::EntityType* resolved = types_.Find( name );
        type = resolved ? *resolved : defaultType_;
    }
    return type;
}

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver::Resolve
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
std::string EntityTypeResolver::Resolve( const rpr::EntityType& type ) const
{
    rpr::EntityType distance = type - rpr::EntityType( defaultType_ );
    std::string result = defaultName_;
    BOOST_FOREACH( const T_Name& name, names_ )
    {
        rpr::EntityType current = type - name.type_;
        if( current <= distance )
        {
            distance = current;
            result = name.name_;
        }
    }
    if( distance <= threshold_ )
        return result;
    return defaultName_;
}

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver::ReadEntry
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
void EntityTypeResolver::ReadEntry( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name" );
    const rpr::EntityType type( xis.attribute< std::string >( "dis" ) );
    types_.Add( name, type );
    names_.push_back( T_Name( type, name ) );
}
