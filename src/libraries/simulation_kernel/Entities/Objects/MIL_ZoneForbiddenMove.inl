//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ZoneForbiddenMove.inl $
// $Author: Nld $
// $Modtime: 21/03/05 15:54 $
// $Revision: 1 $
// $Workfile: MIL_ZoneForbiddenMove.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_ZoneForbiddenMove::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_ZoneForbiddenMove::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_ZoneForbiddenMove( type, nID, army );
}
