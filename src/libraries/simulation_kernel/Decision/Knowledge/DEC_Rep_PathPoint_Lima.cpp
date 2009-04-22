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

int DEC_Rep_PathPoint_Lima::nDIAIdxLima_     = 0;
int DEC_Rep_PathPoint_Lima::nDIAIdxTypeLima_ = 0;

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima::InitializeDIA
// Created: NLD 2003-01-13
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Lima::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "Rep_PointLima" );
    nDIAIdxTypeLima_ = DEC_Tools::InitializeDIAField( "nTypeLima_", diaType );
    nDIAIdxLima_     = DEC_Tools::InitializeDIAField( "lima_"     , diaType );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima constructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Lima::DEC_Rep_PathPoint_Lima( const MT_Vector2D& vPos, const TerrainData& nTypeTerrain, uint nLimaID, const MIL_LimaFunction& function )
    : DEC_Rep_PathPoint( vPos, DEC_Rep_PathPoint::eTypePointLima, nTypeTerrain, "Rep_PointLima" )
    , typeLima_( (int)function.GetID() )
    , limaID_  ( (void*)nLimaID )
{
    SetValue( nDIAIdxTypeLima_, (int)function.GetID() );
    GetVariable( nDIAIdxLima_ ).SetValue( (void*)nLimaID, &DEC_Tools::GetTypeLima() );
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
              << " - nTypePoint_ " << (uint)GetTypePoint()
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
void* DEC_Rep_PathPoint_Lima::GetLimaID()
{
    return limaID_;
}

