//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint_Special.cpp $
// $Author: Age $
// $Modtime: 3/02/05 18:20 $
// $Revision: 3 $
// $Workfile: DEC_Rep_PathPoint_Special.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Rep_PathPoint_Special.h"
#include "MT_Tools/MT_Logger.h"

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Special constructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Special::DEC_Rep_PathPoint_Special( const MT_Vector2D& vPos, E_TypePointParticulier nTypePoint, const TerrainData& nTypeTerrain )
    : DEC_Rep_PathPoint     ( vPos, DEC_Rep_PathPoint::eTypePointParticulier, nTypeTerrain, "Rep_PointParticulier" )
    , nTypePointParticulier_( nTypePoint )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Special destructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Special::~DEC_Rep_PathPoint_Special()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Special::Dump
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Special::Dump() const
{
    MT_LOG_ERROR_MSG( "    DEC_Rep_PathPoint_Special " << vPos_
              << " - Type : " << nObjectTypes_.DumpToString()
              << " - TypeToNext " << nObjectTypesToNextPoint_.DumpToString()
              << " - nTypePoint_ " << (unsigned int)GetTypePoint()
              << " - Type particulier " << (unsigned int)nTypePointParticulier_
              << std::endl );
}
