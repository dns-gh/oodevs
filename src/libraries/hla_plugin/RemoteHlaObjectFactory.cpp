// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteHlaObjectFactory.h"
#include "RemoteAggregate.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteHlaObjectFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteHlaObjectFactory::RemoteHlaObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHlaObjectFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
RemoteHlaObjectFactory::~RemoteHlaObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteHlaObjectFactory::CreateAggregate
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< HlaObject_ABC > RemoteHlaObjectFactory::CreateAggregate( const std::string& name, RemoteAgentListener_ABC& listener ) const
{
    return std::auto_ptr< HlaObject_ABC >( new RemoteAggregate( name, listener ) );
}
