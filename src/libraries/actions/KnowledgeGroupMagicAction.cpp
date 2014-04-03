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
#include "ActionTasker.h"
#include "ActionTiming.h"

#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

#include "ENT/ENT_Tr.h"

#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction constructor
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
KnowledgeGroupMagicAction::KnowledgeGroupMagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /* = true*/ )
    : Action_ABC( controller, &magic )
    , controller_( controller )
    , registered_( registered )
{
    Rename( ENT_Tr::ConvertFromKnowledgeMagicActionType( ENT_Tr::ConvertToKnowledgeMagicActionType( magic.GetName() ) ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction destructor
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
KnowledgeGroupMagicAction::~KnowledgeGroupMagicAction()
{
    if( registered_ )
        controller_.Delete( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction::Serialize
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType()->GetName() );
    xos << xml::attribute( "type", "magicknowledge" );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction::Polish
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupMagicAction::Publish
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupMagicAction::Publish( Publisher_ABC& publisher, int context ) const
{
    sword::KnowledgeMagicAction_Type type =
        ( sword::KnowledgeMagicAction_Type ) GetType()->GetId();
    simulation::KnowledgeMagicAction message;
    message().mutable_knowledge_group()->set_id( Get< ActionTasker >().GetId() );
    message().mutable_start_time()->set_data( Get< ActionTiming >().GetIsoTime() );
    message().set_type( type );
    CommitTo( *message().mutable_parameters() );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
}
