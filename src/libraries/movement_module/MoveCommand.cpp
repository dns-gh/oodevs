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
#include "wrapper/View.h"
#include "wrapper/Event.h"
#include "wrapper/Hook.h"
#include "simulation_kernel/Entities/Orders/MIL_Report.h" // $$$$ MCO : for enums

using namespace sword;
using namespace sword::movement;

DECLARE_HOOK( EntityManagerFindObject, bool, ( unsigned int nID ) )
DECLARE_HOOK( GetKnowledgeObjectRealName, const char*, ( boost::shared_ptr< DEC_Knowledge_Object > object ) )
DECLARE_HOOK( GetObjectKnownId, int, ( boost::shared_ptr< DEC_Knowledge_Object > obstacle ) )
DECLARE_HOOK( UpdateObjectsToAvoid, bool, ( boost::shared_ptr< KnowledgeCache >& cache, const SWORD_Model* entity ) )

namespace
{
    void PostReport( const wrapper::View& entity, MIL_Report::E_DecisionalReport code, const std::string& name )
    {
        wrapper::Event event( "movement report with name" );
        event[ "entity/data" ] = entity[ "data" ];
        event[ "code" ] = code;
        event[ "name" ] = name;
        event.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: MoveCommand constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
MoveCommand::MoveCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& /*model*/ )
    : action_            ( parameters[ "action" ] )
    , identifier_        ( parameters[ "identifier" ] )
    , pMainPath_         ( boost::dynamic_pointer_cast< Agent_Path >( *static_cast< boost::shared_ptr< Path_ABC >* >( parameters[ "path" ].GetUserData() ) ) )
    , executionSuspended_( false )
    , isBlockedByObject_ ( false )
    , blockedTickCounter_( 0 )
    , pathWalker_        ( new PathWalker( module, identifier_ ) )
{
    if( ! pMainPath_ )
        throw std::invalid_argument( "Path pointer is of wrong type" );
    pMainPath_->AddRef();
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void MoveCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& model ) const // $$$$ _RC_ SLI 2012-01-02: moved from StopAction
{
    const wrapper::View& entity = model[ "entities" ][ identifier_ ];
    pathWalker_->MoveStopped( entity );
    if( pMainPath_ )
        pMainPath_->DecRef();
    //PostCallback( PathWalker::eFinished ); // $$$$ _RC_ SLI 2012-01-03: remove it?
}

// -----------------------------------------------------------------------------
// Name: MoveCommand::CreateNewPath
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
void MoveCommand::CreateNewPath( const wrapper::View& entity ) const
{
    assert( pMainPath_.get() );
    assert( pMainPath_->GetState() != Path_ABC::eComputing );
    const T_PointVector& nextWaypoints = pMainPath_->GetNextWaypoints();
    const PathType& pathType = pMainPath_->GetPathType();
    boost::shared_ptr< Agent_Path > pNewPath( new Agent_Path( entity, nextWaypoints, pathType ) );
    pNewPath->ComputePath( pNewPath );
    pathWalker_->MoveCanceled( pMainPath_ );
    pMainPath_->Cancel();
    pMainPath_->DecRef();
    pMainPath_ = pNewPath;
    pMainPath_->AddRef();
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
bool MoveCommand::AvoidObstacles( const wrapper::View& entity, const MT_Vector2D& /*position*/ ) const
{
    if( !pMainPath_ || pMainPath_->GetState() == Path_ABC::eComputing )
        return false;

    if( !GET_HOOK( UpdateObjectsToAvoid )( cache_, entity ) )
    {
        if( isBlockedByObject_ )
            blockedTickCounter_++;
        if( blockedTickCounter_ < 3 )
            return false;
    }
    blockedTickCounter_ = 0;

    boost::shared_ptr< DEC_Knowledge_Object > pObjectColliding;
    double rDistanceCollision = 0.;
    if( !pMainPath_->ComputeFutureObjectCollision( entity, *cache_, rDistanceCollision, pObjectColliding, isBlockedByObject_, true ) )
        return false;
    obstacleId_ = GET_HOOK( GetObjectKnownId )( pObjectColliding );
    PostReport( entity, MIL_Report::eRC_DifficultMovementProgression, GET_HOOK( GetKnowledgeObjectRealName )( pObjectColliding ) );

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

    if( !pMainPath_ )
        return PostCallback( PathWalker::eNotAllowed );

    if( ( AvoidObstacles( entity, position ) && GET_HOOK( EntityManagerFindObject )( obstacleId_ ) ) ||
        ( executionSuspended_ && pMainPath_->GetState() != Path_ABC::eComputing &&
        ( pMainPath_->GetCurrentKeyOnPath() == pMainPath_->GetResult().end() || position != (*pMainPath_->GetCurrentKeyOnPath())->GetPos() ) ) )
    {
        // Recompute Pathfind in order to avoid obstacle or to get back previous path after suspension.
        CreateNewPath( entity );
    }
    executionSuspended_ = false;
    isBlockedByObject_ = false;
    PathWalker::E_ReturnCode nReturn = pathWalker_->Move( pMainPath_, model, entity );
    if( nReturn == PathWalker::eRunning )
    { // NOTHING. Pathfind is computing. Just don't try to do anything in this state.
    }
    else if( nReturn == PathWalker::eNotAllowed )
    { // NOTHING. Unit will not move at all.
    }
    else if( nReturn == PathWalker::eItineraireMustBeJoined )
    {
        CreateNewPath( entity );
        nReturn = pathWalker_->Move( pMainPath_, model, entity );
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
    if( pMainPath_.get() )
    {
        const wrapper::View& entity = model[ "entities" ][ identifier_ ];
        pathWalker_->MoveSuspended( pMainPath_, entity );
        executionSuspended_ = true;
    }
    PostCallback( PathWalker::ePaused );
}
