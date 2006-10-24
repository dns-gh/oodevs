//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_FloatingBridgeDiscontinuous.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_FloatingBridgeDiscontinuous.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_FloatingBridgeDiscontinuous::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_FloatingBridgeDiscontinuous( type, nID, army );
}
