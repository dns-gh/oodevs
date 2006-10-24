//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_PosteControle.inl $
// $Author: Nld $
// $Modtime: 17/02/05 11:31 $
// $Revision: 1 $
// $Workfile: MIL_PosteControle.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PosteControle::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_PosteControle::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_PosteControle( type, nID, army );
}
