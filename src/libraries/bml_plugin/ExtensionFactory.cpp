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
#include "dispatcher/Agent.h"

using namespace bml;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( Publisher& publisher )
    : publisher_( publisher )
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
    entity.Attach< BmlExtension_ABC >( *new AgentExtension( entity, publisher_ ) );
}
