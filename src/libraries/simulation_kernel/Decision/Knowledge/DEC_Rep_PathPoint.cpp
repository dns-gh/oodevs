//*****************************************************************************
// 
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 16:39 $
// $Revision: 6 $
// $Workfile: DEC_Rep_PathPoint.cpp $
// 
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Rep_PathPoint.h"

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Representations.h"

int DEC_Rep_PathPoint::nDIAPointIdx_  = 0;
int DEC_Rep_PathPoint::nDIAClsIdx_    = 0;
int DEC_Rep_PathPoint::nDIATypeIdx_   = 0;
int DEC_Rep_PathPoint::nDIAavtIdx_    = 0;

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::InitializeDIA
// Created: NLD 2003-01-13
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "Rep_Point" );
    nDIAPointIdx_     = DEC_Tools::InitializeDIAField( "point_"     , diaType );
    nDIAClsIdx_       = DEC_Tools::InitializeDIAField( "cls_"       , diaType );
    nDIATypeIdx_      = DEC_Tools::InitializeDIAField( "eType_"     , diaType );
    nDIAavtIdx_       = DEC_Tools::InitializeDIAField( "avt_"       , diaType );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::DEC_Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType )
    : DEC_PathPoint     ( vPos )
    , DIA_TypedObject   ( *DIA_TypeManager::Instance().GetType( szDIARepType ), "$$$ nom tmp" )
    , nTypePoint_       ( nTypePoint )
    , nTypeTerrain_     ( nTypeTerrain )
    , pSentToDiaAgent_  ( 0 )
{
    assert( nType_ != eTypePointPath );

    GetVariable( nDIAPointIdx_ ).SetValue( (void*)&vPos_, &DEC_Tools::GetTypePoint(), 1 ); 
    SetValue( nDIAClsIdx_   , ePoint     );     
    SetValue( nDIATypeIdx_  , nTypePoint );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::~DEC_Rep_PathPoint()
{
    if( pSentToDiaAgent_ )
    {
        pSentToDiaAgent_->RemoveFromCategory   ( "points_interressants", const_cast< DEC_Rep_PathPoint* >( this ) );
        pSentToDiaAgent_->RemoveAllReferencesOf( *this );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::SendToDIA
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint::SendToDIA( DEC_Representations& agent ) const
{
    if( pSentToDiaAgent_ )
        return;
        
    // ATTENTION, si cette fonction est appel�e, alors l'agent physique s'est automatiquement arr�t� sur la position du point...
    agent.AddToCategory( "points_interressants", const_cast< DEC_Rep_PathPoint* >( this ) );

    pSentToDiaAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::Dump
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Rep_PathPoint::Dump() const
{
    std::cout << "    Rep_PathPoint " << vPos_ 
              << " - Type : " << nObjectTypes_.DumpToString() 
              << " - TypeToNext " << nObjectTypesToNextPoint_.DumpToString() 
              << " - Type terrain " << nTypeTerrain_.DumpToString() 
              << " - nTypePoint_ " << (uint)nTypePoint_
              << std::endl;
}

