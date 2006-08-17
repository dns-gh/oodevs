// *****************************************************************************
//
// $Created: JVT 2004-10-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionRecoPoint.inl $
// $Author: Jvt $
// $Modtime: 21/10/04 11:01 $
// $Revision: 1 $
// $Workfile: PHY_PerceptionRecoPoint.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionRecoPoint::HasPointToHandle
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
inline
bool PHY_PerceptionRecoPoint::HasPointToHandle() const
{
    return !recos_.empty();
}
