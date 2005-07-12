//*****************************************************************************
//
// $Created: NLD 2003-04-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Automate/MIL_AutomateMRT.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_AutomateMRT.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::SetMissionForPion
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
inline
bool MIL_AutomateMRT::SetMissionForPion( MIL_AgentPion& pion, MIL_PionMission_ABC& mission )
{
    return missionPionMap_.insert( std::make_pair( &pion, &mission ) ).second;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::SetFuseauForPion
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
inline
bool MIL_AutomateMRT::SetFuseauForPion( MIL_AgentPion& pion, MIL_Fuseau& fuseau )
{
    return fuseauPionMap_.insert( std::make_pair( &pion, &fuseau ) ).second;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMRT::IsActivated
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
inline
bool MIL_AutomateMRT::IsActivated() const
{
    return bActivated_;
}
