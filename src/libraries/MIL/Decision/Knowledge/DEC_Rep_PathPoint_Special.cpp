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

#include "MIL_pch.h"
#include "DEC_Rep_PathPoint_Special.h"
#include "Decision/DEC_Tools.h"

int DEC_Rep_PathPoint_Special::nDIAIdxTypePart_ = 0;

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Lima::InitializeDIA
// Created: NLD 2003-01-13
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Special::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "Rep_PointParticulier" );
    nDIAIdxTypePart_ = DEC_Tools::InitializeDIAField( "eTypePart_", diaType );
}


//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Special constructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Special::DEC_Rep_PathPoint_Special( DEC_Path& path, const MT_Vector2D& vPos, E_TypePointParticulier nTypePoint, const TerrainData& nTypeTerrain )
    : DEC_Rep_PathPoint    ( path, vPos, DEC_Rep_PathPoint::eTypePointParticulier, nTypeTerrain, "Rep_PointParticulier" )
    , nTypePointParticulier_ ( nTypePoint )
{
    SetValue( nDIAIdxTypePart_, nTypePointParticulier_ ); 
}


//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Special constructor
// Created: AGN 03-01-29
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Special::DEC_Rep_PathPoint_Special( const MT_Vector2D& vPos, E_TypePointParticulier nTypePoint, const TerrainData& nTypeTerrain )
    : DEC_Rep_PathPoint    ( vPos, DEC_Rep_PathPoint::eTypePointParticulier, nTypeTerrain, "Rep_PointParticulier" )
    , nTypePointParticulier_ ( nTypePoint )
{
    SetValue( nDIAIdxTypePart_, nTypePointParticulier_ ); 
}


//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Special constructor
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Special::DEC_Rep_PathPoint_Special( const DEC_Rep_PathPoint_Special& rhs )
: DEC_Rep_PathPoint( rhs )
, nTypePointParticulier_( rhs.nTypePointParticulier_ )
{
    SetValue( nDIAIdxTypePart_, nTypePointParticulier_ );     
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Special destructor
// Created: JVT 02-12-10
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Special::~DEC_Rep_PathPoint_Special()
{   
}





