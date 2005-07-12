// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Perceptions/PHY_PerceptionRadarAntiRadar.inl $
// $Author: Nld $
// $Modtime: 23/02/05 14:48 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRadarAntiRadar.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarAntiRadar::HasRadarToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRadarAntiRadar::HasRadarToHandle() const
{
    return !radars_.empty();
}
