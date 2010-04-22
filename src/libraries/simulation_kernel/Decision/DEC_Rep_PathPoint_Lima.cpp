//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint_Lima.cpp $
// $Author: Age $
// $Modtime: 3/02/05 18:19 $
// $Revision: 3 $
// $Workfile: DEC_Rep_PathPoint_Lima.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Rep_PathPoint_Lima.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Decision/DEC_Tools.h"

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima constructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Lima::DEC_Rep_PathPoint_Lima( const MT_Vector2D& vPos, const TerrainData& nTypeTerrain, unsigned int nLimaID, const MIL_LimaFunction& function )
    : DEC_Rep_PathPoint( vPos, DEC_Rep_PathPoint::eTypePointLima, nTypeTerrain, "Rep_PointLima" )
    , typeLima_( (int)function.GetID() )
    , limaID_  ( nLimaID )
{
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima destructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Lima::~DEC_Rep_PathPoint_Lima()
{   
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima::Dump
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Lima::Dump() const
{
    std::cout << "    DEC_Rep_PathPoint_Lima " << vPos_ 
              << " - Type : " << nObjectTypes_.DumpToString() 
              << " - TypeToNext " << nObjectTypesToNextPoint_.DumpToString() 
              << " - nTypePoint_ " << (unsigned int)GetTypePoint()
              << std::endl;
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima::GetTypeLima
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_Rep_PathPoint_Lima::GetTypeLima()
{
    return typeLima_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima::GetLimaID
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
unsigned int DEC_Rep_PathPoint_Lima::GetLimaID()
{
    return limaID_;
}

