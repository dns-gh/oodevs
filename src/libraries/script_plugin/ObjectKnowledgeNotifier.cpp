// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ObjectKnowledgeNotifier.h"
#include "dispatcher/ObjectKnowledge.h"
#include "clients_kernel/Controller.h"
#include "AgentEvents.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeNotifier constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
ObjectKnowledgeNotifier::ObjectKnowledgeNotifier( kernel::Controller& controller, const dispatcher::ObjectKnowledge& knowledge )
    : controller_( controller )
    , knowledge_ ( knowledge )
    , created_   ( false )
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeNotifier destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
ObjectKnowledgeNotifier::~ObjectKnowledgeNotifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeNotifier::DoUpdate
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void ObjectKnowledgeNotifier::DoUpdate( const sword::ObjectKnowledgeUpdate& /*message*/ )
{
    if( !created_ )
    {
        created_ = true;
        controller_.Update( events::ObjectKnowledgeCreated( knowledge_ ) );
    }
}
