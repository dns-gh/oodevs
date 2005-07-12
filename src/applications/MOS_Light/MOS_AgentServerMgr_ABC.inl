//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerMgr_ABC.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_AgentServerMgr_ABC.inl $
//
//*****************************************************************************

#include "MOS_AgentServer.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMgr_ABC::GetAgentServerFromSession
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
MOS_AgentServer& MOS_AgentServerMgr_ABC::GetAgentServerFromSession( DIN::DIN_Link& link )
{
    assert( link.GetUserData() );
    return MT_CAST< MOS_AgentServer& >( *link.GetUserData() );
}

