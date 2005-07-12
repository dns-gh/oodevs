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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MOSServer.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_MOSServer.inl $
//
//*****************************************************************************

#include "MOS_App.h"

//-----------------------------------------------------------------------------
// Name: MOS_MOSServer::GetDINEngine
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
inline
DIN::DIN_Engine& MOS_MOSServer::GetDINEngine()
{
    return dinEngine_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MOSServer::GetController
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
inline
MOS_AgentServerController& MOS_MOSServer::GetController() const
{
    assert( pASController_ );
    return *pASController_;
}
