// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ExtensionFactory.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/Automat.h"
#include "AgentNotifier.h"
#include "AgentManipulator.h"
#include "KnowledgeNotifier.h"
#include "KnowledgeManipulator.h"
#include "AutomatNotifier.h"
#include "AutomatManipulator.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter )
    : controller_( controller )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory destructor
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
ExtensionFactory::~ExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: AGE 2008-06-13
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Agent& entity )
{
    entity.Attach( *new AgentNotifier   ( controller_, entity ) );
    entity.Attach( *new AgentManipulator( converter_, entity ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::AgentKnowledge& entity )
{
    entity.Attach( *new KnowledgeNotifier   ( controller_, entity ) );
    entity.Attach( *new KnowledgeManipulator( converter_, entity ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: SBO 2008-08-13
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Automat& entity )
{
    entity.Attach( *new AutomatNotifier   ( controller_, entity ) );
    entity.Attach( *new AutomatManipulator( entity ) );
}
