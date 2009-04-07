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

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Representations.h"

int DEC_Rep_PathPoint_Front::nDIAPointIdx_ = 0;
int DEC_Rep_PathPoint_Front::nDIAClsIdx_   = 0;
int DEC_Rep_PathPoint_Front::nDIADestIdx_  = 0;

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::DEC_Rep_PathPoint_Front( const MT_Vector2D& vPos, DEC_Rep_PathPoint& dest )
    : DEC_PathPoint     ( vPos )
    , DIA_TypedObject   ( *DIA_TypeManager::Instance().GetType( "Rep_AvantPoint" ), "$$$ nom tmp" )
    , destPoint_        ( dest )
    , pSentToDiaAgent_  ( 0 )
{
    GetVariable( nDIAPointIdx_ ).SetValue( (void*)&vPos_, &DEC_Tools::GetTypePoint(), 1 );
    SetValue      ( nDIAClsIdx_      , eAvantPoint );     
    SetObjectValue( nDIADestIdx_     , &destPoint_ );

    diaParameters_.SetOwnerShip( true );
    diaParameters_.AddParam( new DIA_Variable_Object() );  //$$$$$ Gestion mémoire
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::~DEC_Rep_PathPoint_Front()
{
    if( pSentToDiaAgent_ )
    {
        pSentToDiaAgent_->RemoveFromCategory( "points_interressants", const_cast< DEC_Rep_PathPoint_Front* >( this ) );
        pSentToDiaAgent_->RemoveAllReferencesOf( *this );    
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::InitializeDIA
// Created: NLD 2003-01-13
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Front::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "Rep_AvantPoint" );
    nDIAPointIdx_     = DEC_Tools::InitializeDIAField( "point_"     , diaType );
    nDIAClsIdx_       = DEC_Tools::InitializeDIAField( "cls_"       , diaType );
    nDIADestIdx_      = DEC_Tools::InitializeDIAField( "dest_"      , diaType );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::SendToDIA
// Created: JVT 02-12-09
// Last modified: JVT 02-12-16
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Front::SendToDIA( DEC_Representations& agent ) const
{
    if( pSentToDiaAgent_ )
        return;

    agent.AddToCategory( "points_interressants", const_cast< DEC_Rep_PathPoint_Front* >( this ) );

    pSentToDiaAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::Dump
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Front::Dump() const
{
    std::cout << "    DEC_Rep_PathPoint_Front " << vPos_ 
              << " - Type : " << nObjectTypes_.DumpToString() 
              << " - TypeToNext " << nObjectTypesToNextPoint_.DumpToString() 
              << " - DestPoint : " << destPoint_.GetPos()
              << " - Dest Type : " << destPoint_.GetTypeTerrain().DumpToString() 
              << std::endl;
}

