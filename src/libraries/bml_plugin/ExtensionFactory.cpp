// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "ExtensionFactory.h"
#include "AgentExtension.h"
#include "AutomatExtension.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"

using namespace bml;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( Publisher_ABC& publisher, const ReportFactory& reportFactory )
    : publisher_( publisher )
    , reportFactory_( reportFactory )
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
    entity.Attach< BmlExtension_ABC >( *new AgentExtension( entity, publisher_, reportFactory_ ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Automat& entity )
{
    entity.Attach< BmlExtension_ABC >( *new AutomatExtension( entity, publisher_, reportFactory_ ) );
}
