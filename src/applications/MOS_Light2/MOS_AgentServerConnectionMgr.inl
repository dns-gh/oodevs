//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServerConnectionMgr.inl $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: MOS_AgentServerConnectionMgr.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::GetAgentServer
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
inline
MOS_AgentServer& MOS_AgentServerConnectionMgr::GetAgentServer() const
{
    assert( pAgentServer_ );
    return *pAgentServer_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::IsConnectedToAgentServer
// Created: NLD 2002-09-05
//-----------------------------------------------------------------------------
inline
bool MOS_AgentServerConnectionMgr::IsConnectedToAgentServer() const
{
    return pAgentServer_ != 0;
}
