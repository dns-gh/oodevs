//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ZoneForbiddenFire.inl $
// $Author: Nld $
// $Modtime: 21/03/05 15:54 $
// $Revision: 1 $
// $Workfile: MIL_ZoneForbiddenFire.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenFire::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneForbiddenFire::Create()
{
    return *new MIL_ZoneForbiddenFire();
}
