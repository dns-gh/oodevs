//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Lima/MIL_LimaManager.cpp $
// $Author: Nld $
// $Modtime: 17/02/05 14:12 $
// $Revision: 4 $
// $Workfile: MIL_LimaManager.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_LimaManager.h"

#include "MIL_AgentServer.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_ASN_Messages.h"

using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MIL_LimaManager constructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MIL_LimaManager::MIL_LimaManager()
{
    
}


//-----------------------------------------------------------------------------
// Name: MIL_LimaManager destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MIL_LimaManager::~MIL_LimaManager()
{
    while( !limaMap_.empty() )
        DestroyLima( *limaMap_.begin()->second ); 
}


//=============================================================================
// CREATION / DESTRUCTION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_LimaManager::DestroyLima
// Created: NLD 2002-08-08
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
bool MIL_LimaManager::DestroyLima( uint nID  )
{
    IT_LimaMap itLima = limaMap_.find( nID );
    if( itLima == limaMap_.end() )
        return false;

    MIL_Lima& lima = *itLima->second;
	lima.Cleanup();
    delete &lima;
    limaMap_.erase( itLima );
    return true;
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_LimaManager::CreateLima
// Created: NLD 2002-08-08
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
MIL_Lima* MIL_LimaManager::OnReceiveMsgCreateLima( const ASN1T_MsgLimaCreation& asnMsg, MIL_MOSContextID nCtx )
{
    if( limaMap_.find( asnMsg.oid ) != limaMap_.end() )
    {
        NET_ASN_MsgLimaCreationAck asnAckMsg;
        asnAckMsg.GetAsnMsg().oid        = asnMsg.oid;
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
        asnAckMsg.Send( nCtx );
        return 0;
    }
   
    
    MIL_Lima* pLima = new MIL_Lima();

    if( pLima->Initialize( asnMsg, nCtx ) )
        limaMap_.insert( std::make_pair( pLima->GetID(), pLima ) );
    else
    {
        delete pLima;
        pLima = 0;
    }
    return pLima;
}


//-----------------------------------------------------------------------------
// Name: MIL_LimaManager::OnReceiveMsgDestroyLima
// Created: NLD 2002-09-10
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
void MIL_LimaManager::OnReceiveMsgDestroyLima( const ASN1T_MsgLimaDestruction& asnMsg, MIL_MOSContextID nCtx )
{
    IT_LimaMap itLima = limaMap_.find( asnMsg );
    if( itLima == limaMap_.end() )
    {
        NET_ASN_MsgLimaDestructionAck asnAckMsg;
        asnAckMsg.GetAsnMsg().oid        = asnMsg;
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
        asnAckMsg.Send( nCtx );
        return;
    }

    MIL_Lima& lima = *itLima->second;

    // Destroy
    lima.Cleanup( nCtx );
    delete &lima;
    limaMap_.erase( itLima );
}

//-----------------------------------------------------------------------------
// Name: MIL_LimaManager::OnReceiveMsgUpdateLima
// Created: NLD 2002-09-13
//-----------------------------------------------------------------------------
void MIL_LimaManager::OnReceiveMsgUpdateLima( const ASN1T_MsgLimaUpdate& asnMsg, MIL_MOSContextID nCtx )
{
    IT_LimaMap itLima = limaMap_.find( asnMsg.oid );
    if( itLima == limaMap_.end() )
    {
        NET_ASN_MsgLimaUpdateAck asnAckMsg;
        asnAckMsg.GetAsnMsg().oid        = asnMsg.oid;
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
        asnAckMsg.Send( nCtx );
        return;
    }

    itLima->second->Update( asnMsg, nCtx );   
}


// -----------------------------------------------------------------------------
// Name: MIL_LimaManager::SendStateToNewClient
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void MIL_LimaManager::SendStateToNewClient( NET_AS_MOSServer& /*connection*/ ) const
{
    for( CIT_LimaMap itLima = limaMap_.begin(); itLima != limaMap_.end(); ++itLima )
        itLima->second->SendFullState();
}
