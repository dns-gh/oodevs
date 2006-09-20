// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledges.h"
#include "AgentKnowledge.h"
#include "AgentKnowledgeFactory_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentKnowledges constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledges::AgentKnowledges( Controller& controller, const KnowledgeGroup_ABC& group, AgentKnowledgeFactory_ABC& factory )
    : controller_( controller )
    , group_( group )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledges::~AgentKnowledges()
{
    controller_.Delete( *this );
    Resolver< AgentKnowledge >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentKnowledges::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentKnowledges::DoUpdate( const ASN1T_MsgUnitKnowledgeCreation& message )
{
    if( ! Find( message.oid_connaissance ) )
    {
        AgentKnowledge* pAgentKnowledge = factory_.CreateAgentKnowledge( group_, message );
        Register( message.oid_connaissance, *pAgentKnowledge );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentKnowledges::DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate&      message )
{
    Get( message.oid_connaissance ).Update( message );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledges::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void AgentKnowledges::DoUpdate( const ASN1T_MsgUnitKnowledgeDestruction& message )
{
    delete Find( message.oid_connaissance );
    Remove( message.oid_connaissance );
    controller_.Update( *this );
}
