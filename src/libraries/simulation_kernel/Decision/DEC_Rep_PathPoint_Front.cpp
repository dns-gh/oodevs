//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint_Front.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 4 $
// $Workfile: DEC_Rep_PathPoint_Front.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Rep_PathPoint_Front.h"

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::DEC_Rep_PathPoint_Front( const MT_Vector2D& vPos, boost::shared_ptr< TER_PathPoint > dest )
    : DEC_DIA_PathPoint ( vPos, eTypePointFront, eTypePointNormal, "Rep_AvantPoint" )
    , destPoint_        ( dest )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::~DEC_Rep_PathPoint_Front()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathPoint > DEC_Rep_PathPoint_Front::GetDestPoint() const
{
    return destPoint_;
}
