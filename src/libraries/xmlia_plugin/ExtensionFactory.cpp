// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "ExtensionFactory.h"
#include "AgentExtension.h"
#include "dispatcher/Agent.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( Publisher_ABC& publisher, RapportManager& rapportManager, const Simulation& simulation, const dispatcher::Model& model )
    : publisher_( publisher )
    , rapportManager_( rapportManager )
    , simulation_( simulation )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
ExtensionFactory::~ExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Agent& entity )
{
    entity.Attach< XmliaExtension_ABC >( *new AgentExtension( entity, publisher_, rapportManager_, simulation_, model_ ) );
}
