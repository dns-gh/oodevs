// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisTypeResolver.h"
#include "clients_kernel/ComponentType.h"
#include <xeumeuleu/xml.h>

using namespace dis;

// -----------------------------------------------------------------------------
// Name: DisTypeResolver constructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
DisTypeResolver::DisTypeResolver( const std::string& mappingFile )
{
    xml::xifstream xis( mappingFile );
    std::string defaultType;
    xis >> xml::start( "dis-mapping" )
            >> xml::attribute( "default", defaultType )
            >> xml::list( "entry", *this, &DisTypeResolver::ReadEntry )
        >> xml::end();
    default_ = EntityType( defaultType );
}

// -----------------------------------------------------------------------------
// Name: DisTypeResolver destructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
DisTypeResolver::~DisTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisTypeResolver::Find
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
EntityType DisTypeResolver::Find( const kernel::ComponentType& component ) const
{
    EntityType& type = resolved_[ &component ];
    if( type == EntityType() )
    {
        const EntityType* resolved = types_.Find( component.GetName() );
        type = resolved ? *resolved : default_;
    }
    return type;
}

// -----------------------------------------------------------------------------
// Name: DisTypeResolver::ReadEntry
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
void DisTypeResolver::ReadEntry( xml::xistream& xis )
{
    types_.Add( xml::attribute< std::string >( xis, "name" ), EntityType( xml::attribute< std::string >( xis, "dis" ) ) );
}
