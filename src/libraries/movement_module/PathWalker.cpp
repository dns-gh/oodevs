// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "PathWalker.h"
#include "PathPoint.h"
#include "ModuleFacade.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"
#include "wrapper/Hook.h"
#include "simulation_kernel/Entities/Orders/MIL_Report.h" // $$$$ MCO : for enums
#include <module_api/Log.h>
#include <boost/foreach.hpp>

namespace sword
{
namespace movement
{

DECLARE_HOOK( CanMove, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( CanObjectInteractWith, bool, ( const SWORD_Model* entity, const MIL_Object_ABC& object ) )
DECLARE_HOOK( GetObjectListWithinCircle, void, ( const MT_Vector2D& vCenter, double rRadius, void (*callback)( MIL_Object_ABC* object, void* userData ), void* userData ) )
DECLARE_HOOK( GetObjectId, int, ( const MIL_Object_ABC& object ) )
DECLARE_HOOK( GetObjectRealName, const char*, ( const MIL_Object_ABC& object ) )
DECLARE_HOOK( GetSpeedWithReinforcement, double, ( const SWORD_Model* entity, const TerrainData& environment ) )
DECLARE_HOOK( GetSpeedWithReinforcementObject, double, ( const SWORD_Model* entity, const TerrainData& environment, const MIL_Object_ABC& object ) )
DECLARE_HOOK( GetWorldWeldValue, double, () ) // $$$$ MCO : "static data" which amazingly is hard-coded to 10 in simulation_terrain
DECLARE_HOOK( HasResources, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( NotifyMovingInsideObject, void, ( const SWORD_Model* entity, MIL_Object_ABC& object ) )
DECLARE_HOOK( NotifyMovingOnPathPoint, void, ( const SWORD_Model* entity, const MT_Vector2D& point ) )
DECLARE_HOOK( NotifyMovingOutsideObject, void, ( const SWORD_Model* entity, MIL_Object_ABC& object ) )
DECLARE_HOOK( ObjectIntersect2D, bool, ( const MIL_Object_ABC& object, const MT_Line& line, void (*callback)( const MT_Vector2D& point, void* userData ), void* userData ) )
DECLARE_HOOK( ObjectIsInside, bool, ( const MIL_Object_ABC& object, const MT_Vector2D& point ) )
DECLARE_HOOK( ObjectIsOnBorder, bool, ( const MIL_Object_ABC& object, const MT_Vector2D& point ) )

void PostReport( unsigned int entity, MIL_Report::E_EngineReport code )
{
    wrapper::Event event( "movement report" );
    event[ "entity" ] = entity;
    event[ "code" ] = code;
    event.Post();
}

void PostReport( unsigned int entity, MIL_Report::E_EngineReport code, const std::string& name )
{
    wrapper::Event event( "movement report with name" );
    event[ "entity" ] = entity;
    event[ "code" ] = code;
    event[ "name" ] = name;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: PathWalker constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PathWalker::PathWalker( ModuleFacade& module, unsigned int entity )
    : module_            ( module )
    , entity_            ( entity )
    , itNextPathPoint_   ()
    , itCurrentPathPoint_()
    , vNewPos_           ( 0., 0. )
    , vNewDir_           ( 0., 0. )
    , rCurrentSpeed_     ( 0. )
    , rWalkedDistance_   ( 0. )
    , bForcePathCheck_   ( true )
    , bHasMoved_         ( false )
    , bFuelReportSent_   ( false )
    , pathSet_           ( eFinished )
{
    module.Register( entity_, *this );
}

// -----------------------------------------------------------------------------
// Name: PathWalker destructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
PathWalker::~PathWalker()
{
    module_.Unregister( entity_ );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::ComputeFutureObjectCollision
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool PathWalker::ComputeFutureObjectCollision( const wrapper::View& entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, bool blockedByObject, bool applyScale ) const
{
    if( !pCurrentPath_ )
        return false;
    return pCurrentPath_->ComputeFutureObjectCollision( entity, objectsToTest, rDistance, pObject, blockedByObject, applyScale );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::IsMovingOn
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
bool PathWalker::IsMovingOn( const Path_ABC& path ) const
{
    return pCurrentPath_.get() ? path == *pCurrentPath_ : false;
}

//-----------------------------------------------------------------------------
// Name: PathWalker::ExtrapolatePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D PathWalker::ExtrapolatePosition( const wrapper::View& entity, const double rTime, const bool bBoundOnPath ) const
{
    const MT_Vector2D position( entity[ "movement/position/x" ], entity[ "movement/position/y" ] );
    if( !pCurrentPath_.get() )
        return position;
    try
    {
        return pCurrentPath_->GetFuturePosition( position, rTime * entity[ "movement/speed" ], bBoundOnPath );
    }
    catch( std::exception& e )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
        return position;
    }
}

// -----------------------------------------------------------------------------
// Name: PathWalker::ComputeCurrentSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PathWalker::ComputeCurrentSpeed( const wrapper::View& entity ) const
{
    const PathPoint& curPathPoint = **itCurrentPathPoint_;
    if( curPathPoint.GetType() == PathPoint::eTypePointPath )
        SetEnvironmentType( curPathPoint.GetObjectTypesToNextPoint(), entity );
    rCurrentSpeed_ = GET_HOOK( GetSpeedWithReinforcement )( entity, environment_ );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::InitializeEnvironment
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PathWalker::InitializeEnvironment( const PathResult& path, const wrapper::View& entity ) const
{
    PathResult::CIT_PathPointList itPathPointTmp = itCurrentPathPoint_;
    while( itPathPointTmp != path.GetResult().end() && ( *itPathPointTmp )->GetType() != PathPoint::eTypePointPath )
        ++itPathPointTmp;
    if( itPathPointTmp == path.GetResult().end() )
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
    void AddObject( MIL_Object_ABC* object, void* userData )
    {
        static_cast< std::vector< const MIL_Object_ABC* >* >( userData )->push_back( object );
    }
}

// -----------------------------------------------------------------------------
// Name: PathWalker::SetCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PathWalker::E_ReturnCode PathWalker::SetCurrentPath( boost::shared_ptr< PathResult > pPath, const wrapper::View& entity ) const
{
    if( pCurrentPath_.get() && pPath == pCurrentPath_ && !bForcePathCheck_  /*&& !GetRole< PHY_RoleInterface_Location >().HasDoneMagicMove()*/ )
        return eRunning;

    PathWalker::E_ReturnCode rc = eRunning;
    bool bCanSendTerrainReport = pPath != pCurrentPath_;
    pCurrentPath_ = pPath;
    pPath->InsertDecPoints(); // $$$ HIDEUX
    PostPath( entity, *pCurrentPath_ );
    bForcePathCheck_ = false;
    if( pPath->GetResult().empty() )
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "List of path points resulting from pathfind is empty" );
    itCurrentPathPoint_ = pPath->GetCurrentKeyOnPath();
    if( itCurrentPathPoint_ == pPath->GetResult().end() )
        return eItineraireMustBeJoined;
    if( *itCurrentPathPoint_ )
        pPath->NotifyPointReached( itCurrentPathPoint_ );
    if( ( pPath->GetState() == PathResult::ePartial ) && bCanSendTerrainReport )
    {
        bool isInsideObject = false;
        const MT_Vector2D& lastWaypoint = pPath->GetLastWaypoint();
        typedef std::vector< const MIL_Object_ABC* > T_Objects;
        T_Objects objects;
        GET_HOOK( GetObjectListWithinCircle )( lastWaypoint, 100, &AddObject, &objects );
        for( T_Objects::const_iterator it = objects.begin(); it != objects.end(); ++it )
        {
            if( GET_HOOK( ObjectIsInside )( **it, lastWaypoint ) )
            {
                PostReport( entity_, MIL_Report::eReport_DifficultMovementProgression, GET_HOOK( GetObjectRealName )( **it ) );
                isInsideObject = true;
                break;
            }
        }
        if( !isInsideObject )
            PostReport( entity_, MIL_Report::eReport_DifficultTerrain );
        rc = ePartialPath;
    }
    itNextPathPoint_ = itCurrentPathPoint_;
    ++itNextPathPoint_;
    InitializeEnvironment( *pPath, entity );
    return rc;
}

//-----------------------------------------------------------------------------
// Name: PathWalker::GoToNextNavPoint
// Created: JVT 02-12-06
// Last modified: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
bool PathWalker::GoToNextNavPoint( PathResult& path, const wrapper::View& entity ) const
{
    if( ( *itNextPathPoint_ )->GetType() == PathPoint::eTypePointPath )
    {
        GET_HOOK( NotifyMovingOnPathPoint )( entity, (*itNextPathPoint_)->GetPos() ); // $$$$ MCO : only for populations...
        SetCurrentPathPoint( path );
        ++itNextPathPoint_;
        return false;
    }
    // points particuliers -> EVT vers DEC
    do
    {
        (*itNextPathPoint_)->SendToDIA( entity_, *itNextPathPoint_ );
        SetCurrentPathPoint( path );
    }
    while( ++itNextPathPoint_ != path.GetResult().end() &&
         ( *itNextPathPoint_ )->GetType() != PathPoint::eTypePointPath && ( *itNextPathPoint_ )->GetPos() == vNewPos_ );
    return true;
}

//-----------------------------------------------------------------------------
// Name: PathWalker::SetCurrentPathPoint
// Created: LDC 2012-01-18
//-----------------------------------------------------------------------------
void PathWalker::SetCurrentPathPoint( PathResult& path ) const
{
    itCurrentPathPoint_ = itNextPathPoint_;
    if( itCurrentPathPoint_ != path.GetResult().end() && *itCurrentPathPoint_ )
        path.NotifyPointReached( itCurrentPathPoint_ );
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
void PathWalker::ComputeObjectCollision( MIL_Object_ABC* object, void* userData )
{
    const CollisionData* data = static_cast< const CollisionData* >( userData );
    if( GET_HOOK( ObjectIntersect2D )( *object, *data->lineTmp, &InsertPoint, data->collisions ) )
    {
        for( IT_PointSet itPoint = data->collisions->begin(); itPoint != data->collisions->end(); ++itPoint )
        {
            if( ! GET_HOOK( ObjectIsInside )( *object, *itPoint ) )
                continue;
            IT_MoveStepSet itMoveStep = data->moveStepSet->insert( T_MoveStep( *itPoint ) ).first;
            const_cast< T_ObjectSet& >( itMoveStep->ponctualObjectsOnSet_ ).insert( object );
            // A - C - B ( Le point C ajout� entre A et B contient les m�mes objets que de A -> B)
            if( itMoveStep != data->moveStepSet->begin() )
            {
                IT_MoveStepSet itPrevMoveStep = itMoveStep;
                const_cast< T_ObjectSet& >( itMoveStep->objectsToNextPointSet_ ) = ( --itPrevMoveStep )->objectsToNextPointSet_;
            }
        }
        data->collisions->clear();
    }

    // D�termination si objet courant se trouve sur le trajet entre chaque point
    IT_MoveStepSet itPrevMoveStep = data->moveStepSet->begin();
    bool bInsideObjectOnPrevPoint = false;
    for( IT_MoveStepSet itMoveStep = ++( data->moveStepSet->begin() ); itMoveStep != data->moveStepSet->end(); ++itMoveStep )
    {
        // Picking au milieu de la ligne reliant les 2 points
        MT_Vector2D vTmp = ( itMoveStep->vPos_ + itPrevMoveStep->vPos_ ) / 2;
        if( GET_HOOK( ObjectIsInside )( *object, vTmp ) )
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
void PathWalker::ComputeObjectsCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet ) const
{
    // R�cup�ration de la liste des objets dynamiques contenus dans le rayon vEnd - vStart
    MT_Line lineTmp( vStart, vEnd );
    moveStepSet.insert( T_MoveStep( vStart ) );
    moveStepSet.insert( T_MoveStep( vEnd   ) );
    TER_DistanceLess colCmp( vStart );
    T_PointSet collisions( colCmp );
    CollisionData data;
    data.lineTmp = &lineTmp;
    data.collisions = &collisions;
    data.moveStepSet = &moveStepSet;
    GET_HOOK( GetObjectListWithinCircle )( vStart, ( vEnd - vStart ).Magnitude(), &ComputeObjectCollision, &data );
}

// -----------------------------------------------------------------------------
// Name: PathWalker::TryToMoveToNextStep
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PathWalker::TryToMoveToNextStep( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, double& rTimeRemaining, bool bFirstMove, const wrapper::View& entity ) const
{
    static const double rDistanceBeforeBlockingObject = GET_HOOK( GetWorldWeldValue )();
    CIT_ObjectSet it;

    // Prise en compte des objets ponctuels se trouvant sur le 'move step'
    for( it = itCurMoveStep->ponctualObjectsOnSet_.begin(); it != itCurMoveStep->ponctualObjectsOnSet_.end(); ++it )
    {
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **it );
        double rSpeedWithinObject = GET_HOOK( GetSpeedWithReinforcementObject )( entity, environment_, object );
        if( GET_HOOK( CanObjectInteractWith )( entity, object ) )
        {
            if( !bFirstMove ) //// $$$$$ !bFirstMove A REVOIR - PERMET DE SORTIR D'UN OBSTACLE PONCTUEL
            {
                GET_HOOK( NotifyMovingInsideObject )( entity, object );
                if( rSpeedWithinObject == 0. )
                {
                    rCurrentSpeed_ = 0;
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
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **it );
        if( GET_HOOK( CanObjectInteractWith )( entity, object ) )
        {
            GET_HOOK( NotifyMovingInsideObject )( entity, object );
            rMaxSpeedForStep = std::min( rMaxSpeedForStep, GET_HOOK( GetSpeedWithReinforcementObject )( entity, environment_, object) );
            if( rMaxSpeedForStep == 0. )
            {
                rCurrentSpeed_ = 0;
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

    // itCurMoveStep a pu �tre d�pass� => notification des objets dont on sort
    for( it = itNextMoveStep->objectsOutSet_.begin(); it != itNextMoveStep->objectsOutSet_.end(); ++it )
    {
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **it );
        if( GET_HOOK( CanObjectInteractWith )( entity, object ) )
            GET_HOOK( NotifyMovingOutsideObject )( entity, object );
    }

    if( rMaxSpeedForStep != std::numeric_limits< double >::max() )
        rCurrentSpeed_ = rMaxSpeedForStep;
    const double rDistToWalk = rTimeRemaining * rCurrentSpeed_;
    const MT_Vector2D vNewPosTmp( vNewPos_ + ( vNewDir_ * rDistToWalk ) );

    if( vNewPos_.SquareDistance( vNewPosTmp ) >= vNewPos_.SquareDistance( itNextMoveStep->vPos_ )  )
    {
        rTimeRemaining -= ( itNextMoveStep->vPos_ - vNewPos_ ).Magnitude() / rCurrentSpeed_;
        vNewPos_ = itNextMoveStep->vPos_;
        return true;
    }
    else
    {
        rTimeRemaining -= ( vNewPosTmp - vNewPos_ ).Magnitude() / rCurrentSpeed_;
        vNewPos_ = vNewPosTmp;
        return false;
    }
}

//-----------------------------------------------------------------------------
// Name: PathWalker::TryToMoveTo
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
bool PathWalker::TryToMoveTo( const PathResult& path, const MT_Vector2D& vNewPosTmp, double& rTimeRemaining, const wrapper::View& entity ) const
{
    // Deplacement de vNewPos_ vers vNewPosTmp
    if( vNewPosTmp == vNewPos_ )
        return true;

    if( rCurrentSpeed_ <= 0. )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Current speed is not positive" );
        return false;
    }

    MT_Vector2D vNewPos( vNewPos_ );
    bool bFirstMove = ( static_cast< float >( vNewPos_.rX_ ) == static_cast< float >( ( *path.GetResult().begin() )->GetPos().rX_ ) &&
                        static_cast< float >( vNewPos_.rY_ ) == static_cast< float >( ( *path.GetResult().begin() )->GetPos().rY_ ) );
    if( bFirstMove )
        vNewPos = (*path.GetResult().begin())->GetPos();

    sMoveStepCmp cmp( vNewPos );
    T_MoveStepSet moveStepSet( cmp );
    ComputeObjectsCollision( vNewPos, vNewPosTmp, moveStepSet );
    if( moveStepSet.size() < 2 )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Move step set has not at least 2 elements" ); // $$$$ LDC RC Can never happen.
        return false;
    }
    CIT_MoveStepSet itCurMoveStep  = moveStepSet.begin();
    CIT_MoveStepSet itNextMoveStep = moveStepSet.begin();
    ++itNextMoveStep;
    while( rTimeRemaining > 0. )
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
int PathWalker::Move( boost::shared_ptr< PathResult > pPath, const wrapper::View& entity ) const
{
    if( bHasMoved_ )
        return eAlreadyMoving;

    PathResult::E_State nPathState = pPath->GetState();
    if( nPathState == Path_ABC::eInvalid || nPathState == Path_ABC::eImpossible || nPathState == Path_ABC::eCanceled )
        return eNotAllowed;

    if( nPathState == Path_ABC::eComputing )
    {
        bHasMoved_ = true;
        return eRunning;
    }

    pathSet_ = SetCurrentPath( pPath, entity );
    if( pathSet_ == eItineraireMustBeJoined )
        return eItineraireMustBeJoined;

    bHasMoved_ = true;
    ComputeCurrentSpeed( entity );
    vNewPos_ = MT_Vector2D( entity[ "movement/position/x" ], entity[ "movement/position/y" ] );
    vNewDir_ = MT_Vector2D( entity[ "movement/direction/x" ], entity[ "movement/direction/y" ] );

    if( rCurrentSpeed_ == 0. || ! GET_HOOK( CanMove )( entity ) )
    {
        rCurrentSpeed_ = 0.;
        PostMovement( entity );
        return eNotAllowed;
    }

    if( itNextPathPoint_ == pPath->GetResult().end() )
    {
        rCurrentSpeed_ = 0.;
        PostMovement( entity );
        return eFinished;
    }

    if( ! GET_HOOK( HasResources )( entity ) )
    {
        rCurrentSpeed_ = 0.;
        if( !bFuelReportSent_ )
        {
            PostReport( entity_, MIL_Report::eReport_OutOfGas );
            bFuelReportSent_ = true;
        }
        PostMovement( entity );
        return eNotEnoughFuel;
    }
    bFuelReportSent_ = false;

    double rTimeRemaining = 1.; // 1 dT
    if( ( *itNextPathPoint_ )->GetPos() != vNewPos_ )
        vNewDir_ = ( ( *itNextPathPoint_ )->GetPos() - vNewPos_ ).Normalize();

    rWalkedDistance_ = 0;
    while( rTimeRemaining > 0. )
    {
        const MT_Vector2D vPosBeforeMove( vNewPos_ );
        const bool moveTryResult = TryToMoveTo( *pPath, ( *itNextPathPoint_ )->GetPos(), rTimeRemaining, entity );  // $$$$ VPR 2012-01-06: Modifies vNewPos_
        rWalkedDistance_ += vPosBeforeMove.Distance( vNewPos_ );

        if( !moveTryResult )
        {
            if( pathSet_ == eBlockedByObject )
                PostMovement( entity ); // $$$$ VPR : Replaced by Apply
            PostMovement( entity );
            return pathSet_;
        }

        if( GoToNextNavPoint( *pPath, entity ) )
        {
            PostMovement( entity );
            return pathSet_;
        }

        if( itNextPathPoint_ == pPath->GetResult().end() )
        {
            rCurrentSpeed_ = 0;
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
void PathWalker::MoveSuspended( boost::shared_ptr< PathResult > pPath ) const
{
    if( !pCurrentPath_.get() && !bForcePathCheck_ )
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Move cannot be suspended" );
    if( pCurrentPath_.get() && pCurrentPath_ == pPath )
        bForcePathCheck_ = true;
}

// -----------------------------------------------------------------------------
// Name: PathWalker::MoveCanceled
// Created: NLD 2005-03-16
// -----------------------------------------------------------------------------
void PathWalker::MoveCanceled( boost::shared_ptr< PathResult > pPath ) const
{
    if( pCurrentPath_ == pPath )
    {
        pCurrentPath_.reset();
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
    effect[ "speed" ] = rCurrentSpeed_;
    effect.Post();
    wrapper::Event event( "movement" );
    event[ "entity" ] = entity_;
    event[ "distance" ] = rWalkedDistance_;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: PathWalker::PostPath
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
void PathWalker::PostPath( const wrapper::View& entity, PathResult& path ) const
{
    wrapper::Effect effect( entity[ "movement/path" ] );
    wrapper::Node points = effect[ "points" ];
    BOOST_FOREACH( boost::shared_ptr< PathPoint > point, path.GetResult() )
    {
        const MT_Vector2D& position = point->GetPos();
        wrapper::Node pointNode = points.AddElement();
        pointNode[ "x" ] = position.rX_;
        pointNode[ "y" ] = position.rY_;
    }
    effect[ "identifier" ] = path.GetID();
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

}
}
