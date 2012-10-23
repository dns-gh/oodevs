// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_FIXTURE_H
#define MODULE_FIXTURE_H

#include "module_tester/ApiFixture.h"

// Movement module dependencies
#include "simulation_kernel/Decision/DEC_Path_ABC.h" // $$$$ MCO : for enums
#include "MT_Tools/MT_Vector2D.h"
#include <geometry/Types.h>
#include <spatialcontainer/TerrainData.h>

class KnowledgeCache;
class DEC_Knowledge_Object;
class MT_Line;
class MIL_LimaOrder;
class TerrainPathfinder;

typedef bool (*PathfindShouldEndComputation)( float rCostToCurrentNode, float rCostToGoalNode, void* userData );
typedef float (*PathfindEvaluateCost)( const geometry::Point2f& from, const geometry::Point2f& to, void* userData );
typedef float (*PathfindGetCost)( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween, void* userData );
typedef void (*PathfindHandlePathPoint)( const geometry::Point2f& point, const TerrainData& dataAtPoint, const TerrainData& dataToNextPoint, void* userData );

inline std::ostream& operator<<( std::ostream& os, const TerrainData& data )
{
    return os << data.ToString();
}

inline std::ostream& operator<<( std::ostream& s, const MT_Vector2D& v )
{
    return s << '[' << v.rX_ << ", " << v.rY_ << ']';
}

namespace sword
{
namespace movement
{
    class Path_ABC;
    class PathPoint;
}
}
// end of dependencies

#define REGISTERED_HOOKS( APPLY ) \
    APPLY( CleanPathAfterComputation, 1, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( ComputeAgentFutureObjectCollision, 4, bool, ( const SWORD_Model* entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) ) \
    APPLY( CreatePath, 3, boost::shared_ptr< sword::movement::Path_ABC >, ( const SWORD_Model* model, const MT_Vector2D& vPosEnd, int pathType ) ) \
    APPLY( CreatePathList, 3, boost::shared_ptr< sword::movement::Path_ABC >, ( const SWORD_Model* model, std::vector< boost::shared_ptr< MT_Vector2D > >& points, int pathType ) ) \
    APPLY( ExecutePathfind, 2, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, TerrainPathfinder& pathfind ) ) \
    APPLY( GetAgentFuturePosition, 3, MT_Vector2D, ( const SWORD_Model* entity, double rTime, bool bBoundOnPath ) ) \
    APPLY( GetPathDestPoint, 1, const boost::shared_ptr< sword::movement::PathPoint >*, ( const boost::shared_ptr< sword::movement::PathPoint >& pPoint ) ) \
    APPLY( GetPathDIAType, 1, const char*, ( const boost::shared_ptr< sword::movement::PathPoint >& point ) ) \
    APPLY( GetPathLimaPoint, 1, unsigned int, ( const boost::shared_ptr< sword::movement::PathPoint >& pPoint ) ) \
    APPLY( AvoidEnemies, 1, bool, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( GetEnemyCostAtSecurityRange, 1, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( GetEnemyCostOnContact, 1, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( AvoidObjects, 1, bool, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( GetFirstPoint, 3, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, void(*callback)( const MT_Vector2D& point, void* userData ), void* userData ) ) \
    APPLY( GetObjectCost, 2, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, unsigned int type ) ) \
    APPLY( GetThreshold, 1, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( HandlePopulations, 1, bool, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( GetPopulationSecurityRange, 1, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( GetCostOutsideOfPopulation, 1, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( GetPopulationAttitudeCost, 2, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, unsigned int type ) ) \
    APPLY( GetPathPoints, 3, void, ( unsigned int entity, void(*callback)( const boost::shared_ptr< sword::movement::PathPoint >& point, void* userData ), void* userData ) ) \
    APPLY( GetPathPos, 1, const MT_Vector2D*, ( const boost::shared_ptr< sword::movement::PathPoint >& point ) ) \
    APPLY( GetPathTypeLimaPoint, 1, int, ( const boost::shared_ptr< sword::movement::PathPoint >& pPoint ) ) \
    APPLY( GetPathTypePoint, 1, int, ( const boost::shared_ptr< sword::movement::PathPoint >& pPoint ) ) \
    APPLY( InitializePathClass, 3, void, ( const char* xml, const unsigned int* first, size_t size ) ) \
    APPLY( IsAgentMovingOnPath, 2, bool, ( unsigned int entity, const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( PathGetLastPointOfPath, 1, boost::shared_ptr< MT_Vector2D >, ( const boost::shared_ptr< sword::movement::Path_ABC >& pPath ) ) \
    APPLY( PathGetLength, 1, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( PathGetState, 1, DEC_Path_ABC::E_State, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \

#define USED_HOOKS( APPLY ) \
    APPLY( GetDistanceAvantLima, 1, double, ( const SWORD_Model* entity ) ) \
    APPLY( GetDistanceAvantPoint, 2, double, ( const SWORD_Model* entity, const TerrainData& data ) ) \
    APPLY( GetLimaOrders, 3, void, ( const MIL_LimaOrder& lima, void(*visitor)( unsigned int limaID, int functionID, void* userData ), void* userData ) ) \
    APPLY( GetLimas, 3, void, ( const SWORD_Model* entity, void(*visitor)( const MIL_LimaOrder& lima, void* userData ), void* userData ) ) \
    APPLY( GetPathfindCurrentThread, 0, int, () ) \
    APPLY( IsDestinationTrafficable, 3, bool, ( const SWORD_Model* entity, const MT_Vector2D* first, size_t size ) ) \
    APPLY( IsNullAutomateFuseau, 1, bool, ( const SWORD_Model* entity ) ) \
    APPLY( LimaIntersect2D, 3, bool, ( const MIL_LimaOrder& lima, const MT_Line& line, MT_Vector2D& intersection ) ) \
    APPLY( UsePathDebug, 0, bool, () ) \
    APPLY( FindObjectType, 1, unsigned int, ( const char* type ) ) \
    APPLY( FindPopulationAttitude, 2, bool, ( const char* attitude, unsigned int& id ) ) \
    APPLY( GetAltitudeCost, 5, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to, double rAltitudeCostPerMeter ) ) \
    APPLY( GetEnemiesCost, 7, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rEnemyMaximumCost ) ) \
    APPLY( GetFuseauxCost, 11, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to, double rMaximumFuseauDistance, double rMaximumFuseauDistanceWithAutomata, double rFuseauCostPerMeterOut, double rComfortFuseauDistance, double rFuseauCostPerMeterIn, double rMaximumAutomataFuseauDistance, double rAutomataFuseauCostPerMeterOut ) ) \
    APPLY( GetObjectsCost, 6, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) ) \
    APPLY( GetPopulationsCost, 7, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rPopulationMaximumCost ) ) \
    APPLY( GetUrbanBlockCost, 4, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to ) ) \
    APPLY( IsValidPosition, 1, bool, ( const MT_Vector2D& point ) ) \
    APPLY( GetMaxPathFindComputationDuration, 0, unsigned int, () ) \
    APPLY( CancelPathFindJob, 1, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( ComputeObjectCollision, 8, void, ( const SWORD_Model* entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, MT_Vector2D* start, std::size_t size, bool blockedByObject, bool applyScale ) ) \
    APPLY( GetWorldWeldValue, 0, double, () ) \
    APPLY( ComputePathfind, 13, bool, ( TerrainPathfinder& pathfind, bool needRefine, bool strictClosest, const geometry::Point2f& from, const geometry::Point2f& to, PathfindEvaluateCost evaluate, void* evaluateData, PathfindGetCost get, void* getData, PathfindHandlePathPoint handler, void* handlerData, PathfindShouldEndComputation termination, void* terminationData ) ) \
    APPLY( CanObjectInteractWith, 2, bool, ( const SWORD_Model* entity, const SWORD_Model* object ) ) \
    APPLY( GetObjectListWithinCircle, 5, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* object, void* userData ), void* userData ) ) \
    APPLY( GetSpeedWithReinforcement, 2, double, ( const SWORD_Model* entity, const TerrainData& environment ) ) \
    APPLY( GetTheoricMaxSpeedWithReinforcement, 1, double, ( const SWORD_Model* entity ) ) \
    APPLY( GetSpeedWithReinforcementObject, 3, double, ( const SWORD_Model* entity, const TerrainData& environment, const SWORD_Model* object ) ) \
    APPLY( NotifyMovingInsideObject, 2, void, ( const SWORD_Model* entity, const SWORD_Model* object ) ) \
    APPLY( NotifyMovingOnPathPoint, 2, void, ( const SWORD_Model* entity, const MT_Vector2D& point ) ) \
    APPLY( NotifyMovingOutsideObject, 2, void, ( const SWORD_Model* entity, const SWORD_Model* object ) ) \
    APPLY( ObjectIntersect2D, 4, bool, ( const SWORD_Model* object, const MT_Line& line, void (*callback)( const MT_Vector2D& point, void* userData ), void* userData ) ) \
    APPLY( ObjectIsInside, 2, bool, ( const SWORD_Model* object, const MT_Vector2D& point ) ) \
    APPLY( ObjectIsOnBorder, 2, bool, ( const SWORD_Model* object, const MT_Vector2D& point ) ) \
    APPLY( EntityManagerFindObject, 1, bool, ( unsigned int nID ) ) \
    APPLY( GetKnowledgeObjectRealName, 1, const char*, ( boost::shared_ptr< DEC_Knowledge_Object > object ) ) \
    APPLY( GetObjectKnownId, 1, int, ( boost::shared_ptr< DEC_Knowledge_Object > obstacle ) ) \
    APPLY( StartComputePathfind, 2, void, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path ) ) \
    APPLY( UpdateObjectsToAvoid, 2, bool, ( boost::shared_ptr< KnowledgeCache >& cache, const SWORD_Model* entity ) ) \
    APPLY( ConvertSpeedMosToSim, 1, double, ( double speed ) ) \
    APPLY( GetLandTypeName, 1, const char*, ( const TerrainData& terrain ) ) \
    APPLY( VisitKnowledgeObjects, 3, void, ( const SWORD_Model* entity, bool(*visitor)( const DEC_Knowledge_Object* object, void* userData ), void* userData ) ) \
    APPLY( CanMoveOn, 2, bool, ( const SWORD_Model* entity, const MT_Vector2D& point ) )

#define REGISTERED_AND_USED_HOOKS( APPLY ) \
    APPLY( AddPathPoint, 2, void, ( unsigned int entity, const boost::shared_ptr< sword::movement::PathPoint >& point ) ) \
    APPLY( RemovePathPoint, 2, void, ( unsigned int entity, const boost::shared_ptr< sword::movement::PathPoint >& point ) ) \
    APPLY( ComputeHeight, 1, double, ( const SWORD_Model* entity ) )

HOOK_FIXTURE( HOOKS )

namespace sword
{
namespace movement
{
    struct ModuleFixture : core::HookFixture
    {
        ModuleFixture()
            : HookFixture( "movement_module", boost::assign::list_of( "move" )
                                                                    ( "magic move" )
                                                                    ( "orientate" )
                                                                    ( "compute height" ) )
        {}
    };
}
}

#endif // MODULE_FIXTURE_H
