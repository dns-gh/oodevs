//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 6 $
// $Workfile: Rep_PathPoint.cpp $
//
//*****************************************************************************

#include "Rep_PathPoint.h"

using namespace sword;
using namespace sword::movement;

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
Rep_PathPoint::Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType )
    : PathPoint( vPos, eTypePointSpecial, nTypePoint, szDIARepType )
    , nTypeTerrain_( nTypeTerrain )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
Rep_PathPoint::~Rep_PathPoint()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint::GetTypeTerrain
// Created: AGN 03-03-25
//-----------------------------------------------------------------------------
const TerrainData& Rep_PathPoint::GetTypeTerrain() const
{
    return nTypeTerrain_;
}
