//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServerMgr_ABC.inl $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: MOS_AgentServerMgr_ABC.inl $
//
//*****************************************************************************

#include "MOS_AgentServer.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMgr_ABC::GetAgentServerFromSession
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_AgentServer& MOS_AgentServerMgr_ABC::GetAgentServerFromSession( DIN::DIN_Link& link )
{
    assert( link.GetUserData() );
    return MT_CAST< MOS_AgentServer& >( *link.GetUserData() );
}

