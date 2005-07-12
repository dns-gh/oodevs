//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_AireLogistique.inl $
// $Author: Nld $
// $Modtime: 14/03/05 18:08 $
// $Revision: 1 $
// $Workfile: MIL_AireLogistique.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AireLogistique::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_AireLogistique::Create()
{
    return *new MIL_AireLogistique();
}
