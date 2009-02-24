//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Pion/MIL_PionMission_ABC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_PionMission_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::GetType
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_MissionType_ABC& MIL_AutomateMission::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::GetAutomate
// Created: NLD 2007-04-03
// -----------------------------------------------------------------------------
inline
MIL_Automate& MIL_AutomateMission::GetAutomate() const
{
    return automate_;
}
