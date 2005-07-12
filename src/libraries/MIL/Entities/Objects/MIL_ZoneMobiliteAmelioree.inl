//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/objects/MIL_ZoneMobiliteAmelioree.inl $
// $Author: Pbt $
// $Modtime: 4/02/05 11:42 $
// $Revision: 1 $
// $Workfile: MIL_ZoneMobiliteAmelioree.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMobiliteAmelioree::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneMobiliteAmelioree::Create()
{
    return *new MIL_ZoneMobiliteAmelioree();
}
