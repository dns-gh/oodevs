//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Network/NET_AgentServer.inl $
// $Author: Nld $
// $Modtime: 1/03/05 10:48 $
// $Revision: 4 $
// $Workfile: NET_AgentServer.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetPortAS_MOS
// Created: JVT 02-10-30
//-----------------------------------------------------------------------------
inline
uint16 NET_AgentServer::GetPortAS_MOS() const
{
    return nPortAS_MOS_;
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetMagicAS_MOS
// Created: JVT 02-10-30
//-----------------------------------------------------------------------------
inline
uint NET_AgentServer::GetMagicAS_MOS() const
{
    return nMagicAS_MOS_;
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetConnectionProtocols
// Created: JVT 02-10-30
//-----------------------------------------------------------------------------
inline
DIN::DIN_ConnectionProtocols& NET_AgentServer::GetConnectionProtocols()
{
    return connectionProtocols_;
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetDINEngine
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
inline
DIN::DIN_Engine& NET_AgentServer::GetDINEngine()
{
    return dinEngine_;
}


//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetConnectionMgr
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
NET_AS_MOSServerConnectionMgr& NET_AgentServer::GetConnectionMgr() const
{
    assert( pConnectionMgr_ );
    return *pConnectionMgr_;
}

//-----------------------------------------------------------------------------
// Name: NET_AgentServer::GetMessageMgr
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
NET_AS_MOSServerMsgMgr& NET_AgentServer::GetMessageMgr() const
{
    assert( pMsgMgr_ );
    return *pMsgMgr_;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::MustSendUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
inline
bool NET_AgentServer::MustSendUnitVisionCones() const
{
    return bSendUnitVisionCones_;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::GetDINEngineCriticalSection
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
inline
MT_CriticalSection& NET_AgentServer::GetDINEngineCriticalSection()
{
    return dinEngineCriticalSection_;
}

// -----------------------------------------------------------------------------
// Name: NET_AgentServer::IsThreaded
// Created: NLD 2005-03-01
// -----------------------------------------------------------------------------
inline
bool NET_AgentServer::IsThreaded() const
{
    return bThreaded_;
}
