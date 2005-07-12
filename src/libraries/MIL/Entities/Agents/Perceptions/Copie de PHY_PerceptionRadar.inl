// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Perceptions/PHY_PerceptionRadar.inl $
// $Author: Nld $
// $Modtime: 5/11/04 14:42 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRadar.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadar::HasRadarToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRadar::HasRadarToHandle() const
{
    return !radars_.empty();
}
