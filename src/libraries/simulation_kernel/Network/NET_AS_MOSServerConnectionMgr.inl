//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServerConnectionMgr.inl $
// $Author: Nld $
// $Modtime: 16/06/04 10:13 $
// $Revision: 1 $
// $Workfile: NET_AS_MOSServerConnectionMgr.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServerConnectionMgr::GetMosLightConnection
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const NET_AS_MOSServerMgr_ABC::T_ConnectionMap& NET_AS_MOSServerConnectionMgr::GetConnections() const
{
    return connections_;
}


