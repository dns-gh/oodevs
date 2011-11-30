// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Lima.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "protocol/MessengerSenders.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::translate( "Lima", "Phase line" ), 0, publisher, converter )
    , controller_     ( controller )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const sword::PhaseLineCreation& message )
    : TacticalLine_ABC( QString( message.tactical_line().name().c_str() ) , message.id().id(), publisher, converter )
    , controller_     ( controller )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    controller_.Delete( *(kernel::TacticalLine_ABC*)this );
    controller_.Delete( *(kernel::Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima::UpdateToSim
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void Lima::UpdateToSim( E_State state )
{
    switch( state )
    {
    case eStateCreated:
        {
            plugins::messenger::PhaseLineCreationRequest message;
            message().mutable_tactical_line()->set_name( GetName() );
            WriteGeometry ( *message().mutable_tactical_line()->mutable_geometry() );
            WriteDiffusion( *message().mutable_tactical_line()->mutable_diffusion() );
            Send( message );
        }
        break;
    case eStateModified:
        {
            plugins::messenger::PhaseLineUpdateRequest message;
            message().mutable_id()->set_id( GetId() );
            message().mutable_tactical_line()->set_name( GetName() );
            WriteGeometry ( *message().mutable_tactical_line()->mutable_geometry() );
            WriteDiffusion( *message().mutable_tactical_line()->mutable_diffusion() );
            Send( message );
        }
        break;
    case eStateDeleted:
        plugins::messenger::PhaseLineDestructionRequest message;
        message().mutable_id()->set_id( GetId() );
        Send( message );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Lima::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::Select( kernel::ActionController& actions ) const
{
    actions.Select( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this );
}

// -----------------------------------------------------------------------------
// Name: Lima::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::ContextMenu( kernel::ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this, point );
}

// -----------------------------------------------------------------------------
// Name: Lima::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Lima::Activate( kernel::ActionController& actions ) const
{
    actions.Activate( *this, *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Lima::IsLimit() const
{
    return false;
}
