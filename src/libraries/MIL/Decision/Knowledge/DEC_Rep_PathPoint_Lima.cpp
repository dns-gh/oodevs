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

#include "MIL_pch.h"
#include "DEC_Rep_PathPoint_Lima.h"
#include "Entities/Orders/Lima/MIL_Lima.h"
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
DEC_Rep_PathPoint_Lima::DEC_Rep_PathPoint_Lima( const MT_Vector2D& vPos, const TerrainData& nTypeTerrain, const MIL_Lima& lima )
    : DEC_Rep_PathPoint( vPos, DEC_Rep_PathPoint::eTypePointLima, nTypeTerrain, "Rep_PointLima" )
    , lima_            ( lima )
{
    SetValue( nDIAIdxTypeLima_, (int)lima_.GetLimaFunction() );
    SetValue( nDIAIdxLima_    , (void*)&lima_                );
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



