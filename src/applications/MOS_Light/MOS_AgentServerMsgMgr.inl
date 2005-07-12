//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerMsgMgr.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_AgentServerMsgMgr.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::BuildMessage
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
inline
DIN::DIN_BufferedMessage MOS_AgentServerMsgMgr::BuildMessage()
{
    assert( pMessageService_ );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}
//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::IsPaused
// Created: FBD 03-01-22
//-----------------------------------------------------------------------------
inline
bool MOS_AgentServerMsgMgr::IsPaused() const
{
    return bPaused_;
}

