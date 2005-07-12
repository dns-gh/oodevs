//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServer.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_AgentServer.inl $
//
//*****************************************************************************


//=============================================================================
// ACCESSORS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServer::GetSession
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
DIN::DIN_Link& MOS_AgentServer::GetSession() const
{
    assert( pSession_ );
    return *pSession_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServer::GetStationID
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
inline
DIN::DIN_StationID MOS_AgentServer::GetStationID() const
{
    assert( pSession_ );
    return pSession_->GetStationID();
}

