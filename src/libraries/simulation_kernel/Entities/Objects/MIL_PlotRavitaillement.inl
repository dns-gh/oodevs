//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_PlotRavitaillement.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_PlotRavitaillement.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PlotRavitaillement::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_PlotRavitaillement::Create()
{
    return *new MIL_PlotRavitaillement();
}
