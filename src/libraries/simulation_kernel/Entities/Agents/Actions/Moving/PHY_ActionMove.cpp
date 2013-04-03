// *****************************************************************************
//
// $Bypassd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Moving/PHY_ActionMove.cpp $
// $Author: Nld $
// $Modtime: 16/03/05 16:04 $
// $Revision: 5 $
// $Workfile: PHY_ActionMove.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionMove.h"
#include "PHY_RoleAction_Moving.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_Agent_PathClass.h"
#include "Decision/DEC_PathPoint.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectLoader.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMove::PHY_ActionMove( MIL_AgentPion& pion, boost::shared_ptr< DEC_Path_ABC > pPath )
    : PHY_DecisionCallbackAction_ABC( pion )
    , pion_( pion )
    , role_( pion.GetRole< moving::PHY_RoleAction_Moving >() )
    , pMainPath_( boost::dynamic_pointer_cast< DEC_Agent_Path >( pPath ) )
    , executionSuspended_( false )
    , isBlockedByObject_( false )
    , blockedTickCounter_( 0 )
    , obstacleId_( 0 )
{
    if( pMainPath_.get() )
        pMainPath_->AddRef();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove destructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMove::~PHY_ActionMove()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::UpdateObjectsToAvoid
// Created: NLD 2006-04-28
// -----------------------------------------------------------------------------
bool PHY_ActionMove::UpdateObjectsToAvoid()
{
    T_KnowledgeObjectVector knowledges;
    T_KnowledgeObjectVector newKnowledges;
    MIL_PathObjectFilter filter;
    pion_.GetArmy().GetKnowledge().GetObjectsAtInteractionHeight( knowledges, pion_, filter );
    for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
    {
        double cost = pMainPath_->GetPathClass().GetObjectCost( (*it)->GetType() );
        if( 0. != cost )
            newKnowledges.push_back( *it );
    }
    if( newKnowledges != objectsToAvoid_ )
    {
        objectsToAvoid_ = newKnowledges;
        geometrySignatures_.clear();
        for( auto it = newKnowledges.begin(); it != newKnowledges.end(); ++it )
            geometrySignatures_.push_back( (*it)->GetLocalisation() );
        return true;
    }
    else
    {
        bool modified = false;
        for( int i = 0; i < newKnowledges.size(); ++i )
        {
            if( geometrySignatures_[i] != newKnowledges[i]->GetLocalisation() )
            {
                geometrySignatures_[i] = newKnowledges[i]->GetLocalisation();
                modified = true;
            }
        }
        if( modified )
        {
            objectsToAvoid_ = newKnowledges;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::AvoidObstacles
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
bool PHY_ActionMove::AvoidObstacles()
{
    if( !pMainPath_.get() || pMainPath_->GetState() == DEC_Path_ABC::eComputing )
        return false;

    if( !UpdateObjectsToAvoid() )
    {
        if( isBlockedByObject_ )
            blockedTickCounter_++;
        if( blockedTickCounter_ < 3 )
            return false;
    }
    blockedTickCounter_ = 0;

    boost::shared_ptr< DEC_Knowledge_Object > pObjectColliding;
    double rDistanceCollision = 0.;
    if( !role_.ComputeFutureObjectCollision( objectsToAvoid_, rDistanceCollision, pObjectColliding, pion_, isBlockedByObject_, true ) )
        return false;

    assert( pObjectColliding && pObjectColliding->IsValid() );
    obstacleId_ = pObjectColliding->GetObjectKnown() ? pObjectColliding->GetObjectKnown()->GetID() : 0;

    const std::string name = MIL_ObjectLoader::GetLoader().GetType( pObjectColliding->GetType().GetName() ).GetRealName();
    if( pMainPath_->GetPathClass().AvoidObjects() )
        role_.SendRC( MIL_Report::eRC_DifficultMovementProgression, name );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::CreateNewPath
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
void PHY_ActionMove::CreateNewPath()
{
    assert( pMainPath_.get() );
    assert( pMainPath_->GetState() != DEC_Path_ABC::eComputing );
    const T_PointVector& nextWaypoints = pMainPath_->GetNextWaypoints();
    const DEC_PathType& pathType = pMainPath_->GetPathType();
    boost::shared_ptr< DEC_Agent_Path > pNewPath( new DEC_Agent_Path( pion_, nextWaypoints, pathType ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pNewPath );
    role_.MoveCanceled( pMainPath_ );
    pMainPath_->Cancel();
    pMainPath_->DecRef();
    pMainPath_ = pNewPath;
    pMainPath_->AddRef();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::Execute
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionMove::Execute()
{
    if( !pion_.GetRole< PHY_RoleInterface_Deployment >().IsUndeployed() ) // $$$$ ABR 2011-12-19: not IsUndeployed == IsDeployed || IsDeploying || IsUndeploying -> Can't move, no call back.
        return;
    if( !pMainPath_.get() )
    {
        Callback( static_cast< int >( DEC_PathWalker::eNotAllowed ) );
        return;
    }
    if( ( AvoidObstacles() && MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( obstacleId_ ) ) ||
        ( executionSuspended_ && pMainPath_->GetState() != DEC_Path_ABC::eComputing &&
        ( pMainPath_->GetCurrentKeyOnPath() == pMainPath_->GetResult().end() || pion_.GetRole< PHY_RoleInterface_Location >().GetPosition() != (*pMainPath_->GetCurrentKeyOnPath())->GetPos() ) ) )
    {
        // Recompute Pathfind in order to avoid obstacle or to get back previous path after suspension.
        CreateNewPath();
    }

    executionSuspended_ = false;
    isBlockedByObject_ = false;
    int nReturn = role_.Move( pMainPath_ );

    if( nReturn == DEC_PathWalker::eRunning )
    { // NOTHING. Pathfind is computing. Just don't try to do anything in this state.
    }
    else if( nReturn == DEC_PathWalker::eNotAllowed )
    { // NOTHING. Unit will not move at all.
    }
    else if( nReturn == DEC_PathWalker::eItineraireMustBeJoined )
    {
        CreateNewPath();
        nReturn = role_.Move( pMainPath_ );
    }
    else if( nReturn == DEC_PathWalker::ePartialPath )
    { // NOTHING. Unit will move to the nearest point of the target.
    }
    else if( nReturn == DEC_PathWalker::eBlockedByObject )
    { // Pathfind will be recomputed in a next tick once knowledge of the object has been obtained.
        isBlockedByObject_ = true;
    }
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::ExecuteSuspended
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionMove::ExecuteSuspended()
{
    if( pMainPath_.get() )
    {
        role_.MoveSuspended( pMainPath_ );
        executionSuspended_ = true;
    }
    Callback( static_cast< int >( DEC_PathWalker::ePaused ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionMove::StopAction()
{
    if( pMainPath_.get() )
    {
        role_.MoveCanceled( pMainPath_ );
        pMainPath_->DecRef();
        executionSuspended_ = false;
    }
    Callback( static_cast< int >( DEC_PathWalker::eFinished ) );
}
