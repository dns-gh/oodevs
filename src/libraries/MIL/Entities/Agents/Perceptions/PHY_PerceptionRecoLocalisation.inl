// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoLocalisation.inl $
// $Author: Jvt $
// $Modtime: 22/10/04 14:39 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRecoLocalisation.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoLocalisation::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRecoLocalisation::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
