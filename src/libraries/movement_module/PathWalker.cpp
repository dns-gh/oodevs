// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "PathWalker.h"
#include "PathPoint.h"
#include "ModuleFacade.h"
#include "Agent_PathClass.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"
#include "wrapper/Hook.h"
#include "MT_Tools/MT_Line.h"
#include "simulation_kernel/Entities/Orders/MIL_DecisionalReport.h" // $$$$ MCO : for enums
#include <module_api/Log.h>
#include <boost/foreach.hpp>

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( CanObjectInteractWith, bool, ( const SWORD_Model* entity, const SWORD_Model* object ) )
DECLARE_HOOK( GetObjectListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* object, void* userData ), void* userData ) )
DECLARE_HOOK( GetSpeed, double, ( const SWORD_Model* entity, const TerrainData& environment ) )
DECLARE_HOOK( GetSpeedObject, double, ( const SWORD_Model* entity, const TerrainData& environment, const SWORD_Model* object ) )
DECLARE_HOOK( GetWorldWeldValue, double, () ) // $$$$ MCO : "static data" which amazingly is hard-coded to 10 in simulation_terrain
DECLARE_HOOK( NotifyMovingInsideObject, void, ( const SWORD_Model* entity, const SWORD_Model* object ) )
DECLARE_HOOK( NotifyMovingOnPathPoint, void, ( const SWORD_Model* entity, const MT_Vector2D& point ) )
DECLARE_HOOK( NotifyMovingOutsideObject, void, ( const SWORD_Model* entity, const SWORD_Model* object ) )
DECLARE_HOOK( ObjectIntersect2D, bool, ( const SWORD_Model* object, const MT_Line& line, void (*callback)( const MT_Vector2D& point, void* userData ), void* userData ) )
DECLARE_HOOK( ObjectIsInside, bool, ( const SWORD_Model* object, const MT_Vector2D& point ) )
DECLARE_HOOK( ObjectIsOnBorder, bool, ( const SWORD_Model* object, const MT_Vector2D& point ) )
DECLARE_HOOK( HasKnowledgeObject, bool, ( const SWORD_Model* entity, const SWORD_Model* object ) )

namespace
{
    void PostReport( const wrapper::View& entity, const MIL_DecisionalReport& code )
    {
        wrapper::Event event( "movement report" );
        event[ "entity/data" ] = entity[ "data" ];
        event[ "code" ] = code;
        event.Post();
    }

    void PostReport( const wrapper::View& entity, const MIL_DecisionalReport& code, const std::string& name )
    {
        wrapper::Event event( "movement report with name" );
        event[ "entity/data" ] = entity[ "data" ];
        event[ "code" ] = code;
        event[ "name" ] = name;
        event.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: PathWalker constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PathWalker::PathWalker( ModuleFacade& module, unsigned int entity )
    : module_         ( module )
    , entity_         ( entity )
    , vNewPos_        ( 0, 0 )
    , vNewDir_        ( 0, 0 )
    , speed_          ( 0 )
    , distance_       ( 0 )
    , pointsPassed_   ( 0 )
    , bForcePathCheck_( true )
    , bHasMoved_      ( false )
    , bFuelReportSent_( false )
    , pathSet_        ( eFinished )
{
    module.RegisterPathWalker( entity_, *this );
}

// -----------------------------------------------------------------------------
// Name: PathWalker destructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PathWalker::~PathWalker()
{
    module_.UnregisterPathWalker( entity_ );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::ComputeFutureObjectCollision
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool PathWalker::ComputeFutureObjectCollision( const wrapper::View& model, const wrapper::View& entity, const KnowledgeCache* objectsToTest,
                                               double& rDistance, wrapper::View* knowledgeObjectColliding, bool blockedByObject, bool applyScale ) const
{
    auto path = path_.lock();
    if( !path )
        return false;
    return path->ComputeFutureObjectCollision( model, entity, objectsToTest, rDistance, knowledgeObjectColliding, blockedByObject, applyScale );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::IsMovingOn
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
bool PathWalker::IsMovingOn( boost::shared_ptr< Path_ABC > path ) const
{
    return path == path_.lock();
}

//-----------------------------------------------------------------------------
// Name: PathWalker::ExtrapolatePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D PathWalker::ExtrapolatePosition( const wrapper::View& entity, const double rTime, const bool bBoundOnPath ) const
{
    const MT_Vector2D position( entity[ "movement/position/x" ], entity[ "movement/position/y" ] );
    auto path = path_.lock();
    if( !path)
        return position;
    try
    {
        return path->GetFuturePosition( position, rTime * entity[ "movement/speed" ], bBoundOnPath );
    }
    catch( const std::exception& e )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, tools::GetExceptionMsg( e ).c_str() );
        return position;
    }
}

// -----------------------------------------------------------------------------
// Name: PathWalker::ComputeCurrentSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PathWalker::ComputeCurrentSpeed( const wrapper::View& entity ) const
{
    const PathPoint& curPathPoint = **itCurrentPathPoint_;
    if( curPathPoint.GetType() == PathPoint::eTypePointPath )
        SetEnvironmentType( curPathPoint.GetObjectTypesToNextPoint(), entity );
    speed_ = GET_HOOK( GetSpeed )( entity, environment_ );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::InitializeEnvironment
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PathWalker::InitializeEnvironment( boost::shared_ptr< PathResult > path, const wrapper::View& entity ) const
{
    PathResult::CIT_PathPointList itPathPointTmp = itCurrentPathPoint_;
    while( itPathPointTmp != path->GetResult().end() && ( *itPathPointTmp )->GetType() != PathPoint::eTypePointPath )
        ++itPathPointTmp;
    if( itPathPointTmp == path->GetResult().end() )
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Path point is invalid" );
    SetEnvironmentType( ( *itPathPointTmp )->GetObjectTypesToNextPoint(), entity );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::SetEnvironmentType
// Created: SLI 2012-01-20
// -----------------------------------------------------------------------------
void PathWalker::SetEnvironmentType( const TerrainData& environment, const wrapper::View& entity ) const
{
    if( !( environment_ == environment ) ) //$$$
    {
        environment_ = environment;
        PostEnvironment( entity, environment_ );
    }
}

namespace
{
    void AddObject( const SWORD_Model* object, void* userData )
    {
        static_cast< std::vector< wrapper::View >* >( userData )->push_back( object );
    }
}

// -----------------------------------------------------------------------------
// Name: PathWalker::SetCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PathWalker::E_ReturnCode PathWalker::SetCurrentPath( boost::shared_ptr< PathResult > path, const wrapper::View& model, const wrapper::View& entity ) const
{
    bool pathChanged = path != path_.lock();
    if( !pathChanged && !bForcePathCheck_ )
        return eRunning;

    pointsPassed_ = 0;
    PathWalker::E_ReturnCode rc = eRunning;
    path_ = path;
    boost::shared_ptr< PathResult > currentPath = path_.lock();
    currentPath->InsertDecPoints(); // $$$ HIDEUX
    PostPath( entity );
    bForcePathCheck_ = false;
    if( currentPath->GetResult().empty() )
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "List of path_ points resulting from pathfind is empty" );
    itCurrentPathPoint_ = currentPath->GetCurrentKeyOnPath();
    if( itCurrentPathPoint_ == currentPath->GetResult().end() )
        return eItineraireMustBeJoined;
    if( *itCurrentPathPoint_ )
        currentPath->NotifyPointReached( itCurrentPathPoint_ );
    if( ( currentPath->GetState() == PathResult::ePartial ) && pathChanged )
    {
        const MT_Vector2D& lastWaypoint = currentPath->GetLastWaypoint();
        typedef std::vector< wrapper::View > T_Objects;
        T_Objects objects;
        std::string objectName;
        double cost = 0;
        GET_HOOK( GetObjectListWithinCircle )( model, lastWaypoint, 100, &AddObject, &objects );
        for( T_Objects::const_iterator it = objects.begin(); it != objects.end(); ++it )
        {
            if( GET_HOOK( CanObjectInteractWith )( entity, *it ) && GET_HOOK( HasKnowledgeObject )( entity, *it ) )
            {
                const double objectCost = path->GetClass().GetObjectCost( (*it)[ "type/identifier" ] );
                if( objectCost > cost )
                {
                    cost = objectCost;
                    objectName = (*it)[ "type/real-name" ];
                }
            }
        }
        if( objectName != "" )
            PostReport( entity, report::eRC_DifficultMovementProgression, objectName );
        else
            PostReport( entity, report::eRC_TerrainDifficile );
        rc = ePartialPath;
    }
    itNextPathPoint_ = itCurrentPathPoint_;
    ++itNextPathPoint_;
    InitializeEnvironment( currentPath, entity );
    return rc;
}

//-----------------------------------------------------------------------------
// Name: PathWalker::GoToNextNavPoint
// Created: JVT 02-12-06
// Last modified: JVT 03-02-04
//-----------------------------------------------------------------------------
bool PathWalker::GoToNextNavPoint( boost::shared_ptr< PathResult > path, const wrapper::View& entity ) const
{
    if( ( *itNextPathPoint_ )->GetType() == PathPoint::eTypePointPath )
    {
        GET_HOOK( NotifyMovingOnPathPoint )( entity, (*itNextPathPoint_)->GetPos() ); // $$$$ MCO : only for populations...
        SetCurrentPathPoint( path );
        ++itNextPathPoint_;
        CheckPathNotification( entity );
        return false;
    }
    // points particuliers -> EVT vers DEC
    do
    {
        module_.AddPathPoint( entity_, *itNextPathPoint_ );
        SetCurrentPathPoint( path );
        CheckPathNotification( entity );
    }
    while( ++itNextPathPoint_ != path->GetResult().end() &&
         ( *itNextPathPoint_ )->GetType() != PathPoint::eTypePointPath && ( *itNextPathPoint_ )->GetPos() == vNewPos_ );
    return true;
}

namespace
{
    const std::size_t pathSizeThreshold = 200;
}

// -----------------------------------------------------------------------------
// Name: PathWalker::CheckPathNotification
// Created: LDC 2012-10-22
// -----------------------------------------------------------------------------
void PathWalker::CheckPathNotification( const wrapper::View& entity ) const
{
    ++pointsPassed_;
    if( pointsPassed_ % pathSizeThreshold == 0 )
        PostPath( entity );
}

//-----------------------------------------------------------------------------
// Name: PathWalker::SetCurrentPathPoint
// Created: LDC 2012-01-18
//-----------------------------------------------------------------------------
void PathWalker::SetCurrentPathPoint( boost::shared_ptr< PathResult > path ) const
{
    itCurrentPathPoint_ = itNextPathPoint_;
    if( itCurrentPathPoint_ != path->GetResult().end() && *itCurrentPathPoint_ )
        path->NotifyPointReached( itCurrentPathPoint_ );
}

namespace
{
    void InsertPoint( const MT_Vector2D& point, void* userData )
    {
        static_cast< T_PointSet* >( userData )->insert( point );
    }
}

//-----------------------------------------------------------------------------
// Name: PathWalker::ComputeObjectCollision
// Created: MCO 2012-02-06
//-----------------------------------------------------------------------------
void PathWalker::ComputeObjectCollision( const SWORD_Model* object, void* userData )
{
    const CollisionData* data = static_cast< const CollisionData* >( userData );
    if( GET_HOOK( ObjectIntersect2D )( object, *data->lineTmp, &InsertPoint, data->collisions ) )
    {
        for( IT_PointSet itPoint = data->collisions->begin(); itPoint != data->collisions->end(); ++itPoint )
        {
            if( ! GET_HOOK( ObjectIsInside )( object, *itPoint ) )
                continue;
            IT_MoveStepSet itMoveStep = data->moveStepSet->insert( T_MoveStep( *itPoint ) ).first;
            const_cast< T_ObjectSet& >( itMoveStep->ponctualObjectsOnSet_ ).insert( object );
            // A - C - B ( Le point C ajouté entre A et B contient les mêmes objets que de A -> B)
            if( itMoveStep != data->moveStepSet->begin() )
            {
                IT_MoveStepSet itPrevMoveStep = itMoveStep;
                const_cast< T_ObjectSet& >( itMoveStep->objectsToNextPointSet_ ) = ( --itPrevMoveStep )->objectsToNextPointSet_;
            }
        }
        data->collisions->clear();
    }

    // Détermination si objet courant se trouve sur le trajet entre chaque point
    IT_MoveStepSet itPrevMoveStep = data->moveStepSet->begin();
    bool bInsideObjectOnPrevPoint = false;
    for( IT_MoveStepSet itMoveStep = ++( data->moveStepSet->begin() ); itMoveStep != data->moveStepSet->end(); ++itMoveStep )
    {
        // Picking au milieu de la ligne reliant les 2 points
        MT_Vector2D vTmp = ( itMoveStep->vPos_ + itPrevMoveStep->vPos_ ) / 2;
        if( GET_HOOK( ObjectIsInside )( object, vTmp ) )
        {
            const_cast< T_ObjectSet& >( itPrevMoveStep->objectsToNextPointSet_ ).insert( object );
            bInsideObjectOnPrevPoint = true;
            const_cast< T_ObjectSet& >( itPrevMoveStep->ponctualObjectsOnSet_ ).erase( object ); // This is not yet a ponctual object
        }
        else
        {
            // Stockage des objets desquels on sort
            if( bInsideObjectOnPrevPoint )
            {
                const_cast< T_ObjectSet& >( itMoveStep->objectsOutSet_ ).insert( object );
                bInsideObjectOnPrevPoint = false;
            }
        }
        itPrevMoveStep = itMoveStep;
    }
}

//-----------------------------------------------------------------------------
// Name: PathWalker::ComputeObjectsCollision
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
void PathWalker::ComputeObjectsCollision( const wrapper::View& model, const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet ) const
{
    // Récupération de la liste des objets dynamiques contenus dans le rayon vEnd - vStart
    MT_Line lineTmp( vStart, vEnd );
    moveStepSet.insert( T_MoveStep( vStart ) );
    moveStepSet.insert( T_MoveStep( vEnd   ) );
    TER_DistanceLess colCmp( vStart );
    T_PointSet collisions( colCmp );
    CollisionData data;
    data.lineTmp = &lineTmp;
    data.collisions = &collisions;
    data.moveStepSet = &moveStepSet;
    GET_HOOK( GetObjectListWithinCircle )( model, vStart, ( vEnd - vStart ).Magnitude(), &ComputeObjectCollision, &data );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::TryToMoveToNextStep
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PathWalker::TryToMoveToNextStep( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, double& rTimeRemaining, bool bFirstMove, const wrapper::View& entity ) const
{
    static const double rDistanceBeforeBlockingObject = GET_HOOK( GetWorldWeldValue )();
    T_ObjectSet::const_iterator it;

    // Prise en compte des objets ponctuels se trouvant sur le 'move step'
    for( it = itCurMoveStep->ponctualObjectsOnSet_.begin(); it != itCurMoveStep->ponctualObjectsOnSet_.end(); ++it )
    {
        const wrapper::View& object = *it;
        double rSpeedWithinObject = GET_HOOK( GetSpeedObject )( entity, environment_, object );
        if( GET_HOOK( CanObjectInteractWith )( entity, object ) )
        {
            if( !bFirstMove ) //// $$$$$ !bFirstMove A REVOIR - PERMET DE SORTIR D'UN OBSTACLE PONCTUEL
            {
                GET_HOOK( NotifyMovingInsideObject )( entity, object );
                if( rSpeedWithinObject == 0 )
                {
                    speed_ = 0;
                    vNewPos_ = itCurMoveStep->vPos_;
                    do
                    {
                        vNewPos_ -= vNewDir_ * rDistanceBeforeBlockingObject;
                    } while( GET_HOOK( ObjectIsInside )( object, vNewPos_ ) || GET_HOOK( ObjectIsOnBorder )( object, vNewPos_ ) );

                    GET_HOOK( NotifyMovingOutsideObject )( entity, object );  // $$$$ NLD 2007-05-07:
                    pathSet_ = eBlockedByObject;
                    return false;
                }
            }
            GET_HOOK( NotifyMovingOutsideObject )( entity, object );
        }
    }

    double rMaxSpeedForStep = std::numeric_limits< double >::max();
    for( it = itCurMoveStep->objectsToNextPointSet_.begin(); it != itCurMoveStep->objectsToNextPointSet_.end(); ++it )
    {
        const wrapper::View& object = *it;
        if( GET_HOOK( CanObjectInteractWith )( entity, object ) )
        {
            GET_HOOK( NotifyMovingInsideObject )( entity, object );
            rMaxSpeedForStep = std::min( rMaxSpeedForStep, GET_HOOK( GetSpeedObject )( entity, environment_, object) );
            if( rMaxSpeedForStep == 0 )
            {
                speed_ = 0;
                vNewPos_ = itCurMoveStep->vPos_;
                do
                {
                    vNewPos_ -= vNewDir_ * rDistanceBeforeBlockingObject;
                } while( GET_HOOK( ObjectIsInside )( object, vNewPos_ ) || GET_HOOK( ObjectIsOnBorder )( object, vNewPos_ ) );

                GET_HOOK( NotifyMovingOutsideObject )( entity, object );  // $$$$ NLD 2007-05-07: FOIREUX
                pathSet_ = eBlockedByObject;
                return false;
            }
        }
    }

    // itCurMoveStep a pu être dépassé => notification des objets dont on sort
    for( it = itNextMoveStep->objectsOutSet_.begin(); it != itNextMoveStep->objectsOutSet_.end(); ++it )
    {
        const wrapper::View& object = *it;
        if( GET_HOOK( CanObjectInteractWith )( entity, object ) )
            GET_HOOK( NotifyMovingOutsideObject )( entity, object );
    }

    if( rMaxSpeedForStep != std::numeric_limits< double >::max() )
        speed_ = rMaxSpeedForStep;
    const double rDistToWalk = rTimeRemaining * speed_;
    const MT_Vector2D vNewPosTmp( vNewPos_ + ( vNewDir_ * rDistToWalk ) );

    if( vNewPos_.SquareDistance( vNewPosTmp ) >= vNewPos_.SquareDistance( itNextMoveStep->vPos_ )  )
    {
        rTimeRemaining -= ( itNextMoveStep->vPos_ - vNewPos_ ).Magnitude() / speed_;
        vNewPos_ = itNextMoveStep->vPos_;
        return true;
    }
    else
    {
        rTimeRemaining -= ( vNewPosTmp - vNewPos_ ).Magnitude() / speed_;
        vNewPos_ = vNewPosTmp;
        return false;
    }
}

//-----------------------------------------------------------------------------
// Name: PathWalker::TryToMoveTo
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
bool PathWalker::TryToMoveTo( boost::shared_ptr< PathResult > path, const MT_Vector2D& vNewPosTmp, double& rTimeRemaining, const wrapper::View& model, const wrapper::View& entity ) const
{
    // Deplacement de vNewPos_ vers vNewPosTmp
    if( vNewPosTmp == vNewPos_ )
        return true;

    if( speed_ <= 0 )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Current speed is negative or null" );
        return false;
    }

    MT_Vector2D vNewPos( vNewPos_ );
    bool bFirstMove = ( static_cast< float >( vNewPos_.rX_ ) == static_cast< float >( ( *path->GetResult().begin() )->GetPos().rX_ ) &&
                        static_cast< float >( vNewPos_.rY_ ) == static_cast< float >( ( *path->GetResult().begin() )->GetPos().rY_ ) );
    if( bFirstMove )
        vNewPos = (*path->GetResult().begin())->GetPos();

    sMoveStepCmp cmp( vNewPos );
    T_MoveStepSet moveStepSet( cmp );
    ComputeObjectsCollision( model, vNewPos, vNewPosTmp, moveStepSet );
    if( moveStepSet.size() < 2 )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Move step set has not at least 2 elements" ); // $$$$ LDC RC Can never happen.
        return false;
    }
    CIT_MoveStepSet itCurMoveStep  = moveStepSet.begin();
    CIT_MoveStepSet itNextMoveStep = moveStepSet.begin();
    ++itNextMoveStep;
    while( rTimeRemaining > 0 )
    {
        if( !TryToMoveToNextStep( itCurMoveStep, itNextMoveStep, rTimeRemaining, bFirstMove, entity ) )
            return false;
        itCurMoveStep = itNextMoveStep;
        ++itNextMoveStep;
        if( itNextMoveStep == moveStepSet.end() )
            return true;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PathWalker::Move
// Created: NLD 2004-09-22
// Modified: MGD 2010-03-12
// -----------------------------------------------------------------------------
PathWalker::E_ReturnCode PathWalker::Move( boost::shared_ptr< PathResult > path, const wrapper::View& model, const wrapper::View& entity ) const
{
    if( bHasMoved_ )
        return eAlreadyMoving;

    PathResult::E_State nPathState = path->GetState();
    if( nPathState == Path_ABC::eInvalid || nPathState == Path_ABC::eImpossible || nPathState == Path_ABC::eCanceled )
        return eNotAllowed;

    if( nPathState == Path_ABC::eComputing )
    {
        bHasMoved_ = true;
        return eRunning;
    }

    pathSet_ = SetCurrentPath( path, model, entity );
    if( pathSet_ == eItineraireMustBeJoined )
        return eItineraireMustBeJoined;

    bHasMoved_ = true;
    ComputeCurrentSpeed( entity );
    vNewPos_ = MT_Vector2D( entity[ "movement/position/x" ], entity[ "movement/position/y" ] );
    vNewDir_ = MT_Vector2D( entity[ "movement/direction/x" ], entity[ "movement/direction/y" ] );

    if( ! entity[ "movement/is-ready" ] )
    {
        speed_ = 0;
        PostMovement( entity );
        return eRunning;
    }
    if( speed_ == 0 || ! entity[ "movement/can-move" ] )
    {
        speed_ = 0;
        PostMovement( entity );
        return eNotAllowed;
    }

    if( itNextPathPoint_ == path->GetResult().end() )
    {
        speed_ = 0;
        PostMovement( entity );
        return eFinished;
    }

    if( ! entity[ "movement/has-resources" ] )
    {
        speed_ = 0;
        if( !bFuelReportSent_ )
        {
            PostReport( entity, report::eRC_PlusDeCarburant );
            bFuelReportSent_ = true;
        }
        PostMovement( entity );
        return eNotEnoughFuel;
    }
    bFuelReportSent_ = false;

    double rTimeRemaining = 1.; // 1 dT
    if( ( *itNextPathPoint_ )->GetPos() != vNewPos_ )
        vNewDir_ = ( ( *itNextPathPoint_ )->GetPos() - vNewPos_ ).Normalize();

    distance_ = 0;
    while( rTimeRemaining > 0 )
    {
        const MT_Vector2D vPosBeforeMove( vNewPos_ );
        const bool moveTryResult = TryToMoveTo( path, ( *itNextPathPoint_ )->GetPos(), rTimeRemaining, model, entity );  // $$$$ VPR 2012-01-06: Modifies vNewPos_
        distance_ += vPosBeforeMove.Distance( vNewPos_ );

        if( !moveTryResult )
        {
            PostMovement( entity );
            return pathSet_;
        }

        if( GoToNextNavPoint( path, entity ) )
        {
            PostMovement( entity );
            return pathSet_;
        }

        if( itNextPathPoint_ == path->GetResult().end() )
        {
            speed_ = 0;
            PostMovement( entity );
            return ( pathSet_ != ePartialPath ) ? eFinished : pathSet_;
        }

        ComputeCurrentSpeed( entity );

        if( ( *itNextPathPoint_ )->GetPos() != vNewPos_ )
            vNewDir_ = ( ( *itNextPathPoint_ )->GetPos() - vNewPos_ ).Normalize();
    }
    PostMovement( entity );
    return pathSet_;
}

// -----------------------------------------------------------------------------
// Name: PathWalker::MoveSuspended
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PathWalker::MoveSuspended( boost::shared_ptr< PathResult > path, const wrapper::View& entity ) const
{
    if( path_.expired() && !bForcePathCheck_ )
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Move cannot be suspended" );
    if( path_.lock() == path )
        bForcePathCheck_ = true;
    speed_ = 0;
    MoveStopped( entity );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::MoveStopped
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void PathWalker::MoveStopped( const wrapper::View& entity ) const
{
    wrapper::Effect effect( entity[ "movement" ] );
    effect[ "speed" ] = 0;
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: PathWalker::MoveCanceled
// Created: NLD 2005-03-16
// -----------------------------------------------------------------------------
void PathWalker::MoveCanceled( boost::shared_ptr< PathResult > path ) const
{
    if( path_.lock() == path )
    {
        path_.reset();
        bForcePathCheck_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PathWalker::PostMovement
// Created: VPR 2012-01-09
// -----------------------------------------------------------------------------
void PathWalker::PostMovement( const wrapper::View& entity ) const
{
    wrapper::Effect effect( entity[ "movement" ] );
    effect[ "position/x" ] = vNewPos_.rX_;
    effect[ "position/y" ] = vNewPos_.rY_;
    effect[ "direction/x" ] = vNewDir_.rX_;
    effect[ "direction/y" ] = vNewDir_.rY_;
    effect[ "speed" ] = speed_;
    effect.Post();
    wrapper::Event event( "movement" );
    event[ "entity" ] = entity_;
    event[ "distance" ] = distance_;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: PathWalker::PostPath
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
void PathWalker::PostPath( const wrapper::View& entity ) const
{
    auto path = path_.lock();
    if( !path )
        return;
    wrapper::Effect effect( entity[ "movement/path" ] );
    wrapper::Node points = effect[ "points" ];
    effect[ "identifier" ] = path->GetID();
    const PathResult::T_PathPointList& pts = path->GetResult();
    PathResult::CIT_PathPointList it = pts.begin();
    std::advance( it, pointsPassed_ );
    for( std::size_t index = 0; it != pts.end() && index < pathSizeThreshold; ++it, ++index )
    {
        wrapper::Node point = points.AddElement();
        point[ "x" ] = (*it)->GetPos().rX_;
        point[ "y" ] = (*it)->GetPos().rY_;
    }
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: PathWalker::PostEnvironment
// Created: SLI 2012-01-23
// -----------------------------------------------------------------------------
void PathWalker::PostEnvironment( const wrapper::View& entity, const TerrainData& environment ) const
{
    wrapper::Effect effect( entity[ "movement/environment" ] );
    effect[ "area" ] = environment.Area();
    effect[ "left" ] = environment.Left();
    effect[ "right" ] = environment.Right();
    effect[ "linear" ] = environment.Linear();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: PathWalker::Clean
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PathWalker::Clean()
{
    if( !bHasMoved_ )
        bForcePathCheck_ = true;
    bHasMoved_ = false;
}
