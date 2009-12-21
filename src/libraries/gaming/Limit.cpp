// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Limit.h"
#include "game_asn/MessengerSenders.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Limit::Limit( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::translate( "Limit", "Limit" ), 0, publisher, converter )
    , controller_( controller )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
Limit::Limit( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const ASN1T_MsgLimitCreation& asnMsg )
    : TacticalLine_ABC( asnMsg.tactical_line.name, asnMsg.oid, publisher, converter )
    , controller_( controller )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

//-----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::~Limit()
{
    controller_.Delete( *(kernel::TacticalLine_ABC*)this );
    controller_.Delete( *(kernel::Entity_ABC*)this );
}

//-----------------------------------------------------------------------------
// Name: Limit::UpdateToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void Limit::UpdateToSim( E_State state )
{
    switch( state )
    {
    case eStateCreated:
        {
            plugins::messenger::LimitCreationRequest message;
            message().name = GetName();
            WriteGeometry ( message().geometry );
            WriteDiffusion( message().diffusion );
            Send( message );
            delete[] message().geometry.coordinates.elem;
        }
        break;
    case eStateModified:
        {
            plugins::messenger::LimitUpdateRequest message;
            message().oid = GetId();
            message().tactical_line.name  = GetName();
            WriteGeometry ( message().tactical_line.geometry );
            WriteDiffusion( message().tactical_line.diffusion );
            Send( message );
            delete[] message().tactical_line.geometry.coordinates.elem;
        }
        break;
    case eStateDeleted:
        plugins::messenger::LimitDestructionRequest asnMsg;
        asnMsg() = GetId();
        Send( asnMsg );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Limit::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::Select( kernel::ActionController& actions ) const
{
    actions.Select( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this );
}

// -----------------------------------------------------------------------------
// Name: Limit::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::ContextMenu( kernel::ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this, point );
}

// -----------------------------------------------------------------------------
// Name: Limit::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Limit::Activate( kernel::ActionController& actions ) const
{
    actions.Activate( *this, *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Limit::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Limit::IsLimit() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Limit::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Limit::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
