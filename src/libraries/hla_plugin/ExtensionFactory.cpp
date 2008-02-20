// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ExtensionFactory.h"
#include "FederateFacade.h"
#include "AgentExtension.h"
#include "dispatcher/Agent.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( FederateFacade& federate )
    : federate_( federate )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
ExtensionFactory::~ExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Agent& entity )
{
    entity.Attach( *new AgentExtension( entity, federate_ ) );
}
