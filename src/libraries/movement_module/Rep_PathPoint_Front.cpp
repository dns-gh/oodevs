//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/Rep_PathPoint_Front.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 4 $
// $Workfile: Rep_PathPoint_Front.cpp $
//
//*****************************************************************************

#include "Rep_PathPoint_Front.h"

using namespace sword;
using namespace sword::movement;

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint_Front constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
Rep_PathPoint_Front::Rep_PathPoint_Front( const MT_Vector2D& vPos, boost::shared_ptr< PathPoint > dest )
    : PathPoint( vPos, eTypePointFront, eTypePointNormal, "Rep_AvantPoint" )
    , destPoint_( dest )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Rep_PathPoint_Front destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
Rep_PathPoint_Front::~Rep_PathPoint_Front()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Rep_PathPoint_Front::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
const boost::shared_ptr< PathPoint >& Rep_PathPoint_Front::GetDestPoint() const
{
    return destPoint_;
}
