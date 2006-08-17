//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServerMgr_ABC.inl $
// $Author: Nld $
// $Modtime: 16/06/04 10:13 $
// $Revision: 1 $
// $Workfile: NET_AS_MOSServerMgr_ABC.inl $
//
//*****************************************************************************


#include "NET_AS_MOSServer.h"

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServerMgr_ABC::GetMosConnectionFromLink
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
NET_AS_MOSServer* NET_AS_MOSServerMgr_ABC::GetMosConnectionFromLink( DIN::DIN_Link& link )
{
    return MT_CAST< NET_AS_MOSServer* >( link.GetUserData() );
}

