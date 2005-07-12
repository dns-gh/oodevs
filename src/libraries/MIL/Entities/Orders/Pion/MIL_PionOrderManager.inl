//*****************************************************************************
//
// $Created: NLD 2003-01-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_PionOrderManager.inl $
// $Author: Nld $
// $Modtime: 4/03/05 17:29 $
// $Revision: 2 $
// $Workfile: MIL_PionOrderManager.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveRCMissionFinished
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
inline
void MIL_PionOrderManager::OnReceiveRCMissionFinished()
{
    bRCMissionFinishedReceived_ = true;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::IsWaitingForOrderConduite
// Created: NLD 2003-07-15
//-----------------------------------------------------------------------------
inline
bool MIL_PionOrderManager::IsWaitingForOrderConduite() const
{
    return !orderConduiteRequestVector_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::GetMission
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
const MIL_PionMission_ABC* MIL_PionOrderManager::GetMission() const
{
    return pMission_;
}
