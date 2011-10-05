// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRemoteHlaObjectFactory.h"
#include "NetnRemoteAggregate.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRemoteHlaObjectFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteHlaObjectFactory::NetnRemoteHlaObjectFactory( const RemoteHlaObjectFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteHlaObjectFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnRemoteHlaObjectFactory::~NetnRemoteHlaObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnRemoteHlaObjectFactory::CreateHlaObject
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< HlaObject_ABC > NetnRemoteHlaObjectFactory::CreateAggregate( const std::string& name, RemoteAgentListener_ABC& listener ) const
{
    std::auto_ptr< HlaObject_ABC > remote = factory_.CreateAggregate( name, listener );
    return std::auto_ptr< HlaObject_ABC >( new NetnRemoteAggregate( remote ) );
}
