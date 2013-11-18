// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

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

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::IsSlopeValid
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
inline
    bool DEC_PathPoint::IsSlopeValid() const
{
    return validSlope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::GetSlope
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
inline
    double DEC_PathPoint::GetSlope() const
{
    return slope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathPoint::SetSlope
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
inline
    void DEC_PathPoint::SetSlope( double slope )
{
    validSlope_ = true;
    slope_ = slope;
}
