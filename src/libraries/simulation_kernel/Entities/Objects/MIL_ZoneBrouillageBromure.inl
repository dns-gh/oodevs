//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ZoneBrouillageBromure.inl $
// $Author: Nld $
// $Modtime: 4/11/04 14:38 $
// $Revision: 1 $
// $Workfile: MIL_ZoneBrouillageBromure.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneBrouillageBromure::Create()
{
    return *new MIL_ZoneBrouillageBromure();
}
