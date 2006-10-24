//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_TerrainLargage.inl $
// $Author: Nld $
// $Modtime: 14/03/05 18:12 $
// $Revision: 1 $
// $Workfile: MIL_TerrainLargage.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_TerrainLargage::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_TerrainLargage::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_TerrainLargage( type, nID, army );
}
