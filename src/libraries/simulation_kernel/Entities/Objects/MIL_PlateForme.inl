//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/objects/MIL_PlateForme.inl $
// $Author: Pbt $
// $Modtime: 4/02/05 11:02 $
// $Revision: 1 $
// $Workfile: MIL_PlateForme.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PlateForme::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_PlateForme::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_PlateForme( type, nID, army );
}
