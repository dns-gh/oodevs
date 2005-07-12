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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Config.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_Config.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_Config::GetPortAgentServerMOSServer
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
inline
uint16 MOS_Config::GetPortAgentServerMOSServer() const
{
    return nPortAgentServerMOSServer_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Config::GetConnectionProtocols
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
inline
DIN::DIN_ConnectionProtocols& MOS_Config::GetConnectionProtocols()
{
    return connectionProtocols_;
}
