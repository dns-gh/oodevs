// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ExtensionFactory.h"
#include "dispatcher/AgentSymbols.h"
#include "dispatcher/AutomatSymbols.h"
#include "dispatcher/FormationSymbols.h"
#include "dispatcher/SideSymbols.h"
#include "dispatcher/ObjectSymbols.h"
#include "dispatcher/AgentKnowledgeSymbols.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Side.h"
#include "dispatcher/Object.h"
#include "dispatcher/AgentKnowledge.h"

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory destructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
ExtensionFactory::~ExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Agent& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::AgentSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Automat& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::AutomatSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Formation& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::FormationSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Side& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::SideSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-15
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Object& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::ObjectSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::AgentKnowledge& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::AgentKnowledgeSymbols( entity ) );
}
