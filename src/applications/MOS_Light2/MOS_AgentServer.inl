//*****************************************************************************
//
// $Created: NLD 2002-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServer.inl $
// $Author: Ape $
// $Modtime: 3/02/04 11:09 $
// $Revision: 1 $
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
MOS_INLINE
DIN::DIN_Link& MOS_AgentServer::GetSession() const
{
    assert( pSession_ );
    return *pSession_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServer::GetStationID
// Created: NLD 2002-07-08
//-----------------------------------------------------------------------------
MOS_INLINE
DIN::DIN_StationID MOS_AgentServer::GetStationID() const
{
    assert( pSession_ );
    return pSession_->GetStationID();
}

