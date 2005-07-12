//*****************************************************************************
//
// $Created: NLD 2003-01-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_Def.h $
// $Author: Nld $
// $Modtime: 16/05/03 13:18 $
// $Revision: 1 $
// $Workfile: NET_Def.h $
//
//*****************************************************************************

#ifndef __NET_Def_h_
#define __NET_Def_h_

enum MIL_Connector
{
    eConnector_SIM_MOS = (DIN::DIN_Connector_ABC::DIN_ConnectionID)-1,
    eConnector_SIM_LAU = 1,
    eConnector_MOS_LAU = 2,
    eConnector_SIM_WMI = 3
};


enum MIL_ConnMagicCookie
{
    eConnMagicMOSServerAgentServer   = 1
};

//-------------------------------------------------------------------------
/** @name Default configuration */
//-------------------------------------------------------------------------
//@{
#define MIL_DEFAULT_PORT_AS_MOS 10000
//@}

#endif // __NET_Def_h_
