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
// Name: MIL_PionMission::GetType
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType& MIL_PionMission::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::GetPion
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
inline
MIL_AgentPion& MIL_PionMission::GetPion() const
{
    return pion_;
}
