// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "KnowledgeNotifier.h"
#include "dispatcher/AgentKnowledge.h"
#include "clients_kernel/Controller.h"
#include "AgentEvents.h"

using namespace script;

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
void KnowledgeNotifier::DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message )
{
    if( !created_ )
    {
        created_ = true;
        controller_.Update( events::KnowledgeCreated( knowledge_ ) );
    }
    else 
    {
        if( message.m.max_identification_levelPresent )
            controller_.Update( events::PerceptionChanged( knowledge_ ) );
        if( message.m.positionPresent )
            controller_.Update( events::KnowledgeMoved( knowledge_ ) );
    }
}
