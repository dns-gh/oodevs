//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Limit.cpp $
// $Author: Ape $
// $Modtime: 26/01/05 16:03 $
// $Revision: 8 $
// $Workfile: Limit.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Limit.h"
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "Controller.h"
#include "ActionController.h"

IDManager Limit::idManager_( 138 );

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::Limit( Controller& controller, const CoordinateConverter& converter )
    : TacticalLine_ABC( "Limit", idManager_.GetFreeIdentifier(), converter )
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
Limit::Limit( Controller& controller, const T_PointVector& pointList, const CoordinateConverter& converter )
    : TacticalLine_ABC(  "Limit", idManager_.GetFreeIdentifier(), pointList, converter )
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
Limit::Limit( Controller& controller, const ASN1T_MsgLimitCreation& asnMsg, const CoordinateConverter& converter )
    : TacticalLine_ABC( "Limit", asnMsg.oid, asnMsg.geometrie, converter )
    , controller_( controller )
    , nLevel_( (E_NatureLevel) asnMsg.level )
{
    idManager_.LockIdentifier( GetId() );
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
