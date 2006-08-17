// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoObjects.inl $
// $Author: Jvt $
// $Modtime: 19/01/05 10:27 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRecoObjects.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoObjects::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRecoObjects::HasLocalisationToHandle() const
{
    return !recos_.empty();
}
