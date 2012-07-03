//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint_Special.cpp $
// $Author: Age $
// $Modtime: 3/02/05 18:20 $
// $Revision: 3 $
// $Workfile: Rep_PathPoint_Special.cpp $
//
//*****************************************************************************

#include "Rep_PathPoint_Special.h"

using namespace sword::movement;

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint_Special constructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
Rep_PathPoint_Special::Rep_PathPoint_Special( const MT_Vector2D& vPos, E_TypePointParticulier nTypePoint, const TerrainData& nTypeTerrain )
    : Rep_PathPoint     ( vPos, Rep_PathPoint::eTypePointParticulier, nTypeTerrain, "Rep_PointParticulier" )
    , nTypePointParticulier_( nTypePoint )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint_Special destructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
Rep_PathPoint_Special::~Rep_PathPoint_Special()
{
    // NOTHING
}
