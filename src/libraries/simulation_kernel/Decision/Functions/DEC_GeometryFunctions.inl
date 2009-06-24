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
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Objective.h"

#include "Entities/Objects/TerrainHeuristicCapacity.h"


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

namespace
{
    // =============================================================================
    // $$$$ NLD - A TRIER - DEGUEU
    // =============================================================================
    struct sBestNodeForObstacle
    {
        sBestNodeForObstacle( const MIL_Fuseau& fuseau, const TerrainHeuristicCapacity& heuristic, const MT_Vector2D& vCenter, MT_Float rRadius )
            : fuseau_      ( fuseau )
            , heuristic_   ( heuristic )
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
            
            const int nTestNodeScore = heuristic_.ComputePlacementScore( pos, nPassability );
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
        const TerrainHeuristicCapacity& heuristic_;    
        const MT_Vector2D&        center_;
        MT_Float                  rSquareRadius_;
        MT_Vector2D               bestPos_;
        int                       nLastScore_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeObstaclePosition
// Created: NLD 2003-09-18
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::ComputeObstaclePosition( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypePoint( call[ 0 ] ) );

    MT_Vector2D*        pCenter = call[ 0 ].ToUserPtr( pCenter );
    const std::string&  type = call[ 1 ].ToString();
    MT_Float            rRadius = MIL_Tools::ConvertMeterToSim( call[ 2 ].ToFloat() );

    assert( pCenter );
    
    MT_Vector2D* pResultPos = new MT_Vector2D();
    call.GetResult().SetValue( (void*)pResultPos, &DEC_Tools::GetTypePoint() );

    *pResultPos = *pCenter;
    try 
    {
        const MIL_ObjectType_ABC& object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObjectType( type );
        const TerrainHeuristicCapacity* pCapacity = object.GetCapacity< TerrainHeuristicCapacity >();
        if ( pCapacity )
        {
            sBestNodeForObstacle  costEvaluationFunctor( caller.GetFuseau(), *pCapacity, *pCenter, rRadius );
            TER_World::GetWorld().GetPathFindManager().ApplyOnNodesWithinCircle( *pCenter, rRadius, costEvaluationFunctor );        
            if( costEvaluationFunctor.FoundAPoint() )
                *pResultPos = costEvaluationFunctor.BestPosition();
        }
    }
    catch ( std::exception& e )
    {
        // object not found
    }
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
        call.GetResult().Reset(); // $$$$ SetValue( *(DEC_RolePion_Decision*)0 );
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
        call.GetResult().Reset(); // $$$ SetValue( *(DEC_RolePion_Decision*)0 );
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

    MIL_LimaOrder*    pLima           = caller.FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    MT_Float          rDistBeforeLima = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
    DIA_Variable_ABC& diaReturnCode   = call.GetParameter( 2 );

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
// Name: template< typename T > static void DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau
// Created: SBO 2008-01-11
// -----------------------------------------------------------------------------
template< typename T >
void DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLima  ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 2 ) ) );

    MIL_LimaOrder*    pLima           = caller.FindLima( (uint)call.GetParameter( 0 ).ToPtr() );
    MT_Float          rDistBeforeLima = MIL_Tools::ConvertMeterToSim( call.GetParameter( 1 ).ToFloat() );
    const MIL_Fuseau* pFuseau         = call.GetParameter( 2 ).ToUserPtr( pFuseau );

    if( !pLima || !pFuseau )
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
    else
    {
        MT_Vector2D vResult;
        if( pFuseau->ComputePointBeforeLima( *pLima, rDistBeforeLima, vResult ) )
        {
            MT_Vector2D* pResult = new MT_Vector2D( vResult ); //$$$$ RAM
            call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
        }
        else
            call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
    }
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

    DIA_Parameters& param = call.GetParameters();
    TER_Localisation* pLocalisation = param[0].ToUserPtr( pLocalisation );
    assert( pLocalisation );

    TER_Localisation clippedLocalisation;
    if ( !ClipLocalisationInFuseau( *pLocalisation, caller.GetFuseau(), clippedLocalisation ) )
    {
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
        return; 
    }

    MT_Vector2D* pResult = new MT_Vector2D(); //$$$$ TMP
    bool bOut = clippedLocalisation.ComputeNearestPoint( GetPosition( caller ), *pResult );
    if( bOut)
        call.GetResult().SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
    else
    {
        delete pResult;
        call.GetResult().SetValue( (void*)0, &DEC_Tools::GetTypePoint() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau
// Created: GGR 2006-09-27
// -----------------------------------------------------------------------------
template< typename T > inline
void DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau( DIA_Call_ABC& call, const T& caller )
{
    assert( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypePoint       ( call.GetParameter( 1 ) ) );

    DIA_Parameters& param = call.GetParameters();
    TER_Localisation* pLocalisation = param[0].ToUserPtr( pLocalisation );
    assert( pLocalisation );

    TER_Localisation fuseauLocalisation = TER_Localisation( TER_Localisation::ePolygon, caller.GetFuseau().GetBorderPoints() );

    MT_Vector2D* pResult = new MT_Vector2D(); //$$$$ TMP
    bool bOut;

    bOut = fuseauLocalisation.ComputeNearestPoint( pLocalisation->ComputeBarycenter(), *pResult );
    param.GetParameter( 1 ).SetValue( (void*)pResult, &DEC_Tools::GetTypePoint() );
    call.GetResult().SetValue( bOut );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_GeometryFunctions::SortFuseauxAccordingToSchedule
// Created: NLD 2007-05-14
// -----------------------------------------------------------------------------
template< typename T > inline
void DEC_GeometryFunctions::SortFuseauxAccordingToSchedule( DIA_Call_ABC& call, const T& caller )
{   
    assert( DEC_Tools::CheckTypeListeFuseaux( call.GetParameter( 0 ) ) );   
    assert( DEC_Tools::CheckTypePoint       ( call.GetParameter( 1 ) ) );
    assert( DEC_Tools::CheckTypeLima        ( call.GetParameter( 2 ) ) || DEC_Tools::CheckTypeObjectif( call.GetParameter( 2 ) ) );

    const MT_Vector2D* pRefPoint = call.GetParameter( 1 ).ToUserPtr( pRefPoint );
    assert( pRefPoint );

    call.GetResult() = call.GetParameter( 0 );

    const MIL_LimaOrder* pLima      = 0;
    const DEC_Objective* pObjective = 0;
    if( DEC_Tools::CheckTypeLima( call.GetParameter( 2 ) ) )
        pLima = caller.FindLima( (uint)call.GetParameter( 2 ).ToPtr() ); 
    else if( DEC_Tools::CheckTypeObjectif( call.GetParameter( 2 ) ) )
        pObjective = call.GetParameter( 2 ).ToUserPtr( pObjective );
    if( !pObjective && !pLima )
        return;

    std::multimap< MT_Float, DIA_Variable_ABC* > sortedFuseaux;
    T_ObjectVariableVector& fuseaux = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetResult() ).GetContainer() );
    for( CIT_ObjectVariableVector it = fuseaux.begin(); it != fuseaux.end(); ++it )
    {
        const MIL_Fuseau* pFuseau = (**it).ToUserPtr( pFuseau );
        if( pLima )        
            sortedFuseaux.insert( std::make_pair( pFuseau->ComputeAverageDistanceFromLima( *pLima, *pRefPoint ), *it ) );
        else // if( pObjective )
            sortedFuseaux.insert( std::make_pair( pFuseau->ComputeAverageDistanceFromObjective( *pObjective, *pRefPoint ), *it ) );
    }

    fuseaux.clear();
    for( std::multimap< MT_Float, DIA_Variable_ABC* >::const_iterator it = sortedFuseaux.begin(); it != sortedFuseaux.end(); ++it )
        fuseaux.push_back( it->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDelayFromSchedule
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
template< typename T > inline
void DEC_GeometryFunctions::ComputeDelayFromSchedule( DIA_Call_ABC& call, const T& caller )
{   
    assert( DEC_Tools::CheckTypeFuseau        ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeListeAutomates( call.GetParameter( 1 ) ) );
    assert( DEC_Tools::CheckTypeLima          ( call.GetParameter( 2 ) ) || DEC_Tools::CheckTypeObjectif( call.GetParameter( 2 ) ) );

    const MIL_Fuseau*    pFuseau   = call.GetParameter( 0 ).ToUserPtr( pFuseau );
    const T_ObjectVector automates = call.GetParameter( 1 ).ToSelection();
    assert( pFuseau );

    // Calcul distance entre barycentre automates et element schedulé
    MT_Float rDistanceFromScheduled = std::numeric_limits< MT_Float >::max();
    uint     nSchedule              = 0;
    if( DEC_Tools::CheckTypeLima( call.GetParameter( 2 ) ) )
    {
        const MIL_LimaOrder* pLima = caller.FindLima( (uint)call.GetParameter( 2 ).ToPtr() );
        if( pLima ) 
        {
            rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromLima( *pLima, _ComputeAutomatesBarycenter( automates ) );
            nSchedule = pLima->GetSchedule();
        }
    }
    else if( DEC_Tools::CheckTypeObjectif( call.GetParameter( 2 ) ) )
    {
        const DEC_Objective* pObjective = call.GetParameter( 2 ).ToUserPtr( pObjective );
        if( pObjective )
        {
            rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromObjective( *pObjective, _ComputeAutomatesBarycenter( automates ) );
            nSchedule = pObjective->GetSchedule();
        }
    }

    // Calcul vitesse moyenne de l'automate
    MT_Float rSpeed = std::numeric_limits< MT_Float >::max();
    for( CIT_ObjectVector it = automates.begin(); it != automates.end(); ++it )
    {
        const MIL_Automate& automate = static_cast< DEC_AutomateDecision& >( **it ).GetAutomate();
        rSpeed = std::min( rSpeed, automate.GetAlivePionsMaxSpeed() );
    }

    if( rDistanceFromScheduled == std::numeric_limits< MT_Float >::max() || rSpeed == 0. )
    {
        call.GetResult().SetValue( (float)0. );
        return;
    }

    const MT_Float rTimeToGoToElement = 1.439 * rDistanceFromScheduled / rSpeed; //$$$ Deplacer la formule magique (Cf. PHY_ComposantePion où elle existe aussi...)
    const MT_Float rTimeLeeway        = 1.439 * 2000. / rSpeed;

    // Valeur de retour : = 0 : en avance, ou à 2km de la lima
    //                    = 1 : en retard
    //              entre les 2 : marge de sécurité

    const MT_Float rDelay = nSchedule - ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + rTimeToGoToElement );
    if( rDelay < 0 )
        call.GetResult().SetValue( (float)1. );
    else
        call.GetResult().SetValue( (float)( 1. - std::min( 1., rDelay / rTimeLeeway ) ) );
}
