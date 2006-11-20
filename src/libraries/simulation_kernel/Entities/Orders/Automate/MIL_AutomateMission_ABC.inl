//*****************************************************************************
//
// $Created: NLD 2003-04-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Automate/MIL_AutomateMission_ABC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_AutomateMission_ABC.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetMRT
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
inline
MIL_AutomateMRT& MIL_AutomateMission_ABC::GetMRT()
{
    return mrt_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetAutomate
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MIL_Automate& MIL_AutomateMission_ABC::GetAutomate() const
{
    return automate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::GetType
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MIL_AutomateMissionType& MIL_AutomateMission_ABC::GetType() const
{
    return type_;
}
