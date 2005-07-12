// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionTappingRadar.inl $
// $Author: Nld $
// $Modtime: 25/03/05 11:41 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionTappingRadar.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTappingRadar::HasRadarToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionTappingRadar::HasRadarToHandle() const
{
    return !radars_.empty();
}
