//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServer.inl $
// $Author: Nld $
// $Modtime: 16/06/04 10:13 $
// $Revision: 1 $
// $Workfile: NET_AS_MOSServer.inl $
//
//*****************************************************************************



//=============================================================================
// ACCESSORS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServer::GetLink
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
DIN::DIN_Link& NET_AS_MOSServer::GetLink() const
{
    return link_;
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServer::GetStationID
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
DIN::DIN_StationID NET_AS_MOSServer::GetStationID() const
{
    return link_.GetStationID();
}

// -----------------------------------------------------------------------------
// Name: NET_AS_MOSServer::GetClientType
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
NET_AS_MOSServer::E_ClientType NET_AS_MOSServer::GetClientType() const
{
    return nClientType_;    
}
