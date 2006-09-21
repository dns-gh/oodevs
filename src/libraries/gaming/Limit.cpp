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
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

IDManager Limit::idManager_( 138 );

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::Limit( Controller& controller, Publisher_ABC& publisher, const CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::translate( "Limit", "Limit" ), idManager_.GetFreeIdentifier(), converter, publisher )
    , controller_( controller )
    , nLevel_( eNatureLevel_None )
{
    controller_.Create( *this );
    TacticalLine_ABC::UpdateToSim();
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Limit::Limit( Controller& controller, Publisher_ABC& publisher, const T_PointVector& pointList, const CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::translate( "Limit", "Limit" ), idManager_.GetFreeIdentifier(), pointList, converter, publisher )
    , controller_( controller )
    , nLevel_( eNatureLevel_None )
{
    controller_.Create( *this );
    TacticalLine_ABC::UpdateToSim();
}

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
Limit::Limit( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimitCreation& asnMsg, const CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::translate( "Limit", "Limit" ), asnMsg.oid, asnMsg.geometrie, converter, publisher )
    , controller_( controller )
    , nLevel_( (E_NatureLevel) asnMsg.level )
{
    idManager_.LockIdentifier( GetId() );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
Limit::Limit( kernel::Controller& controller, Publisher_ABC& publisher, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( xis, converter, publisher )
    , controller_( controller )
{
    controller_.Create( *this );
}

//-----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::~Limit()
{
    controller_.Delete( *this );
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
// Name: Limit::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::Draw( const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        if( nLevel_ == eNatureLevel_ooo )
        {
            glLineWidth( 4.f );
            TacticalLine_ABC::Draw( tools );
            glColor3f( 0.5f, 0.5f, 0.5f );
            glLineWidth( 2.f );
        }
        else
        {
            glLineWidth( 5.f );
            TacticalLine_ABC::Draw( tools );
            glColor3f( 0.1f, 0.1f, 0.1f );
            glLineWidth( 3.f );
        }
        TacticalLine_ABC::Draw( tools );
        glColor3f( 0.f, 0.f, 0.f );
        DrawName( tools );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Limit::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::Select( ActionController& actions ) const
{
    actions.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: Limit::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Limit::ContextMenu( ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *this, point );
}

// -----------------------------------------------------------------------------
// Name: Limit::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Limit::Activate( ActionController& actions ) const
{
    actions.Activate( *this );
}

// -----------------------------------------------------------------------------
// Name: Limit::Serialize
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void Limit::Serialize( xml::xostream& xos ) const
{
    xos << start( "limit" );
        SerializeGeometry( xos );
    xos << end();
}

