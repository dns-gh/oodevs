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
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::ObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ObjectMagicAction::ObjectMagicAction( const kernel::Entity_ABC* object, const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /* = true*/ )
    : Action_ABC ( controller, magic )
    , objectId_  ( object ? object->GetId() : 0 )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::ObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ObjectMagicAction::ObjectMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC* object )
    : Action_ABC ( xis, controller, magic )
    , objectId_  ( object ? object->GetId() : 0 )
    , controller_( controller )
    , registered_( true )
{
    // NOTHING
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
    xos << xml::attribute( "id", GetType().GetName() )
        << xml::attribute( "type", "magicobject" );
    if( objectId_ != 0 )
        xos << xml::attribute( "target", objectId_ );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::Publish
// Created: JSR 2010-04-21
// -----------------------------------------------------------------------------
void ObjectMagicAction::Publish( Publisher_ABC& publisher, int ) const
{
    sword::ObjectMagicAction_Type type = ( sword::ObjectMagicAction_Type ) GetType().GetId();
    simulation::ObjectMagicAction message;
    message().mutable_object()->set_id( objectId_ );
    message().set_type( type );
    CommitTo( *message().mutable_parameters() );
    message.Send( publisher );
}
