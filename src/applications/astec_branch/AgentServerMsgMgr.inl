//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentServerMsgMgr.inl $
// $Author: Ape $
// $Modtime: 22/10/04 10:12 $
// $Revision: 4 $
// $Workfile: AgentServerMsgMgr.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::GetMsgRecorder
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
inline
MsgRecorder& AgentServerMsgMgr::GetMsgRecorder()
{
    return msgRecorder_;
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::BuildMessage
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
inline
DIN::DIN_BufferedMessage AgentServerMsgMgr::BuildMessage()
{
    assert( pMessageService_ );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}
