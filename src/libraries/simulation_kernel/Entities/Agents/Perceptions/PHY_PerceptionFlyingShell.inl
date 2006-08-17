// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Perceptions/PHY_PerceptionFlyingShell.inl $
// $Author: Nld $
// $Modtime: 21/02/05 11:15 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionFlyingShell.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionFlyingShell::HasLocalisationToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionFlyingShell::HasLocalisationToHandle() const
{
    return !zones_.empty();
}
