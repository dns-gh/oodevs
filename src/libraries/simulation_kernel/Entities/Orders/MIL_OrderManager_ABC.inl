//*****************************************************************************
//
// $Created: NLD 2003-01-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_OrderManager_ABC.inl $
// $Author: Nld $
// $Modtime: 4/03/05 17:29 $
// $Revision: 2 $
// $Workfile: MIL_OrderManager_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::IsNewMissionStarted
// Created: NLD 2005-09-19
// -----------------------------------------------------------------------------
inline
bool MIL_OrderManager_ABC::IsNewMissionStarted() const
{
    return bNewMissionStarted_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::GetCurrentMission
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
inline
MIL_Mission_ABC* MIL_OrderManager_ABC::GetCurrentMission() const
{
    return pMission_;
}
