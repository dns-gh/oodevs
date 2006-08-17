//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ZoneImplantationMortier.inl $
// $Author: Nld $
// $Modtime: 21/03/05 16:54 $
// $Revision: 1 $
// $Workfile: MIL_ZoneImplantationMortier.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneImplantationMortier::Create()
{
    return *new MIL_ZoneImplantationMortier();
}
