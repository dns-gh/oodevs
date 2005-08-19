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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerMgr_ABC.cpp $
// $Author: Nld $
// $Modtime: 28/11/02 15:10 $
// $Revision: 2 $
// $Workfile: MOS_AgentServerMgr_ABC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"

#include "MOS_AgentServerMgr_ABC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentServerMgr_ABC.inl"
#endif

#include "MOS_AgentServerController.h"
#include "MOS_App.h"

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMgr_ABC constructor
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
MOS_AgentServerMgr_ABC::MOS_AgentServerMgr_ABC( MOS_AgentServerController& controller )
    : controller_( controller )
    , mosServer_ ( controller.GetMOSServer() )
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMgr_ABC destructor
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
MOS_AgentServerMgr_ABC::~MOS_AgentServerMgr_ABC()
{
    
}
