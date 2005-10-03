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

#include "MIL_pch.h"
#include "DEC_Rep_PathPoint.h"

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Decision/DEC_Tools.h"

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
    , DIA_Representation( "$$$ nom tmp", *DIA_TypeManager::Instance().GetType( szDIARepType ) )
    , nTypePoint_       ( nTypePoint )
    , nTypeTerrain_     ( nTypeTerrain )
    , bAlreadySent_   ( false )
{
    assert( nType_ != eTypePointPath );

    SetValue( nDIAPointIdx_ , (void*)&vPos_ );  
    SetValue( nDIAClsIdx_   , ePoint        );     
    SetValue( nDIATypeIdx_  , nTypePoint    );  

    diaParameters_.SetOwnerShip( true );
    diaParameters_.AddParam( new DIA_Variable_Object() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint constructor
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::DEC_Rep_PathPoint( const DEC_Rep_PathPoint& rhs )
    : DEC_PathPoint     ( rhs.vPos_ )
    , DIA_Representation( "$$$ nom tmp", rhs.DIA_TypedObject::GetType() )
    , nTypePoint_       ( rhs.nTypePoint_ )
    , nTypeTerrain_     ( rhs.nTypeTerrain_ )
    , bAlreadySent_   ( false )
{
    assert( nType_ != eTypePointPath );

    SetValue( nDIAPointIdx_, (void*)&vPos_ );  
    SetValue( nDIAClsIdx_  , ePoint        );     
    SetValue( nDIATypeIdx_ , nTypePoint_   );     

    diaParameters_.SetOwnerShip( true );
    diaParameters_.AddParam( new DIA_Variable_Object() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint destructor
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
DEC_Rep_PathPoint::~DEC_Rep_PathPoint()
{

}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::SendToDIA
// Created: JVT 02-12-09
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint::SendToDIA( DEC_RolePion_Decision& agent ) const
{
    if ( bAlreadySent_ )
        return;
        
    // ATTENTION, si cette fonction est appelée, alors l'agent physique s'est automatiquement arrêté sur la position du point...
    diaParameters_.GetParameter( 0 ).SetValue( const_cast< DEC_Rep_PathPoint& >( *this ) );
    DIA_Variable_ABC* pResult = agent.ExecuteScriptFunction( "EVT_DEC_Point", diaParameters_ );
    if( pResult ) 
        delete pResult;
    bAlreadySent_ = true;
}

//-----------------------------------------------------------------------------
// Name: DEC_Rep_PathPoint::RemoveFromDIA
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
void DEC_Rep_PathPoint::RemoveFromDIA( DEC_RolePion_Decision& agent ) const
{
    agent.GetKnowledgePart().RemoveFromCategory   ( "points_interressants", const_cast< DEC_Rep_PathPoint* >( this ) );
    agent.GetBehaviorPart ().RemoveAllReferencesOf( *this, agent.GetContext() );
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

