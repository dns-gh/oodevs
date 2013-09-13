// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

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
bool EntityTypeResolver::Find( const std::string& name, rpr::EntityType& result ) const
{
    bool retval = true;
    result = resolvedTypes_[ name ];
    if( result == rpr::EntityType() )
    {
        const rpr::EntityType* resolved = types_.Find( name );
        retval = ( resolved != 0 );
        result = retval ? *resolved : defaultType_;
    }
    return retval;
}

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver::Resolve
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
bool EntityTypeResolver::Resolve( const rpr::EntityType& type, std::string& result ) const
{
    rpr::EntityType distance = type - rpr::EntityType( defaultType_ );
    result = defaultName_;

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
        return true;
    result = defaultName_;
    return false;
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
