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

using namespace dis;

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtensionFactory::DisExtensionFactory( UdpNetwork& network, const Time_ABC& time )
    : network_( network )
    , time_( time )
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
    EntityIdentifier id( 1, 1, id_ ); // site, application, id
    std::auto_ptr< DisExtension > extension( new DisExtension( time_, network_, entity, id ) );
    entity.Attach( *extension.release() );
    ++id_;
}
