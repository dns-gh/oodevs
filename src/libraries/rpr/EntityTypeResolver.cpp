// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "EntityTypeResolver.h"
#include <xeumeuleu/xml.hpp>

using namespace rpr;

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver constructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
EntityTypeResolver::EntityTypeResolver( xml::xisubstream xis )
{
    std::string defaultType;
    xis >> xml::start( "dis-mapping" )
            >> xml::attribute( "default", defaultType )
            >> xml::list( "entry", *this, &EntityTypeResolver::ReadEntry );
    default_ = rpr::EntityType( defaultType );
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
    rpr::EntityType& type = resolved_[ name ];
    if( type == rpr::EntityType() )
    {
        const rpr::EntityType* resolved = types_.Find( name );
        type = resolved ? *resolved : default_;
    }
    return type;
}

// -----------------------------------------------------------------------------
// Name: EntityTypeResolver::ReadEntry
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
void EntityTypeResolver::ReadEntry( xml::xistream& xis )
{
    types_.Add( xis.attribute< std::string >( "name" ), rpr::EntityType( xis.attribute< std::string >( "dis" ) ) );
}
