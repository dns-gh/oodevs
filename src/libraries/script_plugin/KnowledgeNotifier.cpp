// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "KnowledgeNotifier.h"
#include "AgentEvents.h"
#include "dispatcher/AgentKnowledge.h"
#include "clients_kernel/Controller.h"

using namespace MsgsSimToClient;

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: KnowledgeNotifier constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
KnowledgeNotifier::KnowledgeNotifier( kernel::Controller& controller, const dispatcher::AgentKnowledge& knowledge )
    : controller_( controller )
    , knowledge_ ( knowledge )
    , created_   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeNotifier destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
KnowledgeNotifier::~KnowledgeNotifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeNotifier::DoUpdate
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void KnowledgeNotifier::DoUpdate( const MsgUnitKnowledgeUpdate& message )
{
    if( !created_ )
    {
        created_ = true;
        controller_.Update( events::KnowledgeCreated( knowledge_ ) );
    }
    else
    {
        if( message.has_max_identification_level() )
            controller_.Update( events::PerceptionChanged( knowledge_ ) );
        if( message.has_position())
            controller_.Update( events::KnowledgeMoved( knowledge_ ) );
    }
}
