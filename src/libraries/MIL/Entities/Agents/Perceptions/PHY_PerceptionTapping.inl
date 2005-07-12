// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Perceptions/PHY_PerceptionTapping.inl $
// $Author: Nld $
// $Modtime: 5/11/04 16:29 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionTapping.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionTapping::HasTappingToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionTapping::HasTappingToHandle() const
{
    return !tappings_.empty();
}
