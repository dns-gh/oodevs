// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_GeometryFunctions.inl $
// $Author: Nld $
// $Modtime: 7/06/05 9:59 $
// $Revision: 20 $
// $Workfile: DEC_GeometryFunctions.inl $
//
// *****************************************************************************

#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_PathFindManager.h"

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetPosition
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_GeometryFunctions::GetPosition( const MIL_AgentPion& pion )
{
    return pion.GetRole< PHY_RolePion_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetPosition
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_GeometryFunctions::GetPosition( const MIL_Automate& automate )
{
    return GetPosition( automate.GetPionPC() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: NLD 2003-08-21
// Modified: JVT 2004-11-03
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::SplitLocalisationInParts( DIA_Call_ABC& diaCall, const T& caller )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaCall.GetParameter( 0 ) ) );

    DIA_Parameters& diaParams = diaCall.GetParameters();

    TER_Localisation*   pLocalisation =       diaParams[0].ToUserPtr( pLocalisation );
    const uint          nNbrParts     = (uint)diaParams[1].ToFloat();
    DIA_Variable_ABC&   diaReturnCode =       diaParams[2];

    assert( pLocalisation );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( diaCall.GetResult() );

    TER_Localisation clippedLocalisation;
    if ( !ClipLocalisationInFuseau( *pLocalisation, caller.GetFuseau(), clippedLocalisation ) )
    {
        diaReturnCode.SetValue( eError_LocalisationPasDansFuseau );
        diaObjectList.SetValueUserType( T_LocalisationPtrVector(), DEC_Tools::GetTypeLocalisation() );
        return; 
    }
    SplitLocalisation( *pLocalisation, nNbrParts, diaReturnCode, diaObjectList );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisationInSurfaces
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::SplitLocalisationInSurfaces( DIA_Call_ABC& diaCall, const T& caller )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaCall.GetParameter( 0 ) ) );

    DIA_Parameters& diaParams = diaCall.GetParameters();
    
    TER_Localisation* pLocalisation = diaParams[ 0 ].ToUserPtr( pLocalisation );
    const MT_Float    rAverageArea  = MIL_Tools::ConvertMeterSquareToSim( diaParams[ 1 ].ToFloat() );
    DIA_Variable_ABC& diaReturnCode = diaParams[ 2 ];
    
    assert( pLocalisation );

    DIA_Variable_ObjectList& diaObjectList = static_cast< DIA_Variable_ObjectList& >( diaCall.GetResult() );
    
    TER_Localisation clippedLocalisation;
    if ( !ClipLocalisationInFuseau( *pLocalisation, caller.GetFuseau(), clippedLocalisation ) )
    {
        diaReturnCode.SetValue( eError_LocalisationPasDansFuseau );
        diaObjectList.SetValueUserType( T_LocalisationPtrVector(), DEC_Tools::GetTypeLocalisation() );
        return;
    }
    
    const uint nNbrParts = std::max( (uint)1, (uint)( pLocalisation->GetArea() / rAverageArea ) );
    SplitLocalisation( *pLocalisation, nNbrParts, diaReturnCode, diaObjectList );
}

// -----------------------------------------------------------------------------
// Name:DEC_GeometryFunctions::SplitLocalisationInSections
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::SplitLocalisationInSections( DIA_Call_ABC& call, const T& caller )
{
    const MT_Float                 rSectionLength = MIL_Tools::ConvertMeterToSim( call.GetParameter( 0 ).ToFloat() );
          DIA_Variable_ObjectList& diaObjectList  = static_cast< DIA_Variable_ObjectList& >( call.GetResult() );

    MT_Vector2D vDirection( caller.GetFuseau().GetGlobalDirection().GetPosEnd() - caller.GetFuseau().GetGlobalDirection().GetPosStart() );
    vDirection.Normalize();

    SplitLocalisation( TER_Localisation( TER_Localisation::ePolygon, caller.GetFuseau().GetBorderPoints() ), caller.GetFuseau().GetGlobalDirection().GetPosStart(), vDirection , rSectionLength, diaObjectList );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau
// Created: NLD 2003-09-17
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau( DIA_Call_ABC& diaCall, const T& caller )
{
    assert( DEC_Tools::CheckTypeLocalisation( diaCall.GetParameter( 0 ) ) );

    DIA_Parameters& diaParams = diaCall.GetParameters();

    TER_Localisation*   pLocalisation = diaParams[0].ToUserPtr( pLocalisation );
    DIA_Variable_ABC&   diaReturnCode = diaParams[1];

    // 1. Clippe le polygone dans le fuseau
    T_PointVector clippedPointVector;
    pLocalisation->GetPointsClippedByPolygon( caller.GetFuseau(), clippedPointVector );
    // 2. Barycentre polygone clippé
    MT_Vector2D vBarycenter = MT_ComputeBarycenter( clippedPointVector );

    if( clippedPointVector.empty() || ! caller.GetFuseau().IsInside( vBarycenter ) )
    {
        diaReturnCode.SetValue( eError_LocalisationPasDansFuseau );
        diaCall.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return; 
    }

    assert( TER_World::GetWorld().IsValidPosition( vBarycenter ) );
    // 3. Envoi du résulat à DIA
    MT_Vector2D* pOutVector = new MT_Vector2D( vBarycenter );
    diaCall.GetResult().SetValue( (void*)pOutVector, &DEC_Tools::GetTypePoint() );
    diaReturnCode.SetValue( eNoError );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::ComputeDestPoint( DIA_Call_ABC& call, const T& caller )
{
    MT_Vector2D* pResult = new MT_Vector2D(); //$$$$ RAM
    caller.GetFuseau().ComputeFurthestExtremityPoint( *pResult );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeStartPoint
// Created: NLD 2004-05-24
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::ComputeStartPoint( DIA_Call_ABC& call, const T& caller )
{
    MT_Vector2D* pResult = new MT_Vector2D(); //$$$$ RAM
    caller.GetFuseau().ComputeClosestExtremityPoint( *pResult );
    call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
}

// =============================================================================
// $$$$ NLD - A TRIER - DEGUEU
// =============================================================================
struct sBestNodeForObstacle
{
    sBestNodeForObstacle( const MIL_Fuseau& fuseau, const MIL_RealObjectType& objectType, const MT_Vector2D& vCenter, MT_Float rRadius )
        : fuseau_      ( fuseau )
        , objectType_  ( objectType )
        , center_      ( vCenter )
        , rSquareRadius_( rRadius * rRadius )
        , bestPos_()
        , nLastScore_( std::numeric_limits< int >::min() )
    {
    }
    void Visit( const MT_Vector2D& pos, const TerrainData& nPassability )
    {
        const MT_Float rTestNodeSquareDistance = center_.SquareDistance( pos );
        if( rTestNodeSquareDistance > rSquareRadius_ || !fuseau_.IsInside( pos ) )
            return;

        const int nTestNodeScore = objectType_.ComputePlacementScore( pos, nPassability );
        if( nTestNodeScore == -1 )
            return;

        if( nLastScore_ == std::numeric_limits< int >::min()
         || nTestNodeScore  > nLastScore_
         || ( nTestNodeScore  == nLastScore_ && rTestNodeSquareDistance < center_.SquareDistance( bestPos_ ) ) )
        {
            nLastScore_ = nTestNodeScore;
            bestPos_ = pos;
        }
    }
    bool FoundAPoint() const { return nLastScore_ != std::numeric_limits< int >::min(); };
    const MT_Vector2D& BestPosition() const { return bestPos_; };

private:
    sBestNodeForObstacle& operator=( const sBestNodeForObstacle& );
    const MIL_Fuseau&         fuseau_;
    const MIL_RealObjectType& objectType_;
    const MT_Vector2D&        center_;
    MT_Float                  rSquareRadius_;
    MT_Vector2D               bestPos_;
    int                       nLastScore_;
};

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeObstaclePosition
// Created: NLD 2003-09-18
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::ComputeObstaclePosition( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 1 ) ) );

    MT_Vector2D*        pCenter = call.GetParameter( 1 ).ToUserPtr( pCenter );
    MT_Float            rRadius = MIL_Tools::ConvertMeterToSim( call.GetParameter( 2 ).ToFloat() );

    assert( pCenter );

    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( call.GetParameter( 0 ).ToId() );
    assert( pObjectType );

    MT_Vector2D* pResultPos = new MT_Vector2D();
    call.GetResult().SetValue( (void*)pResultPos, &DEC_Tools::GetTypePoint() );

    sBestNodeForObstacle  costEvaluationFunctor( caller.GetFuseau(), *pObjectType, *pCenter, rRadius );
    TER_World::GetWorld().GetPathFindManager().ApplyOnNodesWithinCircle( *pCenter, rRadius, costEvaluationFunctor );
    const bool bOut = costEvaluationFunctor.FoundAPoint();

    if( bOut )
        *pResultPos = costEvaluationFunctor.BestPosition();
    else
        *pResultPos = *pCenter;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInFuseau
// Created: AGN 03-03-11
//-----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::IsPointInFuseau( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    MT_Vector2D* pVect = call.GetParameter( 0 ).ToUserPtr( pVect );
    assert( pVect != 0 );
    call.GetResult().SetValue( caller.GetFuseau().IsInside( *pVect ) );
}

// =============================================================================
// ALAT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetFrontestPion
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
inline
DEC_RolePion_Decision* DEC_GeometryFunctions::GetFrontestPion( const T_ObjectVector& pions, const MT_Vector2D& vDirection )
{
    const MT_Line          support( MT_Vector2D( 0., 0. ), vDirection );    
    DEC_RolePion_Decision* pResult = 0;
    MT_Float               rSquareDistResult = -1.;
           
    for ( CIT_ObjectVector it = pions.begin(); it != pions.end(); ++it )
    {
        DEC_RolePion_Decision* pKnow = static_cast< DEC_RolePion_Decision* >( *it );
        assert( pKnow );
        
        MT_Vector2D vProjectedPoint;
        MT_Float    rSquareDist;
        
        const bool bInDirection = support.ProjectPointOnLine( GetPosition( pKnow->GetPion() ), vProjectedPoint ) >= 0.;
        
        rSquareDist = vProjectedPoint.rX_ * vProjectedPoint.rX_ + vProjectedPoint.rY_ * vProjectedPoint.rY_;
        if ( !pResult || ( bInDirection &&  rSquareDist > rSquareDistResult ) || ( !bInDirection && rSquareDist < rSquareDistResult )  )
        {
            rSquareDistResult = rSquareDist;
            pResult           = pKnow;
        }        
    }
    
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeFrontestAgent
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
template< typename T >
inline
void DEC_GeometryFunctions::ComputeFrontestAgent( DIA_Call_ABC& call, const T& )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeDirection ( call.GetParameter( 1 ) ) );
    
    const T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();
    
    if( pions.empty() )
    {
        assert( !"La liste ne doit pas etre vide" );
        call.GetResult().SetValue( *(DEC_RolePion_Decision*)0 );
        return;
    }
    
    MT_Vector2D* pDirection = call.GetParameter( 1 ).ToUserPtr( pDirection );
    assert( pDirection );

    call.GetResult().SetValue( *GetFrontestPion( pions, *pDirection ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeBackestAgent
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
template< typename T > 
inline
void DEC_GeometryFunctions::ComputeBackestAgent( DIA_Call_ABC& call, const T& )
{
    assert( DEC_Tools::CheckTypeListePions( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeDirection ( call.GetParameter( 1 ) ) );
    
    const T_ObjectVector pions = call.GetParameter( 0 ).ToSelection();
    
    if ( pions.empty() )
    {
        assert( !"La liste ne doit pas etre vide" );
        call.GetResult().SetValue( *(DEC_RolePion_Decision*)0 );
        return;
    }
    
    MT_Vector2D* pDirection = call.GetParameter( 1 ).ToUserPtr( pDirection );
    assert( pDirection );
    
    MT_Vector2D vNewDirection = *pDirection;
    vNewDirection *= -1.;
    call.GetResult().SetValue( *GetFrontestPion( pions, vNewDirection ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePointBeforeLima
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
template< typename T >
inline
void DEC_GeometryFunctions::ComputePointBeforeLima( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLima( call.GetParameter( 0 ) ) );

    DIA_Parameters& diaParams = call.GetParameters();

    MIL_Lima*         pLima           = diaParams[0].ToUserPtr( pLima );
    MT_Float          rDistBeforeLima = MIL_Tools::ConvertMeterToSim( diaParams[1].ToFloat() );
    DIA_Variable_ABC& diaReturnCode   = diaParams[2];

    if( !pLima )
    {
        diaReturnCode.SetValue( false );
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return;
    }

    MT_Vector2D vResult;

    bool bResult = caller.GetFuseau().ComputePointBeforeLima( *pLima, rDistBeforeLima, vResult );
    diaReturnCode.SetValue( bResult );
    if( bResult )
    {
        MT_Vector2D* pResult = new MT_Vector2D( vResult ); //$$$$ RAM
        call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
    }
    else
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
template< typename T >
inline
void DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint       ( call.GetParameter( 1 ) ) );

    DIA_Parameters& param = call.GetParameters();
    TER_Localisation* pLocalisation = param[0].ToUserPtr( pLocalisation );
    assert( pLocalisation );

    TER_Localisation clippedLocalisation;
    if ( !ClipLocalisationInFuseau( *pLocalisation, caller.GetFuseau(), clippedLocalisation ) )
    {
        param.GetParameter( 1 ).SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        call.GetResult().SetValue( false );
        return; 
    }

    MT_Vector2D* pResult = new MT_Vector2D(); //$$$$ TMP
    bool bOut = clippedLocalisation.ComputeNearestPoint( GetPosition( caller ), *pResult );
    param.GetParameter( 1 ).SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
    call.GetResult().SetValue( bOut );
}
