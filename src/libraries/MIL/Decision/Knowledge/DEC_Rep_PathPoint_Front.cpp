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

#include "MIL_pch.h"

#include "DEC_Rep_PathPoint_Front.h"

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Decision/DEC_Tools.h"

int DEC_Rep_PathPoint_Front::nDIAPointIdx_ = 0;
int DEC_Rep_PathPoint_Front::nDIAClsIdx_   = 0;
int DEC_Rep_PathPoint_Front::nDIADestIdx_  = 0;

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front constructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::DEC_Rep_PathPoint_Front( const MT_Vector2D& vPos, DEC_Rep_PathPoint& dest )
    : DEC_PathPoint     ( vPos )
    , DIA_Representation( "$$$ nom tmp", *DIA_TypeManager::Instance().GetType( "Rep_AvantPoint" ) )
    , bAlreadySent_     ( false )
    , destPoint_        ( dest )
{
    SetValue      ( nDIAPointIdx_    , (void*)&vPos_ );  
    SetValue      ( nDIAClsIdx_      , eAvantPoint   );     
    SetObjectValue( nDIADestIdx_     , &destPoint_   );

    diaParameters_.SetOwnerShip( true );
    diaParameters_.AddParam( new DIA_Variable_Object() );  //$$$$$ Gestion mémoire
}


//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front constructor
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::DEC_Rep_PathPoint_Front( const DEC_Rep_PathPoint_Front& rhs )
    : DEC_PathPoint     ( rhs.vPos_ )
    , DIA_Representation( "$$$ nom tmp", *DIA_TypeManager::Instance().GetType( "Rep_AvantPoint" ) )
    , destPoint_        ( rhs.destPoint_ )
{
    SetValue      ( nDIAPointIdx_, (void*)&vPos_ );  
    SetValue      ( nDIAClsIdx_  , eAvantPoint   );     
    SetObjectValue( nDIADestIdx_ , &destPoint_   );

    diaParameters_.SetOwnerShip( true );
    diaParameters_.AddParam( new DIA_Variable_Object() );  //$$$$$ Gestion mémoire
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint_Front::~DEC_Rep_PathPoint_Front()
{
    
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
void DEC_Rep_PathPoint_Front::SendToDIA( DEC_RolePion_Decision& agent )
{
    if ( bAlreadySent_ )
        return;

    diaParameters_.GetParameter( 0 ).SetValue( *this );
    DIA_Variable_ABC* pResult = agent.ExecuteScriptFunction( "EVT_DEC_Point", diaParameters_ );
    if( pResult ) 
        delete pResult;
    bAlreadySent_ = true;
}


//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint_Front::RemoveFromDIA
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint_Front::RemoveFromDIA( DEC_RolePion_Decision& agent )
{
    agent.GetKnowledgePart().RemoveFromCategory( "points_interressants", static_cast< DIA_Representation* >( this ) );
    agent.GetBehaviorPart ().RemoveAllReferencesOf( *this, agent.GetContext() );    
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

