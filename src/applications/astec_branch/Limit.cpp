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

IDManager Limit::idManager_( 138 );

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::Limit( const CoordinateConverter& converter )
    : TacticalLine_ABC( "Limit", idManager_.GetFreeIdentifier(), converter )
    , nLevel_( eNatureLevel_None )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
Limit::Limit( const T_PointVector& pointList, const CoordinateConverter& converter )
    : TacticalLine_ABC(  "Limit", idManager_.GetFreeIdentifier(), pointList, converter )
    , nLevel_( eNatureLevel_None )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
Limit::Limit( const ASN1T_MsgLimitCreation& asnMsg, const CoordinateConverter& converter )
    : TacticalLine_ABC( "Limit", asnMsg.oid, asnMsg.geometrie, converter )
    , nLevel_( (E_NatureLevel) asnMsg.level )
{
    idManager_.LockIdentifier( GetId() );
}

//-----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
Limit::~Limit()
{
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