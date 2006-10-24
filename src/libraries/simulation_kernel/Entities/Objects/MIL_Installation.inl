//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Installation.inl $
// $Author: Nld $
// $Modtime: 9/02/05 15:01 $
// $Revision: 2 $
// $Workfile: MIL_Installation.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Installation::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_Installation::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_Installation( type, nID, army );
}
