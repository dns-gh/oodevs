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
#include "ActionTasker.h"
#include "ActionTiming.h"

#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"

#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"

using namespace actions;

namespace
{
    QString GetName( bool engage )
    {
        return engage ? tools::translate( "EngageMagicAction", "Engage" ) : tools::translate( "EngageMagicAction", "Disengage" );
    }
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::EngageMagicAction
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
EngageMagicAction::EngageMagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, const bool engaged, bool registered /* = true*/ )
    : Action_ABC ( controller, &magic )
    , controller_( controller )
    , registered_( registered )
    , engaged_( engaged )
{
    Rename( ::GetName( engaged ) );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::~EngageMagicAction
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
EngageMagicAction::~EngageMagicAction()
{
    if( registered_ )
        controller_.Delete( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::Polish
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
void EngageMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::Serialize
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
void EngageMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", "change_mode" );
    xos << xml::attribute( "engaged", engaged_ );
    xos << xml::attribute( "entity", Get< ActionTasker >().GetId() );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: EngageMagicAction::Publish
// Created: FDS 2010-11-22
// -----------------------------------------------------------------------------
void EngageMagicAction::Publish( Publisher_ABC& publisher, int context ) const
{
    simulation::SetAutomatMode message;
    message().mutable_automate()->set_id( Get< ActionTasker >().GetId() );
    message().mutable_start_time()->set_data( Get< ActionTiming >().GetIsoTime() );
    message().set_mode( engaged_ ? sword::engaged : sword::disengaged );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
}
