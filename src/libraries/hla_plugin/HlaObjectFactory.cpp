// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaObjectFactory.h"
#include "AggregateEntity.h"
#include "SurfaceVessel.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: HlaObjectFactory constructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
HlaObjectFactory::HlaObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaObjectFactory destructor
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
HlaObjectFactory::~HlaObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaObjectFactory::CreateAggregate
// Created: SLI 2011-07-26
// -----------------------------------------------------------------------------
std::auto_ptr< HlaObject_ABC > HlaObjectFactory::CreateAggregate( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
{
    return std::auto_ptr< HlaObject_ABC >( new AggregateEntity( agent, identifier, name, force, type ) );
}

// -----------------------------------------------------------------------------
// Name: HlaObjectFactory::CreateSurfaceVessel
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
std::auto_ptr< HlaObject_ABC > HlaObjectFactory::CreateSurfaceVessel( Agent_ABC& agent, const std::string& name, unsigned short identifier, rpr::ForceIdentifier force, const rpr::EntityType& type ) const
{
    return std::auto_ptr< HlaObject_ABC >( new SurfaceVessel( agent, identifier, name, force, type ) );
}
