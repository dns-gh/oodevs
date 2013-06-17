// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MoveCommand.h"
#include "Agent_Path.h"
#include "PathPoint.h"
#include "ModuleFacade.h"
#include "wrapper/View.h"
#include "wrapper/Event.h"
#include "wrapper/Effect.h"
#include "wrapper/Hook.h"
#include "simulation_kernel/Entities/Orders/MIL_DecisionalReport.h" // $$$$ MCO : for enums
#include <tools/Exception.h>

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( EntityManagerFindObject, bool, ( unsigned int nID ) )
DECLARE_HOOK( CreateKnowledgeCache, KnowledgeCache*, () )
DECLARE_HOOK( DeleteKnowledgeCache, void, ( KnowledgeCache* cache ) )
DECLARE_HOOK( UpdateObjectsToAvoid, bool, ( KnowledgeCache* cache, const SWORD_Model* entity ) )
DECLARE_HOOK( CancelPathFindJob, void, ( size_t path ) )

namespace
{
    void PostReport( const wrapper::View& entity, const MIL_DecisionalReport& code, const std::string& name )
    {
        wrapper::Event event( "movement report with name" );
        event[ "entity/data" ] = entity[ "data" ];
        event[ "code" ] = code;
        event[ "name" ] = name;
        event.Post();
    }
    void PostMovementIntention( const wrapper::View& entity, bool intention )
    {
        wrapper::Effect effect( entity[ "movement/intention" ] );
        effect = intention;
        effect.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: MoveCommand constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
MoveCommand::MoveCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model )
    : module_            ( module )
    , action_            ( parameters[ "action" ] )
    , identifier_        ( parameters[ "identifier" ] )
    , mainPath_          ( module.GetPath( parameters[ "path" ] ) )
    , cache_             ( GET_HOOK( CreateKnowledgeCache )(), boost::bind( GET_HOOK( DeleteKnowledgeCache ), _1 ) )
    , executionSuspended_( false )
    , isBlockedByObject_ ( false )
    , blockedTickCounter_( 0 )
    , pathWalker_        ( new PathWalker( module, identifier_ ) )
{
    if( mainPath_.expired() )
        throw MASA_EXCEPTION( "Path pointer is of wrong type." );
    PostMovementIntention( model[ "entities" ][ identifier_ ], true );
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void MoveCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& model ) const // $$$$ _RC_ SLI 2012-01-02: moved from StopAction
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    pathWalker_->MoveStopped( entity );
    auto mainPath = mainPath_.lock();
    if( mainPath )
        GET_HOOK( CancelPathFindJob )( mainPath->GetID() );
    PostMovementIntention( model[ "entities" ][ identifier_ ], false );
    wrapper::Effect effect( entity[ "movement" ] );
    effect[ "path/points" ];
    effect.Post();
    //PostCallback( PathWalker::eFinished ); // $$$$ _RC_ SLI 2012-01-03: remove it?
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::CreateNewPath
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
void MoveCommand::CreateNewPath( const wrapper::View& entity ) const
{
    boost::shared_ptr< Agent_Path > mainPath = mainPath_.lock();
    assert( mainPath->GetState() != Path_ABC::eComputing );
    const T_PointVector& nextWaypoints = mainPath->GetNextWaypoints();
    const PathType& pathType = mainPath->GetPathType();
    boost::shared_ptr< Agent_Path > pNewPath( new Agent_Path( module_, entity, nextWaypoints, pathType ) );
    module_.RegisterPath( pNewPath );
    pNewPath->Initialize();
    pathWalker_->MoveCanceled( mainPath );
    mainPath->Cancel();
    GET_HOOK( CancelPathFindJob )( mainPath->GetID() );
    mainPath_ = pNewPath;
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::PostCallback
// Created: MCO 2012-01-05
// -----------------------------------------------------------------------------
void MoveCommand::PostCallback( sword::movement::PathWalker::E_ReturnCode code ) const
{
    wrapper::Event event( "movement callback" );
    event[ "entity" ] = identifier_;
    event[ "action" ] = action_;
    event[ "code" ] = code;
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::AvoidObstacles
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
bool MoveCommand::AvoidObstacles( const wrapper::View& model, const wrapper::View& entity, const MT_Vector2D& /*position*/ ) const
{
    auto mainPath = mainPath_.lock();
    if( !mainPath )
        throw MASA_EXCEPTION( "Invalid path while avoiding obstacles" );
    if( mainPath->GetState() == Path_ABC::eComputing )
        return false;
    if( !GET_HOOK( UpdateObjectsToAvoid )( cache_.get(), entity ) )
    {
        if( isBlockedByObject_ )
            blockedTickCounter_++;
        if( blockedTickCounter_ < 3 )
            return false;
    }
    blockedTickCounter_ = 0;

    wrapper::View knowledgeObjectColliding;
    double rDistanceCollision = 0.;
    if( !mainPath->ComputeFutureObjectCollision( model, entity, cache_.get(), rDistanceCollision, &knowledgeObjectColliding, isBlockedByObject_, true ) )
        return false;
    obstacleId_ = knowledgeObjectColliding[ "identifier" ];
    PostReport( entity, report::eRC_DifficultMovementProgression, knowledgeObjectColliding[ "real-name" ] );

    return true;
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::Execute
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void MoveCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& model ) const
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    const wrapper::View& movement = entity[ "movement" ];
    const MT_Vector2D position( movement[ "position/x" ], movement[ "position/y" ] );
    pathWalker_->Clean();  // $$$$ _RC_ VPR 2012-01-09: RAII this !

    if( entity[ "is-deployed" ] ) // $$$$ ABR 2011-12-19: not IsUndeployed == IsDeployed || IsDeploying || IsUndeploying -> Can't move, no call back.
        return;

    if( mainPath_.expired() )
        return PostCallback( PathWalker::eNotAllowed );

    boost::shared_ptr< PathResult > mainPath = mainPath_.lock();
    if( ( AvoidObstacles( model, entity, position ) && GET_HOOK( EntityManagerFindObject )( obstacleId_ ) ) ||
        ( executionSuspended_ && mainPath->GetState() != Path_ABC::eComputing &&
        ( mainPath->GetCurrentKeyOnPath() == mainPath->GetResult().end() || position != (*mainPath->GetCurrentKeyOnPath())->GetPos() ) ) )
    {
        // Recompute Pathfind in order to avoid obstacle or to get back previous path after suspension.
        CreateNewPath( entity );
    }
    executionSuspended_ = false;
    isBlockedByObject_ = false;
    PathWalker::E_ReturnCode nReturn = pathWalker_->Move( mainPath, model, entity );
    if( nReturn == PathWalker::eRunning )
    { // NOTHING. Pathfind is computing. Just don't try to do anything in this state.
    }
    else if( nReturn == PathWalker::eNotAllowed )
    { // NOTHING. Unit will not move at all.
    }
    else if( nReturn == PathWalker::eItineraireMustBeJoined )
    {
        CreateNewPath( entity );
        nReturn = pathWalker_->Move( mainPath, model, entity );
    }
    else if( nReturn == PathWalker::ePartialPath )
    { // NOTHING. Unit will move to the nearest point of the target.
    }
    else if( nReturn == PathWalker::eBlockedByObject )
    { // Pathfind will be recomputed in a next tick once knowledge of the object has been obtained.
        isBlockedByObject_ = true;
    }
    PostCallback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::ExecutePaused
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void MoveCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& model ) const
{
    if( !mainPath_.expired() )
    {
        const wrapper::View& entity = model[ "entities" ][ identifier_ ];
        pathWalker_->MoveSuspended( mainPath_.lock(), entity );
        executionSuspended_ = true;
    }
    PostCallback( PathWalker::ePaused );
}
