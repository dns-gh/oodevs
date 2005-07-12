//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Limit/MIL_LimitManager.cpp $
// $Author: Age $
// $Modtime: 7/03/05 12:04 $
// $Revision: 5 $
// $Workfile: MIL_LimitManager.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_LimitManager.h"

#include "MIL_AgentServer.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"

using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MIL_LimitManager constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MIL_LimitManager::MIL_LimitManager()
{
    
}


//-----------------------------------------------------------------------------
// Name: MIL_LimitManager destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MIL_LimitManager::~MIL_LimitManager()
{
    while( !limitMap_.empty() )
        DestroyLimit( *limitMap_.begin()->second );
}

//=============================================================================
// CREATION / DESTRUCTION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_LimitManager::DestroyLimit
// Created: NLD 2002-08-08
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
bool MIL_LimitManager::DestroyLimit( uint nID )
{
    IT_LimitMap itLimit = limitMap_.find( nID );
    if( itLimit == limitMap_.end() )
        return false;

    MIL_Limit& limit = *itLimit->second;
	limit.Cleanup();
    limitMap_.erase( itLimit );
    return true;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_LimitManager::CreateLimit
// Created: NLD 2002-08-08
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
MIL_Limit* MIL_LimitManager::OnReceiveMsgCreateLimit( const ASN1T_MsgLimitCreation& asnMsg, MIL_MOSContextID nCtx )
{
    if( limitMap_.find( asnMsg.oid ) != limitMap_.end() )
    {
        NET_ASN_MsgLimitCreationAck asnAckMsg;
        asnAckMsg.GetAsnMsg().oid        = asnMsg.oid;
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
        asnAckMsg.Send( nCtx );
        return 0;
    }
   
    MIL_Limit* pLimit = new MIL_Limit();
    if( pLimit->Initialize( asnMsg, nCtx ) )
        limitMap_.insert( std::make_pair( pLimit->GetID(), pLimit ) );
    else
    {
        delete pLimit;
        pLimit = 0;
    }
    return pLimit;
}


//-----------------------------------------------------------------------------
// Name: MIL_LimitManager::OnReceiveMsgDestroyLimit
// Created: NLD 2002-09-10
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
void MIL_LimitManager::OnReceiveMsgDestroyLimit( const ASN1T_MsgLimitDestruction& asnMsg, MIL_MOSContextID nCtx )
{
    IT_LimitMap itLimit = limitMap_.find( asnMsg );
    if( itLimit == limitMap_.end() )
    {
        NET_ASN_MsgLimitDestructionAck asnAckMsg;
        asnAckMsg.GetAsnMsg().oid        = asnMsg;
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
        asnAckMsg.Send( nCtx );
        return;
    }

    MIL_Limit& limit = *itLimit->second;

    // Destroy
    limit.Cleanup( nCtx );
    limitMap_.erase( itLimit );
}

//-----------------------------------------------------------------------------
// Name: MIL_LimitManager::OnReceiveMsgUpdateLimit
// Created: NLD 2002-09-13
//-----------------------------------------------------------------------------
void MIL_LimitManager::OnReceiveMsgUpdateLimit( const ASN1T_MsgLimitUpdate& asnMsg, MIL_MOSContextID nCtx )
{
    IT_LimitMap itLimit = limitMap_.find( asnMsg.oid );
    if( itLimit == limitMap_.end() )
    {
        NET_ASN_MsgLimitUpdateAck asnAckMsg;
        asnAckMsg.GetAsnMsg().oid        = asnMsg.oid;
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
        asnAckMsg.Send( nCtx );
        return;
    }

    itLimit->second->Update( asnMsg, nCtx );   
}

// -----------------------------------------------------------------------------
// Name: MIL_LimitManager::SendStateToNewClient
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void MIL_LimitManager::SendStateToNewClient( NET_AS_MOSServer& /*connection*/ ) const
{
    for( CIT_LimitMap itLimit = limitMap_.begin(); itLimit != limitMap_.end(); ++itLimit )
        itLimit->second->SendFullState();
}
