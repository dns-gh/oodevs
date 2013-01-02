//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint_Lima.cpp $
// $Author: Age $
// $Modtime: 3/02/05 18:19 $
// $Revision: 3 $
// $Workfile: Rep_PathPoint_Lima.cpp $
//
//*****************************************************************************

#include "Rep_PathPoint_Lima.h"

using namespace sword::movement;

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint_Lima constructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
Rep_PathPoint_Lima::Rep_PathPoint_Lima( const MT_Vector2D& vPos, const TerrainData& nTypeTerrain, unsigned int nLimaID, int typeLima )
    : Rep_PathPoint( vPos, Rep_PathPoint::eTypePointLima, nTypeTerrain, "Rep_PointLima" )
    , typeLima_( typeLima )
    , limaID_  ( nLimaID )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint_Lima destructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
Rep_PathPoint_Lima::~Rep_PathPoint_Lima()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Rep_PathPoint_Lima::GetTypeLima
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int Rep_PathPoint_Lima::GetTypeLima() const
{
    return typeLima_;
}

// -----------------------------------------------------------------------------
// Name: Rep_PathPoint_Lima::GetLimaID
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
unsigned int Rep_PathPoint_Lima::GetLimaID() const
{
    return limaID_;
}
