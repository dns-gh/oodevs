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
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServer.cpp $
// $Author: Nld $
// $Modtime: 20/02/03 15:56 $
// $Revision: 2 $
// $Workfile: MOS_AgentServer.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"

#include "MOS_AgentServer.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentServer.inl"
#endif

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_AgentServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_AgentServer::MOS_AgentServer()
    : DIN_UserData_ABC()
    , pSession_          ( 0 )
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServer destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_AgentServer::~MOS_AgentServer()
{
    
}

//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServer::Initialize
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
void MOS_AgentServer::Initialize( DIN_Link& link )
{
    pSession_ = &link;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServer::Terminate
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
void MOS_AgentServer::Terminate()
{
    pSession_ = 0;    
}
