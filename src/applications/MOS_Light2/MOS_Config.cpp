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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Config.cpp $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
// $Workfile: MOS_Config.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_Config.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Config.inl"
#endif

using namespace NEK;

//-----------------------------------------------------------------------------
// Name: MOS_Config constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_Config::MOS_Config()
    : nPortAgentServerMOSServer_  ( MOS_DEFAULT_PORT_AS_MOS )
    , connectionProtocols_        ( NEK_Protocols::eTCP, NEK_Protocols::eIPv4   )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Config destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_Config::~MOS_Config()
{
    
}
