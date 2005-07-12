//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/objects/MIL_Piste.inl $
// $Author: Pbt $
// $Modtime: 4/02/05 10:23 $
// $Revision: 1 $
// $Workfile: MIL_Piste.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Piste::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_Piste::Create()
{
    return *new MIL_Piste();
}
