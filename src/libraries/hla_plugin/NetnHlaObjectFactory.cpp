// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnHlaObjectFactory.h"
#include "NetnAggregate.h"
#include "NetnSurfaceVessel.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnHlaObjectFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnHlaObjectFactory::NetnHlaObjectFactory( const HlaObjectFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnHlaObjectFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
NetnHlaObjectFactory::~NetnHlaObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnHlaObjectFactory::CreateAggregate
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< HlaObject_ABC > NetnHlaObjectFactory::CreateAggregate( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
{
    std::auto_ptr< HlaObject_ABC > aggregate = factory_.CreateAggregate( agent, name, identifier, force, type );
    return std::auto_ptr< HlaObject_ABC >( new NetnAggregate( aggregate, agent, name, identifier ) );
}

// -----------------------------------------------------------------------------
// Name: NetnHlaObjectFactory::CreateSurfaceVessel
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
std::auto_ptr< HlaObject_ABC > NetnHlaObjectFactory::CreateSurfaceVessel( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
{
    std::auto_ptr< HlaObject_ABC > aggregate = factory_.CreateSurfaceVessel( agent, name, identifier, force, type );
    return std::auto_ptr< HlaObject_ABC >( new NetnSurfaceVessel( aggregate, name, identifier ) );
}
