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

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
inline
bool MIL_PionOrderManager::IsNewMissionStarted() const
{
    return bNewMissionStarted_;
}
