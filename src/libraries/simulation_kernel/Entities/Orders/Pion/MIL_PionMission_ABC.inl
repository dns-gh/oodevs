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
// Name: MIL_PionMission_ABC::GetPion
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MIL_AgentPion& MIL_PionMission_ABC::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission_ABC::GetType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_PionMissionType& MIL_PionMission_ABC::GetType() const
{
    return type_;
}
