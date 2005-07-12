//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ZoneBrouillageBrod.inl $
// $Author: Nld $
// $Modtime: 4/11/04 14:38 $
// $Revision: 1 $
// $Workfile: MIL_ZoneBrouillageBrod.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneBrouillageBrod::Create()
{
    return *new MIL_ZoneBrouillageBrod();
}
