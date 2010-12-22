// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "UrbanMagicAction.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::UrbanMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UrbanMagicAction::UrbanMagicAction( const kernel::Entity_ABC& object, kernel::Controller& controller, int structuralState, bool registered /*= true*/ )
    : ActionWithTarget_ABC ( controller, kernel::MagicActionType( "urban" ), object ) // $$$$ _RC_ SLG 2010-12-22: Creation d'un MagicActionType fictif
    , controller_( controller )
    , registered_( registered )
    , structuralState_( structuralState )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::UrbanMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UrbanMagicAction::UrbanMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::Entity_ABC& object )
    : ActionWithTarget_ABC( xis, controller, kernel::MagicActionType( "urban" ), object )  // $$$$ _RC_ SLG 2010-12-22: Creation d'un MagicActionType fictif
    , controller_( controller )
    , registered_( true )
    , structuralState_( 100 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::~UrbanMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UrbanMagicAction::~UrbanMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UrbanMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UrbanMagicAction::Serialize( xml::xostream& xos ) const
{
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::Publish
// Created: JSR 2010-04-21
// -----------------------------------------------------------------------------
void UrbanMagicAction::Publish( Publisher_ABC& publisher, int ) const
{
    simulation::UrbanMagicAction message;
    message().mutable_id()->set_id( GetEntity().GetId() );
    message().set_structural_state( structuralState_ );
    message.Send( publisher );
    message().Clear();
}
