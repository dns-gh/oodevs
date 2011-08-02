// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "KnowledgeGroupMagicAction.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction constructor
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
KnowledgeGroupMagicAction::KnowledgeGroupMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /* = true*/ )
    : ActionWithTarget_ABC ( controller, magic, entity )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction constructor
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
KnowledgeGroupMagicAction::KnowledgeGroupMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity )
    : ActionWithTarget_ABC( xis, controller, magic, entity )
    , controller_         ( controller )
    , registered_         ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction destructor
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
KnowledgeGroupMagicAction::~KnowledgeGroupMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction::Serialize
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType().GetName() );
    xos << xml::attribute( "type", "magicknowledge" );
    ActionWithTarget_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction::Polish
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction::Publish
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicAction::Publish( Publisher_ABC& publisher, int ) const
{
    sword::KnowledgeMagicAction_Type type =
        ( sword::KnowledgeMagicAction_Type ) GetType().GetId();
    simulation::KnowledgeMagicAction message;
    message().mutable_knowledge_group()->set_id( GetEntity().GetId() );
    message().set_type( type );
    CommitTo( *message().mutable_parameters() );
    message.Send( publisher );
    message().Clear();
}
