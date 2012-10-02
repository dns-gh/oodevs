// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MovementHooks.h"
#include "Hook.h"
#include "PathAdapter.h"
#include "PathPointAdapter.h"
#include "KnowledgeCache.h"
#include "MIL_AgentServer.h"
#include "RoleAction_Moving.h"
#include "AlgorithmsFactories.h"
#include "LocationComputer_ABC.h"
#include "LocationComputerFactory_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Terrain/PHY_RoleInterface_TerrainAnalysis.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Tools/MIL_Tools.h"
#include "Urban/MIL_UrbanCache.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_PopulationConcentrationManager.h"
#include "simulation_terrain/TER_PopulationFlowManager.h"
#include "simulation_terrain/TER_World.h"
#include "tools/Loader_ABC.h"
#include <pathfind/TerrainPathfinder.h>
#undef ERROR
#include <pathfind/TerrainRule_ABC.h>
#include <core/Facade.h>
#include <core/UserData.h>
#include <core/Convert.h>
#include <module_api/Hook.h>
#include <module_api/Model.h>
#include <boost/foreach.hpp>

using namespace sword;

namespace sword
{
namespace movement
{
    class PathPoint;
}
}

#define GET_ROLE( node, role ) (*core::Convert( node ))[ "pion" ].GetUserData< MIL_AgentPion >().GetRole< role >()
#define GET_PION( node ) (*core::Convert( node ))[ "pion" ].GetUserData< MIL_AgentPion >()
#define GET_DATA( node, data ) (*core::Convert( node ))[ "data" ].GetUserData< data >()

namespace
{
    DEFINE_HOOK( StartComputePathfind, void, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
    {
        MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( PathAdapter::Add( *core::Convert( entity ), path ) );
    }
    DEFINE_HOOK( GetAgentListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )
    {
        const core::Model& rootNode = *core::Convert( root );
        TER_Agent_ABC::T_AgentPtrVector agents;
        TER_World::GetWorld().GetAgentManager().GetListWithinCircle( vCenter, rRadius, agents );
        BOOST_FOREACH( TER_Agent_ABC* agent, agents )
            callback( core::Convert( &rootNode[ "entities" ][ static_cast< const PHY_RoleInterface_Location* >( agent )->GetAgent().GetID() ] ), userData );
    }
    DEFINE_HOOK( GetAgentListWithinLocalisation, void, ( const SWORD_Model* root, const SWORD_Model* localization, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) ) \
    {
        const core::Model& rootNode = *core::Convert( root );
        TER_Agent_ABC::T_AgentPtrVector agentsDetected;
        TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( *core::Convert( localization )->GetUserData< const TER_Localisation* >(), agentsDetected );
        BOOST_FOREACH( TER_Agent_ABC* agent, agentsDetected )
            callback( core::Convert( &rootNode[ "entities" ][ static_cast< const PHY_RoleInterface_Location* >( agent )->GetAgent().GetID() ] ), userData );
    }
    DEFINE_HOOK( GetObjectListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* object, void* userData ), void* userData ) )
    {
        const core::Model& rootNode = *core::Convert( root );
        TER_Object_ABC::T_ObjectVector objects; // Récupération de la liste des objets dynamiques contenus dans le rayon vEnd - vStart
        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( vCenter, rRadius, objects );
        BOOST_FOREACH( TER_Object_ABC* object, objects )
            callback( core::Convert( &rootNode[ "objects" ][ static_cast< MIL_Object_ABC* >( object )->GetID() ] ), userData );
    }
    DEFINE_HOOK( GetConcentrationListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* concentration, void* userData ), void* userData ) )
    {
        const core::Model& rootNode = *core::Convert( root );
        TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations;
        TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinCircle( vCenter, rRadius, perceivableConcentrations );
        BOOST_FOREACH( const TER_PopulationConcentration_ABC* terConcentration, perceivableConcentrations )
        {
            const MIL_PopulationConcentration* concentration = static_cast< const MIL_PopulationConcentration* >( terConcentration );
            callback( core::Convert( &rootNode[ "populations" ][ concentration->GetPopulation().GetID() ][ "elements" ][ concentration->GetID() ] ), userData );
        }
    }
    DEFINE_HOOK( GetFlowListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* flow, void* userData ), void* userData ) )
    {
        const core::Model& rootNode = *core::Convert( root );
        TER_PopulationFlow_ABC::T_ConstPopulationFlowVector perceivableFlows;
        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinCircle( vCenter, rRadius, perceivableFlows );
        BOOST_FOREACH( const TER_PopulationFlow_ABC* terFlow, perceivableFlows )
        {
            const MIL_PopulationFlow* flow = static_cast< const MIL_PopulationFlow* >( terFlow );
            callback( core::Convert( &rootNode[ "populations" ][ flow->GetPopulation().GetID() ][ "elements" ][ flow->GetID() ] ), userData );
        }
    }
    DEFINE_HOOK( GetUrbanObjectListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& center, float radius, void (*callback)( const SWORD_Model* urbanObjectWrapper, void* userData ), void* userData ) )
    {
        const core::Model& rootNode = *core::Convert( root );
        std::vector< const MIL_UrbanObject_ABC* > perceivableUrbanBlock;
        MIL_AgentServer::GetWorkspace().GetUrbanCache().GetListWithinCircle( center, radius, perceivableUrbanBlock );
        BOOST_FOREACH( const MIL_UrbanObject_ABC* urbanBlock, perceivableUrbanBlock )
            callback( core::Convert( &rootNode[ "urban-objects" ][ urbanBlock->GetID() ] ), userData );
    }
    DEFINE_HOOK( GetUrbanBlocksListWithinSegment, void, ( const SWORD_Model* root, MT_Vector2D first, MT_Vector2D second, void (*callback)( const SWORD_Model* urbanObjectWrapper, void* userData ), void* userData ) )
    {
        const core::Model& rootNode = *core::Convert( root );
        std::vector< const MIL_UrbanObject_ABC* > list;
        MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocksWithinSegment( first, second, list );
        BOOST_FOREACH( const MIL_UrbanObject_ABC* urbanBlock, list )
            callback( core::Convert( &rootNode[ "urban-objects" ][ urbanBlock->GetID() ] ), userData );
    }
    DEFINE_HOOK( EntityManagerFindObject, bool, ( unsigned int nID ) )
    {
        return MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( nID ) != 0;
    }
    DEFINE_HOOK( GetWorldWeldValue, double, () )
    {
        return TER_World::GetWorld().GetWeldValue();
    }
    DEFINE_HOOK( IsDestinationTrafficable, bool, ( const SWORD_Model* entity, const MT_Vector2D* first, size_t size ) )
    {
        const std::vector< MT_Vector2D > points( first, first + size );
        const PHY_RoleInterface_TerrainAnalysis& analysis = GET_ROLE( entity, PHY_RoleInterface_TerrainAnalysis );
        return analysis.CanMoveOnUrbanBlock( points ) &&
               analysis.CanMoveOnBurningCells( points ) &&
               analysis.CanMoveOnKnowledgeObject( points );
    }
    DEFINE_HOOK( GetDistanceAvantLima, double, ( const SWORD_Model* entity ) )
    {
        return GET_PION( entity ).GetType().GetDistanceAvantLima();
    }
    DEFINE_HOOK( GetDistanceAvantPoint, double, ( const SWORD_Model* entity, const TerrainData& data ) )
    {
        return GET_PION( entity ).GetType().GetDistanceAvantPoint( data );
    }
    DEFINE_HOOK( LimaIntersect2D, bool, ( const MIL_LimaOrder& lima, const MT_Line& line, MT_Vector2D& intersection ) )
    {
        return lima.Intersect2D( line, intersection );
    }
    DEFINE_HOOK( GetLimaOrders, void, ( const MIL_LimaOrder& lima, void(*visitor)( unsigned int limaID, int functionID, void* userData ), void* userData ) )
    {
        for( MIL_LimaOrder::CIT_LimaFunctions it = lima.GetFunctions().begin(); it != lima.GetFunctions().end(); ++it )
            visitor( lima.GetID(), (*it)->GetID(), userData );
    }
    DEFINE_HOOK( GetLimas, void, ( const SWORD_Model* entity, void(*visitor)( const MIL_LimaOrder& lima, void* userData ), void* userData ) )
    {
        const T_LimaVector& limas = GET_PION( entity ).GetOrderManager().GetLimas();
        for( CIT_LimaVector it = limas.begin(); it != limas.end(); ++it )
            visitor( *it, userData );
    }
    DEFINE_HOOK( UsePathDebug, bool, () )
    {
        return MIL_AgentServer::IsInitialized() &&  MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug();
    }
    DEFINE_HOOK( GetPathfindCurrentThread, int, () )
    {
        return MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread();
    }
    DEFINE_HOOK( FindObjectType, unsigned int, ( const char* type ) )
    {
        return  MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( type ).GetID();
    }
    DEFINE_HOOK( IsNullAutomateFuseau, bool, ( const SWORD_Model* entity ) )
    {
        return GET_PION( entity ).GetAutomate().GetOrderManager().GetFuseau().IsNull();
    }
    DEFINE_HOOK( IsValidPosition, bool, ( const MT_Vector2D& point ) )
    {
        return TER_World::GetWorld().IsValidPosition( point );
    }
    DEFINE_HOOK( GetMaxPathFindComputationDuration, unsigned int, () )
    {
        return MIL_AgentServer::GetWorkspace().GetPathFindManager().GetMaxComputationDuration();
    }
    DEFINE_HOOK( CancelPathFindJob, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
    {
        boost::shared_ptr< PathAdapter > removed = PathAdapter::Remove( path );
        if( removed )
            MIL_AgentServer::GetWorkspace().GetPathFindManager().CancelJob( removed.get() );
        else
            MT_LOG_ERROR_MSG( "Path already canceled" );
    }
    DEFINE_HOOK( ComputeObjectCollision, void,
        ( const SWORD_Model* entity, const KnowledgeCache& objectsToTest, double& rDistance,
          boost::shared_ptr< DEC_Knowledge_Object >& pObject, MT_Vector2D* start, std::size_t size, bool blockedByObject, bool applyScale ) )
    {
        const MIL_AgentPion& agent = (*core::Convert( entity ))[ "pion" ].GetUserData< MIL_AgentPion >();
        static const double epsilon = 1e-8;
        // Path hull
        TER_Polygon pathHull;
        T_PointVector hullPoints;
        hullPoints.reserve( 1 + size );
        for( MT_Vector2D* itPathPoint = start; itPathPoint != start + size; ++itPathPoint )
            hullPoints.push_back( *itPathPoint );
        if( size != 0 )
            hullPoints.push_back( *start );
        pathHull.Reset( hullPoints, true );
        MT_Rect bbox = pathHull.GetBoundingBox();
        // Optimisation: Check bounding box instead of doing 345 intersection checks in case we have 345 points in the path (which happens)
        // Check intersection with convex hull of path if there are less segments.
        // Worst case: Bounding hull has 1 more segment than the actual path, or Bounding hull has 2 segments (degenerate).
        // Best case: Bounding hull has 3 segments.
        std::size_t hullSize = pathHull.GetBorderPoints().size();
        bool hullIntersectionIsFaster = hullSize > 2 && hullSize < hullPoints.size();
        // Determination de tous les objets connus avec lesquels il va y avoir collision dans le déplacement en cours
        std::auto_ptr< const TER_Localisation > pScaledObjectLocation;
        for( CIT_KnowledgeObjectVector itKnowledge = objectsToTest.objectsToAvoid_.begin(); itKnowledge != objectsToTest.objectsToAvoid_.end(); ++itKnowledge )
        {
            boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = *itKnowledge;
            const TER_Localisation* pObjectLocation = 0;
            if( applyScale )
            {
                pObjectLocation = new TER_Localisation( pKnowledge->GetLocalisation() );
                if( start && blockedByObject ) // $$$$ MCO 2012-06-07: point list can actually never be empty but well...
                {
                    T_PointVector firstPointVector;
                    firstPointVector.push_back( *start );
                    if( pKnowledge->IsObjectInsidePathPoint( firstPointVector, agent ) )
                        continue;
                }
                const_cast< TER_Localisation* >( pObjectLocation )->Scale( 10 ); // $$$ CMA arbitrary 10m precision (useful for recomputing path when it is very close to obstacle)
                pScaledObjectLocation.reset( pObjectLocation );
            }
            else
                pObjectLocation = &pKnowledge->GetLocalisation();

            MT_Rect objectBBox = pObjectLocation->GetBoundingBox();
            if( !bbox.Intersect2D( objectBBox ) && !bbox.Contains( objectBBox ) && !objectBBox.Contains( bbox ) )
                continue;
            if( hullIntersectionIsFaster )
            {
                const T_PointVector& borderPoints = pathHull.GetBorderPoints();
                if( borderPoints.empty() )
                    continue;
                T_PointVector::const_iterator itPathHullPoint = borderPoints.begin();
                const MT_Vector2D* pPrevPathHullPos = &(*itPathHullPoint);
                ++itPathHullPoint;
                bool hullIntersected = false;
                for( ; !hullIntersected && itPathHullPoint != borderPoints.end(); ++itPathHullPoint )
                {
                    MT_Line lineTmp( *pPrevPathHullPos, *itPathHullPoint );
                    TER_DistanceLess colCmp( *pPrevPathHullPos );
                    T_PointSet collisions( colCmp );

                    hullIntersected |= pObjectLocation->Intersect2D( lineTmp, collisions, epsilon );
                    pPrevPathHullPos = &(*itPathHullPoint);
                }
                if( !hullIntersected )
                {
                    TER_Localisation localisationHull( pathHull );
                    if( !pObjectLocation->Contains( localisationHull, epsilon ) && !localisationHull.Contains( *pObjectLocation, epsilon ) )
                        continue;
                }
            }
            double rDistanceSum = 0.;
            const MT_Vector2D* pPrevPos = start;
            for( MT_Vector2D* itPathPoint = ( start + 1 ); itPathPoint != start + size; ++itPathPoint )
            {
                MT_Line lineTmp( *pPrevPos, *itPathPoint );
                TER_DistanceLess colCmp( *pPrevPos );
                T_PointSet collisions( colCmp );
                if( pObjectLocation->Intersect2D( lineTmp, collisions, epsilon ) )
                {
                    if( collisions.empty() ) // should never happen
                        continue;
                    rDistanceSum += pPrevPos->Distance( *collisions.begin() );
                    if( !pObject || rDistanceSum < rDistance )
                    {
                        rDistance = rDistanceSum;
                        pObject = pKnowledge;
                    }
                    break;
                }
                else
                    rDistanceSum += pPrevPos->Distance( *itPathPoint );
                pPrevPos = itPathPoint;
            }
        }
    }
    typedef bool (*PathfindShouldEndComputation)( float rCostToCurrentNode, float rCostToGoalNode, void* userData );
    typedef float (*PathfindEvaluateCost)( const geometry::Point2f& from, const geometry::Point2f& to, void* userData );
    typedef float (*PathfindGetCost)( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween, void* userData );
    typedef void (*PathfindHandlePathPoint)( const geometry::Point2f& point, const TerrainData& dataAtPoint, const TerrainData& dataToNextPoint, void* userData );
    DEFINE_HOOK( ComputePathfind, bool, ( TerrainPathfinder& pathfind, bool needRefine, bool strictClosest,
                                          const geometry::Point2f& from, const geometry::Point2f& to,
                                          PathfindEvaluateCost evaluate, void* evaluateData,
                                          PathfindGetCost get, void* getData,
                                          PathfindHandlePathPoint handler, void* handlerData,
                                          PathfindShouldEndComputation termination, void* terminationData ) ) // $$$$ MCO : or just have hooks on the other side as callbacks
    {
        if( needRefine )
            pathfind.SetConfiguration( 1, 3 ); // $$$$ AGE 2005-03-30: whatever
        pathfind.SetChoiceRatio( strictClosest ? 0.f : 0.1f );
        struct PathfindCallbackAdapter : pathfind::AStarManagementCallback_ABC
        {
            PathfindCallbackAdapter( PathfindShouldEndComputation callback, void* userData )
                : callback_( callback )
                , userData_( userData )
            {}
            virtual bool ShouldEndComputation( float rCostToCurrentNode, float rCostToGoalNode )
            {
                return callback_( rCostToCurrentNode, rCostToGoalNode, userData_ );
            }
            PathfindShouldEndComputation callback_;
            void* userData_;
        } callback( termination, terminationData );
        pathfind.SetCallback( &callback );
        struct TerrainRuleAdapter : TerrainRule_ABC
        {
            TerrainRuleAdapter( PathfindEvaluateCost evaluate, void* evaluateData,
                                PathfindGetCost get, void* getData )
                : evaluate_    ( evaluate )
                , evaluateData_( evaluateData )
                , get_         ( get )
                , getData_     ( getData )
            {}
            virtual float EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to )
            {
                return evaluate_( from, to, evaluateData_ );
            }
            virtual float GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween )
            {
                return get_( from, to, terrainTo, terrainBetween, getData_ );
            }
            PathfindEvaluateCost evaluate_;
            void* evaluateData_;
            PathfindGetCost get_;
            void* getData_;
        } rule( evaluate, evaluateData, get, getData );
        struct ThreadHandlerAdapter : tools::thread::Handler_ABC< TerrainPathPoint >
        {
            ThreadHandlerAdapter( PathfindHandlePathPoint handler, void* handlerData )
                : handler_    ( handler )
                , handlerData_( handlerData )
            {}
            virtual void Handle( const TerrainPathPoint& point )
            {
                handler_( point, point.DataAtPoint(), point.DataToNextPoint(), handlerData_ );
            }
            PathfindHandlePathPoint handler_;
            void* handlerData_;
        } h( handler, handlerData );
        const bool bResult = pathfind.ComputePath( from, to, rule, h );
        pathfind.SetConfiguration( 0, 0 );
        pathfind.SetCallback( 0 );
        return bResult;
    }
    DEFINE_HOOK( GetSpeedWithReinforcement, double, ( const SWORD_Model* entity, const TerrainData& environment ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).GetSpeedWithReinforcement( environment );
    }
    DEFINE_HOOK( GetMaxSpeedWithReinforcement, double, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).GetMaxSpeedWithReinforcement();
    }
    DEFINE_HOOK( NotifyMovingOnPathPoint, void, ( const SWORD_Model* entity, const MT_Vector2D& point ) )
    {
        GET_ROLE( entity, RoleAction_Moving ).NotifyMovingOnPathPoint( point );
    }
    DEFINE_HOOK( CanMoveOn, bool, ( const SWORD_Model* entity, const MT_Vector2D& point ) )
    {
        return GET_ROLE( entity, PHY_RoleInterface_TerrainAnalysis ).CanMoveOn( point );
    }
    DEFINE_HOOK( ObjectIntersect2D, bool, ( const SWORD_Model* object, const MT_Line& line, void (*callback)( const MT_Vector2D& point, void* userData ), void* userData ) )
    {
        MT_Vector2D pt;
        TER_DistanceLess cmp( pt );
        T_PointSet collisions( cmp );
        bool result = GET_DATA( object, MIL_Object_ABC ).Intersect2D( line, collisions );
        std::for_each( collisions.begin(), collisions.end(), boost::bind( callback, _1, userData ) );
        return result;
    }
    DEFINE_HOOK( ObjectIsInside, bool, ( const SWORD_Model* object, const MT_Vector2D& point ) )
    {
        return GET_DATA( object, MIL_Object_ABC ).IsInside( point );
    }
    DEFINE_HOOK( ObjectIsOnBorder, bool, ( const SWORD_Model* object, const MT_Vector2D& point ) )
    {
        return GET_DATA( object, MIL_Object_ABC ).IsOnBorder( point );
    }
    DEFINE_HOOK( GetSpeedWithReinforcementObject, double, ( const SWORD_Model* entity, const TerrainData& environment, const SWORD_Model* object ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).GetSpeedWithReinforcement( environment, GET_DATA( object, MIL_Object_ABC ) );
    }
    DEFINE_HOOK( CanObjectInteractWith, bool, ( const SWORD_Model* entity, const SWORD_Model* object ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).CanObjectInteractWith( GET_DATA( object, MIL_Object_ABC ) );
    }
    DEFINE_HOOK( NotifyMovingInsideObject, void, ( const SWORD_Model* entity, const SWORD_Model* object ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).NotifyMovingInsideObject( GET_DATA( object, MIL_Object_ABC ) );
    }
    DEFINE_HOOK( NotifyMovingOutsideObject, void, ( const SWORD_Model* entity, const SWORD_Model* object ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).NotifyMovingOutsideObject( GET_DATA( object, MIL_Object_ABC ) );
    }
    DEFINE_HOOK( GetObjectId, int, ( const SWORD_Model* object ) )
    {
        return GET_DATA( object, MIL_Object_ABC ).GetID();
    }
    DEFINE_HOOK( CanMove, bool, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).CanMove();
    }
    DEFINE_HOOK( HasResources, bool, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, RoleAction_Moving ).HasResources();
    }
    DEFINE_HOOK( GetObjectKnownId, int, ( boost::shared_ptr< DEC_Knowledge_Object > obstacle ) )
    {
        assert( obstacle && obstacle->IsValid() );
        return obstacle->GetObjectKnown() ? obstacle->GetObjectKnown()->GetID() : 0;
    }
    DEFINE_HOOK( UpdateObjectsToAvoid, bool, ( boost::shared_ptr< KnowledgeCache >& cache, const SWORD_Model* entity ) )
    {
        if( ! cache )
            cache.reset( new KnowledgeCache() );
        T_KnowledgeObjectVector knowledges;
        MIL_AgentPion& pion = (*core::Convert( entity ) )[ "pion" ].GetUserData< MIL_AgentPion >();
        pion.GetArmy().GetKnowledge().GetObjectsAtInteractionHeight( knowledges, pion, MIL_DangerousObjectFilter() );
        if( knowledges != cache->objectsToAvoid_ )
        {
            cache->objectsToAvoid_ = knowledges;
            cache->geometrySignatures_.clear();
            for( T_KnowledgeObjectVector::const_iterator it = knowledges.begin(); it != knowledges.end(); ++it )
                cache->geometrySignatures_.push_back( (*it)->GetLocalisation() );
            return true;
        }
        else
        {
            bool modified = false;
            for( std::size_t i = 0; i < knowledges.size(); ++i )
            {
                if( cache->geometrySignatures_[i] != knowledges[i]->GetLocalisation() )
                {
                    cache->geometrySignatures_[i] = knowledges[i]->GetLocalisation();
                    modified = true;
                }
            }
            if( modified )
            {
                cache->objectsToAvoid_ = knowledges;
                return true;
            }
        }
        return false;
    }

    DEFINE_HOOK( FindPopulationAttitude, bool, ( const char* attitude, unsigned int& id ) )
    {
        const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( attitude );
        if( ! pAttitude )
            return false;
        id = pAttitude->GetID();
        return true;
    }
    DEFINE_HOOK( ConvertSpeedMosToSim, double, ( double speed ) )
    {
        return MIL_Tools::ConvertSpeedMosToSim( speed );
    }
    DEFINE_HOOK( GetLandTypeName, const char*, ( const TerrainData& terrain ) )
    {
        return MIL_Tools::GetLandTypeName( terrain ).c_str();
    }
    DEFINE_HOOK( GetObjectRealName, const char*, ( const SWORD_Model* object ) )
    {
        return GET_DATA( object, MIL_Object_ABC ).GetType().GetRealName().c_str();
    }
    DEFINE_HOOK( GetKnowledgeObjectRealName, const char*, ( boost::shared_ptr< DEC_Knowledge_Object > object ) )
    {
        return object->GetType().GetRealName().c_str();
    }
    DEFINE_HOOK( ComputeHeight, double, ( const SWORD_Model* entity ) )
    {
        MIL_AgentPion& pion = GET_PION( entity );
        return pion.Execute( *pion.GetAlgorithms().locationComputerFactory_->Create() ).GetHeight();
    }
    DEFINE_HOOK( VisitKnowledgeObjects, void, ( const SWORD_Model* entity, bool(*visitor)( const DEC_Knowledge_Object* object, void* userData ), void* userData ) )
    {
        T_KnowledgeObjectVector knowledgesObject;
        GET_PION( entity ).GetArmy().GetKnowledge().GetObjects( knowledgesObject );
        for( CIT_KnowledgeObjectVector itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
        {
            const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
            if( visitor( &knowledge, userData ) )
                break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MovementHooks::Initialize
// Created: SLI 2012-03-13
// -----------------------------------------------------------------------------
void MovementHooks::Initialize( core::Facade& facade )
{
    REGISTER_HOOK( CanMove, facade );
    REGISTER_HOOK( CanObjectInteractWith, facade );
    REGISTER_HOOK( ComputeObjectCollision, facade );
    REGISTER_HOOK( ComputePathfind, facade );
    REGISTER_HOOK( ConvertSpeedMosToSim, facade );
    REGISTER_HOOK( EntityManagerFindObject, facade );
    REGISTER_HOOK( FindObjectType, facade );
    REGISTER_HOOK( FindPopulationAttitude, facade );
    REGISTER_HOOK( GetDistanceAvantLima, facade );
    REGISTER_HOOK( GetDistanceAvantPoint, facade );
    REGISTER_HOOK( GetKnowledgeObjectRealName, facade );
    REGISTER_HOOK( GetLandTypeName, facade );
    REGISTER_HOOK( GetLimaOrders, facade );
    REGISTER_HOOK( GetLimas, facade );
    REGISTER_HOOK( GetAgentListWithinCircle, facade );
    REGISTER_HOOK( GetAgentListWithinLocalisation, facade );
    REGISTER_HOOK( GetObjectListWithinCircle, facade );
    REGISTER_HOOK( GetConcentrationListWithinCircle, facade );
    REGISTER_HOOK( GetFlowListWithinCircle, facade );
    REGISTER_HOOK( GetUrbanObjectListWithinCircle, facade );
    REGISTER_HOOK( GetUrbanBlocksListWithinSegment, facade );
    REGISTER_HOOK( GetMaxPathFindComputationDuration, facade );
    REGISTER_HOOK( CancelPathFindJob, facade );
    REGISTER_HOOK( GetMaxSpeedWithReinforcement, facade );
    REGISTER_HOOK( GetObjectId, facade );
    REGISTER_HOOK( GetObjectKnownId, facade );
    REGISTER_HOOK( GetObjectRealName, facade );
    REGISTER_HOOK( GetPathfindCurrentThread, facade );
    REGISTER_HOOK( GetSpeedWithReinforcement, facade );
    REGISTER_HOOK( GetSpeedWithReinforcementObject, facade );
    REGISTER_HOOK( GetWorldWeldValue, facade );
    REGISTER_HOOK( HasResources, facade );
    REGISTER_HOOK( IsDestinationTrafficable, facade );
    REGISTER_HOOK( IsNullAutomateFuseau, facade );
    REGISTER_HOOK( IsValidPosition, facade );
    REGISTER_HOOK( LimaIntersect2D, facade );
    REGISTER_HOOK( CanMoveOn, facade );
    REGISTER_HOOK( NotifyMovingInsideObject, facade );
    REGISTER_HOOK( NotifyMovingOnPathPoint, facade );
    REGISTER_HOOK( NotifyMovingOutsideObject, facade );
    REGISTER_HOOK( ObjectIntersect2D, facade );
    REGISTER_HOOK( ObjectIsInside, facade );
    REGISTER_HOOK( ObjectIsOnBorder, facade );
    REGISTER_HOOK( StartComputePathfind, facade );
    REGISTER_HOOK( UpdateObjectsToAvoid, facade );
    REGISTER_HOOK( UsePathDebug, facade );
    REGISTER_HOOK( ComputeHeight, facade );
    REGISTER_HOOK( VisitKnowledgeObjects, facade );
    PathAdapter::Initialize( facade );
    PathPointAdapter::Initialize( facade );
}
