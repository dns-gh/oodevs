//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_AS_MOSServer.cpp $
// $Author: Nld $
// $Modtime: 16/06/04 10:13 $
// $Revision: 1 $
// $Workfile: NET_AS_MOSServer.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DIN/DIN_Link.h"
#include "NET_AS_MOSServer.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServer constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServer::NET_AS_MOSServer( DIN::DIN_Link& link )
    : DIN_UserData_ABC()
    , link_           ( link )
{
    assert( !link_.GetUserData() );
    link_.SetUserData( this );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServer destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
NET_AS_MOSServer::~NET_AS_MOSServer()
{
    link_.SetUserData( 0 );
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServer::GetLink
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
DIN::DIN_Link& NET_AS_MOSServer::GetLink() const
{
    return link_;
}

//-----------------------------------------------------------------------------
// Name: NET_AS_MOSServer::GetStationID
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
DIN::DIN_StationID NET_AS_MOSServer::GetStationID() const
{
    return link_.GetStationID();
}
