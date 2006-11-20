//*****************************************************************************
//
// $Created: NLD 2003-04-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateOrderManager.inl $
// $Author: Nld $
// $Modtime: 4/03/05 17:34 $
// $Revision: 2 $
// $Workfile: MIL_AutomateOrderManager.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveRCMissionFinished
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
inline
void MIL_AutomateOrderManager::OnReceiveRCMissionFinished()
{
    bRCMissionFinishedReceived_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
inline
bool MIL_AutomateOrderManager::IsNewMissionStarted() const
{
    return bNewMissionStarted_;
}
