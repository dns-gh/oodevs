//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_FosseAntichar.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_FosseAntichar.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_FosseAntiChar::Create
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_FosseAntiChar::Create()
{
    return *new MIL_FosseAntiChar();
}
