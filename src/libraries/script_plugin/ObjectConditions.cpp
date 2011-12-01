// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ObjectConditions.h"
#include "SimpleEntityCondition.h"
#include "AgentEvents.h"
#include <directia/brain/Brain.h>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ObjectConditions constructor
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
ObjectConditions::ObjectConditions( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectConditions destructor
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
ObjectConditions::~ObjectConditions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectConditions::RegisterIn
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
void ObjectConditions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "events.objects"] = this;
    brain.Register( "ObjectDestroyed", &ObjectConditions::ObjectDestroyed );
    brain.Register( "ObjectKnowledgeCreated", &ObjectConditions::ObjectKnowledgeCreated );
}

namespace directia
{
    void UsedByDIA    ( dispatcher::Object_ABC* ) {}
    void ReleasedByDIA( dispatcher::Object_ABC* ) {}
    void UsedByDIA    ( ObjectConditions* ) {}
    void ReleasedByDIA( ObjectConditions* ) {}
}

// -----------------------------------------------------------------------------
// Name: ObjectConditions::ObjectDestroyed
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ObjectConditions::ObjectDestroyed()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::ObjectDestroyed >( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectConditions::ObjectDestroyed
// Created: DSO 2010-07-27
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ObjectConditions::ObjectKnowledgeCreated()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::ObjectKnowledgeCreated >( controller_ ) );
}

