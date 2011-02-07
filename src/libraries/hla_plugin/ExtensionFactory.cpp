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
#include "AgentListener_ABC.h"
#include "AgentProxy.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( dispatcher::Model_ABC& model )
    : model_( model )
{
    model_.RegisterFactory( *this );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
ExtensionFactory::~ExtensionFactory()
{
    model_.UnregisterFactory( *this );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Agent& entity )
{
    agents_.push_back( T_Agent( new AgentProxy( entity ) ) );
    for( CIT_Listeners it = listeners_.begin(); it != listeners_.end(); ++it )
        (*it)->Created( *agents_.back() );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Register
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void ExtensionFactory::Register( AgentListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Unregister
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void ExtensionFactory::Unregister( AgentListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}
