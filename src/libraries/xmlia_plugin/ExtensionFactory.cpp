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
//#include "AgentExtension.h"
#include "AutomatExtension.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( Publisher_ABC& publisher, const ReportFactory& reportFactory, const Simulation& simulation, const dispatcher::Model& model )
    : publisher_( publisher )
    , reportFactory_( reportFactory )
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
    //entity.Attach< XmliaExtension_ABC >( *new AgentExtension( entity, publisher_, reportFactory_, simulation_, model_ ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Automat& entity )
{
    entity.Attach< XmliaExtension_ABC >( *new AutomatExtension( entity, publisher_, reportFactory_ ) );
}
