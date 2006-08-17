// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Perceptions/PHY_PerceptionRecoSurveillance.inl $
// $Author: Nld $
// $Modtime: 17/11/04 11:55 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRecoSurveillance.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoSurveillance::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRecoSurveillance::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
