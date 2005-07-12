// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Perceptions/PHY_PerceptionRadarDoppler.inl $
// $Author: Nld $
// $Modtime: 8/11/04 18:02 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRadarDoppler.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRadarDoppler::HasRadarToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRadarDoppler::HasRadarToHandle() const
{
    return !radars_.empty();
}
