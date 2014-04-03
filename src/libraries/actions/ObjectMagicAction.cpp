// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObjectMagicAction.h"
#include "ActionTasker.h"
#include "ActionTiming.h"

#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

#include "ENT/ENT_Tr.h"

#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::ObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ObjectMagicAction::ObjectMagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /* = true*/ )
    : Action_ABC ( controller, &magic )
    , controller_( controller )
    , registered_( registered )
{
    Rename( ENT_Tr::ConvertFromObjectMagicActionType( ENT_Tr::ConvertToObjectMagicActionType( magic.GetName() ) ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::~ObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ObjectMagicAction::~ObjectMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void ObjectMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void ObjectMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType()->GetName() )
        << xml::attribute( "type", "magicobject" )
        << xml::attribute( "target", Get< ActionTasker >().GetId() );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::Publish
// Created: JSR 2010-04-21
// -----------------------------------------------------------------------------
void ObjectMagicAction::Publish( Publisher_ABC& publisher, int context ) const
{
    sword::ObjectMagicAction_Type type = ( sword::ObjectMagicAction_Type ) GetType()->GetId();
    simulation::ObjectMagicAction message;
    message().mutable_object()->set_id( Get< ActionTasker >().GetId() );
    message().mutable_start_time()->set_data( Get< ActionTiming >().GetIsoTime() );
    message().set_type( type );
    CommitTo( *message().mutable_parameters() );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
}
