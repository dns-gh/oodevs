// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: PathPoint::GetPos
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& PathPoint::GetPos() const
{
    return vPos_;
}

//-----------------------------------------------------------------------------
// Name: PathPoint::GetType
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
inline
PathPoint::E_Type PathPoint::GetType() const
{
    return nType_;
}

//-----------------------------------------------------------------------------
// Name: PathPoint::GetObjectTypes
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
inline
const TerrainData& PathPoint::GetObjectTypes() const
{
    return nObjectTypes_;
}

//-----------------------------------------------------------------------------
// Name: PathPoint::GetObjectTypesToNextPoint
// Created: JVT 02-12-06
//-----------------------------------------------------------------------------
inline
const TerrainData& PathPoint::GetObjectTypesToNextPoint() const
{
    return nObjectTypesToNextPoint_;
}
