//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/objects/MIL_AirePoser.inl $
// $Author: Pbt $
// $Modtime: 3/02/05 16:45 $
// $Revision: 1 $
// $Workfile: MIL_AirePoser.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AirePoser::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_AirePoser::Create()
{
    return *new MIL_AirePoser();
}
