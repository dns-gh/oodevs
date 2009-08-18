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
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
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
    return pion.GetRole< PHY_RoleInterface_Location >().GetPosition();
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
 std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitLocalisationInParts( const T& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts )
{
    assert( pLocalisation );

     std::vector< boost::shared_ptr< TER_Localisation > > result;

    TER_Localisation clippedLocalisation;
    if ( ClipLocalisationInFuseau( *pLocalisation, caller.GetOrderManager().GetFuseau(), clippedLocalisation ) )
        SplitLocalisation( *pLocalisation, nNbrParts, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisationInSurfaces
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
template< typename T >
std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitLocalisationInSurfaces( const T& caller, TER_Localisation* pLocalisation, const MT_Float rAverageArea )
{
    assert( pLocalisation );

    std::vector< boost::shared_ptr< TER_Localisation > > result;
    
    TER_Localisation clippedLocalisation;
    if ( ClipLocalisationInFuseau( *pLocalisation, caller.GetOrderManager().GetFuseau(), clippedLocalisation ) )
    {
        const uint nNbrParts = std::max( (uint)1, (uint)( pLocalisation->GetArea() / rAverageArea ) );
        SplitLocalisation( *pLocalisation, nNbrParts, result );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name:DEC_GeometryFunctions::SplitLocalisationInSections
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
template< typename T >
std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitLocalisationInSections( const T& caller, const MT_Float rSectionLength )
{
    const MT_Line& globalDirection = caller.GetOrderManager().GetFuseau().GetGlobalDirection();
    MT_Vector2D vDirection( globalDirection.GetPosEnd() - globalDirection.GetPosStart() );
    vDirection.Normalize();

    return SplitLocalisation( TER_Localisation( TER_Localisation::ePolygon, caller.GetOrderManager().GetFuseau().GetBorderPoints() ), globalDirection.GetPosStart(), vDirection , rSectionLength );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau
// Created: NLD 2003-09-17
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau( const T& caller, TER_Localisation* pLocalisation )
{
    // 1. Clippe le polygone dans le fuseau
    T_PointVector clippedPointVector;
    pLocalisation->GetPointsClippedByPolygon( caller.GetOrderManager().GetFuseau(), clippedPointVector );
    // 2. Barycentre polygone clippé
    MT_Vector2D vBarycenter = MT_ComputeBarycenter( clippedPointVector );

    boost::shared_ptr< MT_Vector2D > result;

    if( !clippedPointVector.empty() && caller.GetOrderManager().GetFuseau().IsInside( vBarycenter ) )
    {
        assert( TER_World::GetWorld().IsValidPosition( vBarycenter ) );
        // 3. Envoi du résulat à DIA
        result.reset( new MT_Vector2D( vBarycenter ) );
    }
    return result;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeDestPoint( const T& caller )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() ); //$$$$ RAM
    caller.GetOrderManager().GetFuseau().ComputeFurthestExtremityPoint( *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeStartPoint
// Created: NLD 2004-05-24
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeStartPoint( const T& caller )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() ); //$$$$ RAM
    caller.GetOrderManager().GetFuseau().ComputeClosestExtremityPoint( *pResult );
    return pResult;
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
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeObstaclePosition( const T& caller, MT_Vector2D* pCenter, const std::string& type, MT_Float rRadius )
{
    assert( pCenter );
    boost::shared_ptr< MT_Vector2D > pResultPos ( new MT_Vector2D( *pCenter ) );

    const MIL_ObjectType_ABC& object = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObjectType( type );
    const TerrainHeuristicCapacity* pCapacity = object.GetCapacity< TerrainHeuristicCapacity >();
    if ( pCapacity )
    {
        sBestNodeForObstacle  costEvaluationFunctor( caller.GetOrderManager().GetFuseau(), *pCapacity, *pCenter, rRadius );
        TER_World::GetWorld().GetPathFindManager().ApplyOnNodesWithinCircle( *pCenter, rRadius, costEvaluationFunctor );        
        if( costEvaluationFunctor.FoundAPoint() )
            pResultPos.reset( new MT_Vector2D( costEvaluationFunctor.BestPosition() ) );
    }
    return pResultPos;
    return pResultPos;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInFuseau
// Created: AGN 03-03-11
//-----------------------------------------------------------------------------
template< typename T >
bool DEC_GeometryFunctions::IsPointInFuseau( const T& caller, MT_Vector2D* pVect )
{
    assert( pVect != 0 );
    return caller.GetOrderManager().GetFuseau().IsInside( *pVect );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePointBeforeLima
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputePointBeforeLima( const T& caller, int phaseLine, float distanceBefore )
{
    boost::shared_ptr< MT_Vector2D > pResult;
    if( const MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( phaseLine ) )
    {
        MT_Vector2D vResult;
        if( caller.GetOrderManager().GetFuseau().ComputePointBeforeLima( *pLima, MIL_Tools::ConvertMeterToSim( distanceBefore ), vResult ) )
            pResult.reset( new MT_Vector2D( vResult ) );
    }
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau
// Created: SBO 2008-01-11
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau( const T& caller, uint limaID, MT_Float rDistBeforeLima, const MIL_Fuseau* pFuseau )
{
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( limaID );

    boost::shared_ptr< MT_Vector2D > pResult;
    if(  pLima && pFuseau )
    {
        MT_Vector2D vResult;
        if ( pFuseau->ComputePointBeforeLima( *pLima, rDistBeforeLima, vResult ) )
            pResult.reset( new MT_Vector2D( vResult ) );
    }
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau( const T& caller, const TER_Localisation* pLocation )
{
    assert( pLocation );
    boost::shared_ptr< MT_Vector2D > pResult;
    TER_Localisation clipped;
    if( ClipLocalisationInFuseau( *pLocation, caller.GetOrderManager().GetFuseau(), clipped ) )
    {
        MT_Vector2D vResult;
        if( clipped.ComputeNearestPoint( GetPosition( caller ), vResult ) )
            pResult.reset( new MT_Vector2D( vResult ) );
    }
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau
// Created: GGR 2006-09-27
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau( const T& caller, const TER_Localisation* pLocation )
{
    assert( pLocation );
    boost::shared_ptr< MT_Vector2D > pResult;

    TER_Localisation fuseauLocation = TER_Localisation( TER_Localisation::ePolygon, caller.GetOrderManager().GetFuseau().GetBorderPoints() );

    MT_Vector2D vResult;
    if ( fuseauLocation.ComputeNearestPoint( pLocation->ComputeBarycenter(), vResult ) )
        pResult.reset( new MT_Vector2D( vResult ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
template< typename T > 
float DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives( const T& caller, const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const std::vector< DEC_Objective* >& objectives )
{
    //
    typedef std::vector< DEC_Objective* >     T_ObjectiveVector;
    typedef T_ObjectiveVector::iterator       IT_ObjectiveVector;
    typedef T_ObjectiveVector::const_iterator CIT_ObjectiveVector;
    //
    const MIL_LimaOrder* pNextLima = caller.GetOrderManager().FindNextScheduledLima();

    const DEC_Objective* pNextObjective = 0;
    for( std::vector< DEC_Objective* >::const_iterator it = objectives.begin(); it != objectives.end(); ++it )
    {
        const DEC_Objective* pObjective = *it;
        if( pObjective->GetSchedule() == 0 || pObjective->IsFlagged() )
            continue;

        if( ( !pNextObjective || pObjective->GetSchedule() < pNextObjective->GetSchedule() )
         && ( !pNextLima      || pObjective->GetSchedule() < pNextLima->GetSchedule() ) )
            pNextObjective = *it;
    }

    MT_Float rDistanceFromScheduled = std::numeric_limits< MT_Float >::max();
    uint     nSchedule              = 0;
    if( pNextObjective )
    {
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromObjective( *pNextObjective, _ComputeAutomatesBarycenter( automates ) );
        nSchedule = pNextObjective->GetSchedule();
    }
    else if( pNextLima )
    {
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromLima( *pNextLima, _ComputeAutomatesBarycenter( automates ) );
        nSchedule = pNextLima->GetSchedule();
    }
    
    return ComputeDelayFromSchedule( pFuseau, automates, ( float ) rDistanceFromScheduled, nSchedule );
}
