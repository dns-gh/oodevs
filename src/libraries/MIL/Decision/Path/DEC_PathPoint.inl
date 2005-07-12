//*****************************************************************************
//
// $Created: JVT 02-12-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathPoint.inl $
// $Author: Age $
// $Modtime: 15/02/05 10:57 $
// $Revision: 4 $
// $Workfile: DEC_PathPoint.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetPos
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_PathPoint::GetPos() const
{
    return vPos_;
}


//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetType
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
inline
DEC_PathPoint::E_Type DEC_PathPoint::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetObjectTypes
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
inline
const TerrainData& DEC_PathPoint::GetObjectTypes() const
{
    return nObjectTypes_;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetObjectTypesToNextPoint
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
inline
const TerrainData& DEC_PathPoint::GetObjectTypesToNextPoint() const
{
    return nObjectTypesToNextPoint_;
}


