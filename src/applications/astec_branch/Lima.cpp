//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Lima.cpp $
// $Author: Ape $
// $Modtime: 26/01/05 15:47 $
// $Revision: 8 $
// $Workfile: Lima.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Lima.h"
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "Tools.h"

IDManager Lima::idManager_( 137 );

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( const CoordinateConverter& converter )
    : TacticalLine_ABC( Tools::ToString( eLimaFuncLCA ).ascii(), idManager_.GetFreeIdentifier(), converter )
    , nFuncType_      ( eLimaFuncLCA )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( const T_PointVector& pointList, E_FuncLimaType nFuncType, const CoordinateConverter& converter )
    : TacticalLine_ABC( Tools::ToString( nFuncType ).ascii(), idManager_.GetFreeIdentifier(), pointList, converter )
    , nFuncType_      ( nFuncType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::Lima( const ASN1T_MsgLimaCreation& asnMsg, const CoordinateConverter& converter )
    : TacticalLine_ABC( Tools::ToString( (E_FuncLimaType)asnMsg.fonction ).ascii(), asnMsg.oid, asnMsg.geometrie, converter )
    , nFuncType_      ( (E_FuncLimaType)asnMsg.fonction )
{
    idManager_.LockIdentifier( GetId() );
}
    
// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
Lima::~Lima()
{
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
            break;
    }
}
