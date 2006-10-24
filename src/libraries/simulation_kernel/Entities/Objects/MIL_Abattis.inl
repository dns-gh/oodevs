//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_Abattis.inl $
// $Author: Nld $
// $Modtime: 21/02/05 11:51 $
// $Revision: 1 $
// $Workfile: MIL_Abattis.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MIL_Abattis::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_Abattis::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_Abattis( type, nID, army );
}
