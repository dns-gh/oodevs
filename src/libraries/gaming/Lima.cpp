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
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "xeumeuleu/xml.h"
#include <qobject.h>

using namespace kernel;
using namespace xml;

IDManager Lima::idManager_( 137 );

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, Publisher_ABC& publisher, const CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::ToString( eLimaFuncLCA ), idManager_.GetFreeIdentifier(), converter, publisher )
    , controller_     ( controller )
    , nFuncType_      ( eLimaFuncLCA )
{
    controller_.Create( *this );
    TacticalLine_ABC::UpdateToSim();
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, Publisher_ABC& publisher, const T_PointVector& pointList, E_FuncLimaType nFuncType, const CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::ToString( nFuncType ), idManager_.GetFreeIdentifier(), pointList, converter, publisher )
    , controller_     ( controller )
    , nFuncType_      ( nFuncType )
{
    controller_.Create( *this );
    TacticalLine_ABC::UpdateToSim();
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( Controller& controller, Publisher_ABC& publisher, const ASN1T_MsgLimaCreation& asnMsg, const CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( tools::ToString( (E_FuncLimaType)asnMsg.fonction ), asnMsg.oid, asnMsg.geometrie, converter, publisher )
    , controller_     ( controller )
    , nFuncType_      ( (E_FuncLimaType)asnMsg.fonction )
{
    idManager_.LockIdentifier( GetId() );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
Lima::Lima( kernel::Controller& controller, Publisher_ABC& publisher, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( xis, converter, publisher )
    , controller_( controller )
{
    int type = 0;
    xis >> xml::attribute( "type", type );
    nFuncType_ = E_FuncLimaType( type );
    controller_.Create( *this );
}
 
// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    controller_.Delete( *this );
    idManager_.ReleaseIdentifier( GetId() );
}

//-----------------------------------------------------------------------------
// Name: Lima::FillAndSend
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
template< typename T >
void Lima::FillAndSend()
{
    T message;
    message.GetAsnMsg().oid      = GetId();
    message.GetAsnMsg().fonction = (ASN1T_EnumTypeLima)nFuncType_;
    WriteGeometry( message.GetAsnMsg().geometrie );
    Send( message );
    delete[] message.GetAsnMsg().geometrie.vecteur_point.elem;
};

// -----------------------------------------------------------------------------
// Name: Lima::UpdateToSim
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void Lima::UpdateToSim( E_State state )
{
    switch( state )
    {
        case eStateCreated:
            FillAndSend< ASN_MsgLimaCreation >();
            break;
        case eStateModified:
            FillAndSend< ASN_MsgLimaUpdate >();
            break;
        case eStateDeleted:
        {
            ASN_MsgLimaDestruction asnMsg;
            asnMsg.GetAsnMsg() = GetId();
            Send( asnMsg );
        }
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Lima::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::Draw( const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 5.0 );
        TacticalLine_ABC::Draw( tools );
        glLineWidth( 3.0 );
        glColor4d( 0.55, 0.3, 0.1, 1.0 );
        TacticalLine_ABC::Draw( tools );
        glColor3f( 0.f, 0.f, 0.f );
        DrawName( tools );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Lima::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::Select( ActionController& actions ) const
{
    actions.Select( *this );
}

// -----------------------------------------------------------------------------
// Name: Lima::ContextMenu
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Lima::ContextMenu( ActionController& actions, const QPoint& point ) const
{
    actions.ContextMenu( *this, point );
}

// -----------------------------------------------------------------------------
// Name: Lima::Activate
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void Lima::Activate( ActionController& actions ) const
{
    actions.Activate( *this );
}

// -----------------------------------------------------------------------------
// Name: Lima::Serialize
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void Lima::Serialize( xml::xostream& xos ) const
{
    xos << start( "lima" )
            << attribute( "type", int( nFuncType_ ) );
        SerializeGeometry( xos );
    xos << end();
}
