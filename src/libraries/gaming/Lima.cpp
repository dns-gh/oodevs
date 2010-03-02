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
#include "Tools.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/MessengerSenders.h"
#include "protocol/Protocol.h"
#include "protocol/Publisher_ABC.h"

using namespace Common;

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
Lima::Lima( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const MsgsMessengerToClient::MsgLimaCreation& message )
    : TacticalLine_ABC( QString( message.tactical_line().name().c_str() ) , message.oid(), publisher, converter )
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
            plugins::messenger::LimaCreationRequest message;
            message().mutable_tacticalline()->set_name( GetName() );
            WriteGeometry ( *message().mutable_tacticalline()->mutable_geometry() );
            WriteDiffusion( *message().mutable_tacticalline()->mutable_diffusion() );
            Send( message );
            message().mutable_tacticalline()->mutable_geometry()->mutable_coordinates()->mutable_elem()->Clear();
        }
        break;
    case eStateModified:
        {
            plugins::messenger::LimaUpdateRequest message;
            message().set_oid( GetId() );
            message().mutable_tactical_line()->set_name( GetName() );
            WriteGeometry ( *message().mutable_tactical_line()->mutable_geometry() );
            WriteDiffusion( *message().mutable_tactical_line()->mutable_diffusion() );
            Send( message );
            message().mutable_tactical_line()->mutable_geometry()->mutable_coordinates()->mutable_elem()->Clear();
        }
        break;
    case eStateDeleted:
        plugins::messenger::LimaDestructionRequest message;
        message().set_oid( GetId() );
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

// -----------------------------------------------------------------------------
// Name: Lima::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Lima::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
