// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentHierarchiesCommunication.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Simulationsenders.h"

// -----------------------------------------------------------------------------
// Name: AgentHierarchiesCommunication
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
AgentHierarchiesCommunication::AgentHierarchiesCommunication( kernel::Controller& controller, kernel::Agent_ABC& holder,
                               const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                               const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver )
:AgentHierarchies< kernel::CommunicationHierarchies >::AgentHierarchies(controller, holder, automatResolver)
    ,jammed_ ( false )
    ,radioEmitterDisabled_ ( false )
    ,radioReceiverDisabled_ ( false )
    , superior_ ( 0 )
    ,groupResolver_ ( groupResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ~AgentHierarchiesCommunication
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
AgentHierarchiesCommunication::~AgentHierarchiesCommunication()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DoUpdate
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
void AgentHierarchiesCommunication::DoUpdate( const MsgsSimToClient::MsgUnitCreation& message )
{
    AgentHierarchies< kernel::CommunicationHierarchies >::DoUpdate( message );
    superior_ = &GetAutomatResolver().Get( message.oid_automate() );
}

// -----------------------------------------------------------------------------
// Name: DoUpdate
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
void AgentHierarchiesCommunication::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message )
{
    if( message.has_communications() && message.communications().has_jammed() )
    {
        jammed_ = message.communications().jammed() ;
        radioEmitterDisabled_ = message.has_radio_emitter_disabled() && message.radio_emitter_disabled();
        radioReceiverDisabled_ = message.has_radio_receiver_disabled() && message.radio_receiver_disabled();
        if( jammed_ || radioReceiverDisabled_ || radioEmitterDisabled_ )
            UpdateSuperior( groupResolver_.Get( message.communications().knowledge_group() )  );
        else
            UpdateSuperior( *superior_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DoUpdate
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
void AgentHierarchiesCommunication::DoUpdate( const Common::MsgUnitChangeSuperior& message )
{
    if( jammed_ || radioEmitterDisabled_ )
        superior_ = & GetAutomatResolver().Get( message.oid_automate() );
    else
        UpdateSuperior( GetAutomatResolver().Get( message.oid_automate() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchiesCommunication::CanCommunicate
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
bool AgentHierarchiesCommunication::CanCommunicate() const
{
    return !jammed_ && !radioReceiverDisabled_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchiesCommunication::IsJammed
// Created: HBD 2010-07-30
// -----------------------------------------------------------------------------
bool AgentHierarchiesCommunication::IsJammed() const
{
    return false;
}
