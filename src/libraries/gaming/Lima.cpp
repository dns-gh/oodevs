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
#include "MessengerMessages.h"
#include "Tools.h"
#include "TacticalLinePositions.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, Publisher_ABC& publisher )
    : TacticalLine_ABC( "Phase line", 0, publisher )
    , controller_     ( controller )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimaCreation& asnMsg )
    : TacticalLine_ABC( asnMsg.tactical_line.name, asnMsg.oid, publisher )
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
            messenger::LimaCreationRequest message;
            message().name = GetName();
            WriteGeometry ( message().geometry );
            WriteDiffusion( message().diffusion );
            Send( message );
            delete[] message().geometry.coordinates.elem;
        }
        break;
    case eStateModified:
        {
            messenger::LimaUpdateRequest message;
            message().oid = GetId();
            message().tactical_line.name= GetName();
            WriteGeometry ( message().tactical_line.geometry );
            WriteDiffusion( message().tactical_line.diffusion );
            Send( message );
            delete[] message().tactical_line.geometry.coordinates.elem;
        }
        break;
    case eStateDeleted:
        messenger::LimaDestructionRequest asnMsg;
        asnMsg() = GetId();
        Send( asnMsg );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Lima::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::Select( ActionController& actions ) const
{
    actions.Select( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this );
}

// -----------------------------------------------------------------------------
// Name: Lima::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::ContextMenu( ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this, point );
}

// -----------------------------------------------------------------------------
// Name: Lima::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Lima::Activate( ActionController& actions ) const
{
    actions.Activate( *this, *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima::CopyTo
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void Lima::CopyTo( ASN1T_Line& destination ) const
{
    WriteGeometry( destination ); // $$$$ SBO 2006-11-14: visitor/Serializer
}

// -----------------------------------------------------------------------------
// Name: Lima::CopyTo
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void Lima::CopyTo( kernel::Location_ABC& location ) const
{
    WriteGeometry( location );
}

// -----------------------------------------------------------------------------
// Name: Lima::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Lima::IsLimit() const
{
    return false;
}
