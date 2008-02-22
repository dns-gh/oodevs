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
#include "AgentExtension.h"
#include "AggregateEntityClass.h"
#include "dispatcher/Agent.h"
#include "hla/ObjectIdentifier.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( AggregateEntityClass& agentClass )
    : agentClass_( agentClass )
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
    std::auto_ptr< AgentExtension > extension( new AgentExtension( entity ) );
    agentClass_.Register( *extension );
    entity.Attach( *extension.release() );
}
