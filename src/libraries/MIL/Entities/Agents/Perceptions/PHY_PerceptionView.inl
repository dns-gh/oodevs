// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Perceptions/PHY_PerceptionView.inl $
// $Author: Jvt $
// $Modtime: 29/04/05 10:24 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionView.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Enable
// Created: JVT 2005-04-29
// -----------------------------------------------------------------------------
inline
void PHY_PerceptionView::Enable()
{
    bIsEnabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionView::Disable
// Created: JVT 2005-04-29
// -----------------------------------------------------------------------------
inline
void PHY_PerceptionView::Disable()
{
    bIsEnabled_ = false;
}
