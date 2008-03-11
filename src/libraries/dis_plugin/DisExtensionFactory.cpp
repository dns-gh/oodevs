// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisExtensionFactory.h"
#include "DisExtension.h"
#include "dispatcher/Agent.h"
#include <xeumeuleu/xml.h>

using namespace dis;

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtensionFactory::DisExtensionFactory( UdpNetwork& network, const Time_ABC& time, xml::xistream& xis )
    : network_( network )
    , time_( time )
    , site_( xml::attribute< unsigned short >( xis, "site" ) )
    , application_( xml::attribute< unsigned short >( xis, "application" ) )
    , exercise_( (unsigned char)xml::attribute< unsigned short >( xis, "exercise" ) )
    , id_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtensionFactory::~DisExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory::Create
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void DisExtensionFactory::Create( dispatcher::Agent& entity )
{
    EntityIdentifier id( site_, application_, id_ ); // site, application, id
    std::auto_ptr< DisExtension > extension( new DisExtension( time_, network_, entity, id, exercise_ ) );
    entity.Attach( *extension.release() );
    ++id_;
}
