//*****************************************************************************
//
// $Created: NLD 2003-04-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Population/MIL_PopulationMission_ABC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_PopulationMission_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::GetPopulation
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MIL_Population& MIL_PopulationMission_ABC::GetPopulation() const
{
    return population_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission_ABC::GetType
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MIL_PopulationMissionType& MIL_PopulationMission_ABC::GetType() const
{
    return type_;
}
