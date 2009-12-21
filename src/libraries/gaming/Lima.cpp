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
#include "clients_kernel/ModelVisitor_ABC.h"
#include "game_asn/MessengerSenders.h"
#include "Tools.h"

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
Lima::Lima( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const ASN1T_MsgLimaCreation& asnMsg )
    : TacticalLine_ABC( asnMsg.tactical_line.name, asnMsg.oid, publisher, converter )
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
            message().name = GetName();
            WriteGeometry ( message().geometry );
            WriteDiffusion( message().diffusion );
            Send( message );
            delete[] message().geometry.coordinates.elem;
        }
        break;
    case eStateModified:
        {
            plugins::messenger::LimaUpdateRequest message;
            message().oid = GetId();
            message().tactical_line.name= GetName();
            WriteGeometry ( message().tactical_line.geometry );
            WriteDiffusion( message().tactical_line.diffusion );
            Send( message );
            delete[] message().tactical_line.geometry.coordinates.elem;
        }
        break;
    case eStateDeleted:
        plugins::messenger::LimaDestructionRequest asnMsg;
        asnMsg() = GetId();
        Send( asnMsg );
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
