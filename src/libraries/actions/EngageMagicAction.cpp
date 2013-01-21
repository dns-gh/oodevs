// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "EngageMagicAction.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::EngageMagicAction
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
EngageMagicAction::EngageMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, const QString& name, const bool engaged, bool registered /* = true*/ )
    : ActionWithTarget_ABC ( controller, magic, entity )
    , entity_( entity )
    , controller_( controller )
    , registered_( registered )
    , engaged_ ( engaged )
{
    Rename ( name );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::EngageMagicAction
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
EngageMagicAction::EngageMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity, const QString& name, const bool engaged )
    : ActionWithTarget_ABC( xis, controller, magic, entity )
    , entity_( entity )
    , controller_( controller )
    , registered_( true )
    , engaged_ ( engaged )
{
    Rename ( name );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::~EngageMagicAction
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
EngageMagicAction::~EngageMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::Polish
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
void EngageMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::Serialize
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
void EngageMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", "change_mode" );
    xos << xml::attribute( "engaged", engaged_ );
    xos << xml::attribute( "entity", entity_.GetId() );
    ActionWithTarget_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::Publish
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
void EngageMagicAction::Publish( Publisher_ABC& publisher, int ) const
{
    simulation::SetAutomatMode message;
    message().mutable_automate()->set_id( entity_.GetId() );
    message().set_mode( engaged_ ? sword::engaged : sword::disengaged );
    message.Send( publisher );
}
