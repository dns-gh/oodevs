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
#include "DisasterImpactComputer.h"
#include "MIL_AgentServer.h"
#include "PHY_RoleAction_Moving.h"
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_Agent_PathClass.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_PathComputer.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_PathWalker.h"
#include "Decision/DEC_Agent_PathClass.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_PathWalker.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/DisasterAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/TrafficabilityCapacity.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "propagation/Extractor_ABC.h"
#include "simulation_terrain/TER_World.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMove::PHY_ActionMove( MIL_AgentPion& pion, boost::shared_ptr< DEC_Path_ABC > pPath, bool suspended )
    : PHY_DecisionCallbackAction_ABC( pion )
    , pion_( pion )
    , role_( pion.GetRole< moving::PHY_RoleAction_Moving >() )
    , pMainPath_( boost::dynamic_pointer_cast< DEC_Agent_Path >( pPath ) )
    , executionSuspended_( false )
    , isBlockedByObject_( false )
    , blockedTickCounter_( 0 )
    , obstacleId_( 0 )
    , blockedByDisaster_( false )
    , oldDisasterImpact_( 1 )
{
    if( suspended )
        Suspend();
    if( pMainPath_->GetState() == DEC_Path_ABC::eCanceled )
    {
        pion.GetRole< moving::PHY_RoleAction_Moving >().SendRC( report::eRC_TerrainDifficile );
        CreateNewPath();
    }
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
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = pion_.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsAtInteractionHeight( knowledges, pion_, filter );
    bool disasterFound = false;
    for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
    {
        const MIL_Object_ABC* obj = ( *it )->GetObjectKnown();
        const DisasterAttribute* disaster = obj ? obj->RetrieveAttribute< DisasterAttribute >() : 0;
        if( disaster )
        {
            disasterFound = true;
            if( IsDisasterToAvoid( *disaster ) )
                newKnowledges.push_back( *it );
        }
        else
        {
            const MIL_ObjectType_ABC& type = (*it)->GetType();
            double cost = pMainPath_->GetPathClass().GetObjectCost( type );
            if( 0. != cost || type.GetCapacity< TrafficabilityCapacity >() )
                newKnowledges.push_back( *it );
        }
    }
    if( !disasterFound )
    {
        oldDisasterImpact_ = 1;
        blockedByDisaster_ = false;
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
        for( std::size_t i = 0; i < newKnowledges.size(); ++i )
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
// Name: PHY_ActionMove::IsDisasterToAvoid
// Created: JSR 2014-05-14
// -----------------------------------------------------------------------------
bool PHY_ActionMove::IsDisasterToAvoid( const DisasterAttribute& disaster )
{
    double latitude, longitude;
    TER_World::GetWorld().SimToMosMgrsCoord( pion_.GetRole< PHY_RoleInterface_Location >().GetPosition(), latitude, longitude );
    float maxValue = 0;
    const auto& extractors = disaster.GetExtractors();
    for( auto it = extractors.begin(); it != extractors.end(); ++it )
        maxValue = std::max( maxValue, ( *it )->GetValue( longitude, latitude ) );
    DisasterImpactComputer computer( maxValue );
    pion_.Execute< OnComponentComputer_ABC >( computer );
    const double disasterImpact = computer.GetModifier();
    bool ret = false;
    if( disasterImpact > oldDisasterImpact_ )
        blockedByDisaster_ = false;
    else if( disasterImpact <= 0.1 && !blockedByDisaster_ )
    {
        ret = true;
        blockedByDisaster_ = true;
    }
    oldDisasterImpact_ = disasterImpact;
    return ret;
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
    for( auto it = objectsToAvoid_.begin(); it != objectsToAvoid_.end() && !pObjectColliding; ++it )
    {
        const MIL_Object_ABC* obj = ( *it )->GetObjectKnown();
        if( obj && obj->RetrieveAttribute< DisasterAttribute>() )
            pObjectColliding = *it;
    }
    double rDistanceCollision = 0.;
    if( !pObjectColliding && !role_.ComputeFutureObjectCollision( objectsToAvoid_, rDistanceCollision, pObjectColliding, pion_, isBlockedByObject_, true ) )
        return false;

    assert( pObjectColliding && pObjectColliding->IsValid() );
    obstacleId_ = pObjectColliding->GetObjectKnown() ? pObjectColliding->GetObjectKnown()->GetID() : 0;

    if( pMainPath_->GetPathClass().AvoidObjects() )
        role_.SendRC( report::eRC_DifficultMovementProgression, pObjectColliding->GetType().GetRealName() );
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
    T_PointVector nextWaypoints = pMainPath_->GetNextWaypoints();
    nextWaypoints.insert( nextWaypoints.begin(), pion_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    const DEC_PathType& pathType = pMainPath_->GetPathType();
    const auto computer = boost::make_shared< DEC_PathComputer >( pion_.GetID() );
    const auto pNewPath = boost::make_shared< DEC_Agent_Path >( pion_, nextWaypoints, pathType, computer );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( computer, false );
    role_.MoveCanceled( pMainPath_ );
    pMainPath_->Cancel();
    pMainPath_ = pNewPath;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::Execute
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_ActionMove::Execute()
{
    if( pion_.HasBeenTeleported() )
    {
        Callback( static_cast< int >( DEC_PathWalker::eTeleported) );
        return;
    }
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
        role_.NotifyCurrentPathChanged();
        executionSuspended_ = false;
    }
    Callback( static_cast< int >( DEC_PathWalker::eFinished ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::GetState
// Created: LGY 2013-07-11
// -----------------------------------------------------------------------------
PHY_ActionMove::E_State PHY_ActionMove::GetState() const
{
    if( pMainPath_->GetState() == DEC_Path_ABC::eComputing )
        return eRunning;
    if( !role_.HasCurrentPath() )
        return eRunning;
    return eDone;
}
