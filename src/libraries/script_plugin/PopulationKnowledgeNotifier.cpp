// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "PopulationKnowledgeNotifier.h"
#include "dispatcher/PopulationKnowledge.h"
#include "clients_kernel/Controller.h"
#include "AgentEvents.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeNotifier constructor
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
PopulationKnowledgeNotifier::PopulationKnowledgeNotifier( kernel::Controller& controller, const dispatcher::PopulationKnowledge& knowledge )
    : controller_( controller )
    , knowledge_ ( knowledge )
    , created_   ( false )
{
   // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeNotifier destructor
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
PopulationKnowledgeNotifier::~PopulationKnowledgeNotifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeNotifier::DoUpdate
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
/*
void PopulationKnowledgeNotifier::DoUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& message )
{
    MT_LOG_INFO_MSG( "PopulationKnowledgeNotifier::DoUpdate" );
    if( !created_ )
    {
        created_ = true;
        controller_.Update( events::PopulationKnowledgeCreated( knowledge_ ) );
    }
}
*/


// -----------------------------------------------------------------------------
// Name: PopulationKnowledgeNotifier::DoUpdate
// Created: DSO 2010-09-16
// -----------------------------------------------------------------------------
void PopulationKnowledgeNotifier::DoUpdate( const sword::MsgPopulationConcentrationKnowledgeUpdate& /*message*/ )
{
    if( !created_ )
    {
        created_ = true;
        controller_.Update( events::PopulationKnowledgeCreated( knowledge_ ) );
    }
/*
    else
    {
        if( message.m.max_identification_levelPresent )
            controller_.Update( events::PerceptionChanged( knowledge_ ) );
        if( message.m.positionPresent )
            controller_.Update( events::KnowledgeMoved( knowledge_ ) );
    }
*/
}
