//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint.inl $
// $Author: Age $
// $Modtime: 3/02/05 18:17 $
// $Revision: 2 $
// $Workfile: DEC_Rep_PathPoint.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::GetTypePoint
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
inline
DEC_Rep_PathPoint::E_TypePoint DEC_Rep_PathPoint::GetTypePoint() const
{
    return nTypePoint_;
}


//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::GetTypeTerrain
// Created: AGN 03-03-25
//-----------------------------------------------------------------------------
inline
const TerrainData& DEC_Rep_PathPoint::GetTypeTerrain() const
{
    return nTypeTerrain_;
}

