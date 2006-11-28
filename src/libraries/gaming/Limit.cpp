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
#include "ASN_Messages.h"
#include "TacticalLinePositions.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

IDManager Limit::idManager_( 138 );

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Limit::Limit( Controller& controller, Publisher_ABC& publisher )
    : TacticalLine_ABC( tools::translate( "Limit", "Limit" ), idManager_.GetFreeIdentifier(), publisher )
    , controller_( controller )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
Limit::Limit( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimitCreation& asnMsg )
    : TacticalLine_ABC( asnMsg.tactical_line.nom, asnMsg.oid, publisher )
    , controller_( controller )
{
    idManager_.LockIdentifier( GetId() );
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

//-----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::~Limit()
{
    controller_.Delete( *(kernel::TacticalLine_ABC*)this );
    idManager_.ReleaseIdentifier( GetId() );
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
            ASN_MsgLimitCreationRequest message;
            message.GetAsnMsg().nom = GetName();
            WriteGeometry ( message.GetAsnMsg().geometrie );
            WriteDiffusion( message.GetAsnMsg().diffusion );
            Send( message );
            delete[] message.GetAsnMsg().geometrie.vecteur_point.elem;
        }
        break;
    case eStateModified:
        {
            ASN_MsgLimitUpdateRequest message;
            message.GetAsnMsg().oid = GetId();
            message.GetAsnMsg().tactical_line.nom = GetName();
            WriteGeometry ( message.GetAsnMsg().tactical_line.geometrie );
            WriteDiffusion( message.GetAsnMsg().tactical_line.diffusion );
            Send( message );
            delete[] message.GetAsnMsg().tactical_line.geometrie.vecteur_point.elem;
        }
        break;
    case eStateDeleted:
        ASN_MsgLimitDestructionRequest asnMsg;
        asnMsg.GetAsnMsg() = GetId();
        Send( asnMsg );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Limit::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::Select( ActionController& actions ) const
{
    actions.Select( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this );
}

// -----------------------------------------------------------------------------
// Name: Limit::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::ContextMenu( ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *(Entity_ABC*)this, *(kernel::TacticalLine_ABC*)this, *this, point );
}

// -----------------------------------------------------------------------------
// Name: Limit::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Limit::Activate( ActionController& actions ) const
{
    actions.Activate( *this, *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Limit::Serialize
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void Limit::Serialize( xml::xostream& xos ) const
{
    xos << start( "limit" );
    TacticalLine_ABC::Serialize( xos );
    Interface().Apply( &Serializable_ABC::SerializeAttributes, xos );
    xos << end();
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
// Name: Limit::CopyTo
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void Limit::CopyTo( ASN1T_Line& destination ) const
{
    WriteGeometry ( destination );
}
