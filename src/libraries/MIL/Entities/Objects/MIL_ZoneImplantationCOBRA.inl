//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ZoneImplantationCOBRA.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_ZoneImplantationCOBRA.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneImplantationCOBRA::Create()
{
    return *new MIL_ZoneImplantationCOBRA();
}
