//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerController.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 5 $
// $Workfile: MOS_AgentServerController.inl $
//
//*****************************************************************************

#include "MOS_MOSServer.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerController::GetDINEngine
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
inline
DIN::DIN_Engine& MOS_AgentServerController::GetDINEngine()
{
    return mosServer_.GetDINEngine();
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerController::GetMOSServer
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
MOS_MOSServer& MOS_AgentServerController::GetMOSServer() const
{
    return mosServer_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerController::GetConnectionMgr
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
MOS_AgentServerConnectionMgr& MOS_AgentServerController::GetConnectionMgr() const
{
    assert( pConnectionMgr_ );
    return *pConnectionMgr_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerController::GetMessageMgr
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
MOS_AgentServerMsgMgr& MOS_AgentServerController::GetMessageMgr() const
{
    assert( pMsgMgr_ );
    return *pMsgMgr_;
}
