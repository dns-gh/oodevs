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
#include "ASN_Messages.h"
#include "Tools.h"
#include "TacticalLinePositions.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

IDManager Lima::idManager_( 137 );

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, Publisher_ABC& publisher )
    : TacticalLine_ABC( "Lima", idManager_.GetFreeIdentifier(), publisher )
    , controller_     ( controller )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimaCreation& asnMsg )
    : TacticalLine_ABC( asnMsg.tactical_line.nom, asnMsg.oid, publisher )
    , controller_     ( controller )
{
    idManager_.LockIdentifier( GetId() );
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}
 
// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    controller_.Delete( *(kernel::TacticalLine_ABC*)this );
    idManager_.ReleaseIdentifier( GetId() );
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
            ASN_MsgLimaCreationRequest message;
            message.GetAsnMsg().nom = GetName();
            WriteGeometry ( message.GetAsnMsg().geometrie );
            WriteDiffusion( message.GetAsnMsg().diffusion );
            Send( message );
            delete[] message.GetAsnMsg().geometrie.vecteur_point.elem;
        }
        break;
    case eStateModified:
        {
            ASN_MsgLimaUpdateRequest message;
            message.GetAsnMsg().oid = GetId();
            message.GetAsnMsg().tactical_line.nom = GetName();
            WriteGeometry ( message.GetAsnMsg().tactical_line.geometrie );
            WriteDiffusion( message.GetAsnMsg().tactical_line.diffusion );
            Send( message );
            delete[] message.GetAsnMsg().tactical_line.geometrie.vecteur_point.elem;
        }
        break;
    case eStateDeleted:
        ASN_MsgLimaDestructionRequest asnMsg;
        asnMsg.GetAsnMsg() = GetId();
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
// Name: Lima::Serialize
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void Lima::Serialize( xml::xostream& xos ) const
{
    xos << start( "lima" );
    TacticalLine_ABC::Serialize( xos );
    Interface().Apply( &Serializable_ABC::SerializeAttributes, xos );
    xos << end();
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
// Name: Lima::IsLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
bool Lima::IsLimit() const
{
    return false;
}
