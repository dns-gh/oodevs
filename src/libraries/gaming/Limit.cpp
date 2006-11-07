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
    , nLevel_( eNatureLevel_None )
{
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
Limit::Limit( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimitCreation& asnMsg )
    : TacticalLine_ABC( tools::translate( "Limit", "Limit" ), asnMsg.oid, publisher )
    , controller_( controller )
    , nLevel_( (E_NatureLevel) asnMsg.level )
{
    idManager_.LockIdentifier( GetId() );
    controller_.Create( *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
Limit::Limit( kernel::Controller& controller, Publisher_ABC& publisher, xml::xistream& xis )
    : TacticalLine_ABC( xis, publisher )
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
// Name: Limit::FillAndSend
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
template< typename T >
void Limit::FillAndSend()
{
    T message;
    message.GetAsnMsg().oid   = GetId();
    message.GetAsnMsg().level = (ASN1T_EnumNatureLevel) nLevel_;
    WriteGeometry( message.GetAsnMsg().geometrie );
    Send( message );
    delete[] message.GetAsnMsg().geometrie.vecteur_point.elem;
};

//-----------------------------------------------------------------------------
// Name: Limit::UpdateToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void Limit::UpdateToSim( E_State state )
{
    if( state == eStateCreated )
        FillAndSend< ASN_MsgLimitCreation >();
    else if( state == eStateModified )
        FillAndSend< ASN_MsgLimitUpdate >();
    else if( state == eStateDeleted )
    {
        ASN_MsgLimitDestruction asnMsg;
        asnMsg.GetAsnMsg() = GetId();
        Send( asnMsg );
    }
}

// -----------------------------------------------------------------------------
// Name: Limit::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::Select( ActionController& actions ) const
{
    actions.Select( *this, *(kernel::TacticalLine_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Limit::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::ContextMenu( ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *this, *(kernel::TacticalLine_ABC*)this, point );
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
