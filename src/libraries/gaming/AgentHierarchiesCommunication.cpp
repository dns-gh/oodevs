// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentHierarchiesCommunication.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_gui/EntityHierarchies.h"
#include "clients_gui/PropertiesDictionary.h"
#include "protocol/SimulationSenders.h"

// -----------------------------------------------------------------------------
// Name: AgentHierarchiesCommunication
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
AgentHierarchiesCommunication::AgentHierarchiesCommunication( kernel::Controllers& controllers, kernel::Agent_ABC& holder,
                               const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                               const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver )
:AgentHierarchies< kernel::CommunicationHierarchies >::AgentHierarchies( controllers.controller_, holder, automatResolver)
    ,jammed_ ( false )
    ,radioEmitterDisabled_ ( false )
    ,radioReceiverDisabled_ ( false )
    ,groupResolver_ ( groupResolver )
    ,controllers_ ( controllers )
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
void AgentHierarchiesCommunication::DoUpdate( const sword::UnitCreation& message )
{
    AgentHierarchies< kernel::CommunicationHierarchies >::DoUpdate( message );
    superior_ = &GetAutomatResolver().Get( message.automat().id() );
}

// -----------------------------------------------------------------------------
// Name: DoUpdate
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
void AgentHierarchiesCommunication::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_communications() && message.communications().has_jammed() )
    {
        jammed_ = message.communications().jammed() ;
        radioEmitterDisabled_ = message.has_radio_emitter_disabled() && message.radio_emitter_disabled();
        radioReceiverDisabled_ = message.has_radio_receiver_disabled() && message.radio_receiver_disabled();
        bool wasSelected = controllers_.actions_.IsSelected( &entity_ );
        if( jammed_ || radioReceiverDisabled_ || radioEmitterDisabled_ )
            UpdateSuperior( groupResolver_.Get( message.communications().knowledge_group().id() )  );
        else
            UpdateSuperior( const_cast< kernel::Entity_ABC& >( *superior_ ) );
        if( wasSelected )
          entity_.Select( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DoUpdate
// Created: FDS 2010-03-29
// -----------------------------------------------------------------------------
void AgentHierarchiesCommunication::DoUpdate( const sword::UnitChangeSuperior& message )
{
    if( jammed_ || radioEmitterDisabled_ )
        superior_ = & GetAutomatResolver().Get( message.parent().id() );
    else
        UpdateSuperior( GetAutomatResolver().Get( message.parent().id() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchiesCommunication::CanCommunicate
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
bool AgentHierarchiesCommunication::CanCommunicate() const
{
    return !jammed_ && !radioReceiverDisabled_ && !radioEmitterDisabled_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchiesCommunication::CanReceive
// Created: LDC 2013-09-27
// -----------------------------------------------------------------------------
bool AgentHierarchiesCommunication::CanReceive() const
{
    return !jammed_ && !radioReceiverDisabled_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchiesCommunication::IsJammed
// Created: HBD 2010-07-30
// -----------------------------------------------------------------------------
bool AgentHierarchiesCommunication::IsJammed() const
{
    return jammed_;
}
