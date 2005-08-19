//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerController.cpp $
// $Author: Nld $
// $Modtime: 20/02/03 19:23 $
// $Revision: 2 $
// $Workfile: MOS_AgentServerController.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_AgentServerController.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentServerController.inl"
#endif

#include "MOS_AgentServerConnectionMgr.h"
#include "MOS_AgentServerMsgMgr.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerController constructor
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
MOS_AgentServerController::MOS_AgentServerController( MOS_MOSServer& mosServer )
    : mosServer_( mosServer )
{
    pConnectionMgr_ = new MOS_AgentServerConnectionMgr( *this );
    pMsgMgr_        = new MOS_AgentServerMsgMgr       ( *this );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerController destructor
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
MOS_AgentServerController::~MOS_AgentServerController()
{    
    delete pMsgMgr_;
    delete pConnectionMgr_;  
}
