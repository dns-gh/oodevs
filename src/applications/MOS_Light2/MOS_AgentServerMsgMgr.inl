//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServerMsgMgr.inl $
// $Author: Ape $
// $Modtime: 22/10/04 10:12 $
// $Revision: 4 $
// $Workfile: MOS_AgentServerMsgMgr.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::GetMsgRecorder
// Created: APE 2004-10-21
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_MsgRecorder& MOS_AgentServerMsgMgr::GetMsgRecorder()
{
    return msgRecorder_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::BuildMessage
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
MOS_INLINE
DIN::DIN_BufferedMessage MOS_AgentServerMsgMgr::BuildMessage()
{
    assert( pMessageService_ );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::IsPaused
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
MOS_INLINE
bool MOS_AgentServerMsgMgr::IsPaused() const
{
    return bPaused_;
}

