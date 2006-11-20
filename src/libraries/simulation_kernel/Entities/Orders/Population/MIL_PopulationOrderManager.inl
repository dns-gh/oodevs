//*****************************************************************************
//
// $Created: NLD 2003-04-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Population/MIL_PopulationOrderManager.inl $
// $Author: Nld $
// $Modtime: 4/03/05 17:34 $
// $Revision: 2 $
// $Workfile: MIL_PopulationOrderManager.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveRCMissionFinished
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
inline
void MIL_PopulationOrderManager::OnReceiveRCMissionFinished()
{
    bRCMissionFinishedReceived_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationOrderManager::IsNewMissionStarted() const
{
    return bNewMissionStarted_;
}
