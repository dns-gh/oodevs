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
    , blockedByDisaster_( false )
    , blockedTickCounter_( 0 )
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
    ComputeNewKnowledges( knowledges );
    std::vector< TER_Localisation > signatures;
    for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
        signatures.push_back( (*it)->GetLocalisation() );
    if( knowledges == objectsToAvoid_ && signatures == geometrySignatures_ )
        return true;
    objectsToAvoid_.swap( knowledges );
    geometrySignatures_.swap( signatures );
    return false;
}

void PHY_ActionMove::ComputeNewKnowledges( T_KnowledgeObjectVector& newKnowledges )
{
    T_KnowledgeObjectVector knowledges;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = pion_.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsAtInteractionHeight( knowledges, pion_, MIL_PathObjectFilter() );
    bool disasterFound = false;
    for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
        if( ComputeNewKnowledge( *it, newKnowledges ) )
            disasterFound = true;
    if( !disasterFound )
    {
        oldDisasterImpact_ = 1;
        blockedByDisaster_ = false;
    }
}

bool PHY_ActionMove::ComputeNewKnowledge( const boost::shared_ptr< DEC_Knowledge_Object >& object, T_KnowledgeObjectVector& newKnowledges )
{
    const MIL_Object_ABC* obj = object->GetObjectKnown();
    const DisasterAttribute* disaster = obj ? obj->RetrieveAttribute< DisasterAttribute >() : 0;
    const auto& type = object->GetType();
    if( disaster && IsDisasterToAvoid( *disaster )
        || pMainPath_->GetPathClass().GetObjectCost( type )
        || type.GetCapacity< TrafficabilityCapacity >() )
        newKnowledges.push_back( object );
    return disaster != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::IsDisasterToAvoid
// Created: JSR 2014-05-14
// -----------------------------------------------------------------------------
bool PHY_ActionMove::IsDisasterToAvoid( const DisasterAttribute& disaster )
{
    const double disasterImpact = ComputeImpact( disaster );
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

double PHY_ActionMove::ComputeImpact( const DisasterAttribute& disaster ) const
{
    double latitude, longitude;
    TER_World::GetWorld().SimToMosMgrsCoord( pion_.GetRole< PHY_RoleInterface_Location >().GetPosition(), latitude, longitude );
    float maxValue = 0;
    const auto& extractors = disaster.GetExtractors();
    for( auto it = extractors.begin(); it != extractors.end(); ++it )
        maxValue = std::max( maxValue, (*it)->GetValue( longitude, latitude ) );
    DisasterImpactComputer computer( maxValue );
    pion_.Execute< OnComponentComputer_ABC >( computer );
    return computer.GetModifier();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::AvoidObstacles
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
bool PHY_ActionMove::AvoidObstacles()
{
    const bool avoid = pMainPath_->GetPathClass().AvoidObjects();
    if( UpdateObjectsToAvoid() && avoid )
    {
        // We're blocked even if no change in the known collision objects.
        if( isBlockedByObject_ )
            ++blockedTickCounter_;
        if( blockedTickCounter_ < 3 )
            // Do not recompute a path just yet, wait for maybe acquiring or
            // loosing the knowledge of the object.
            // In the latter case it probably means the object is gone.
            return false;
    }
    blockedTickCounter_ = 0;
    const auto collision = UpdateCollision();
    if( avoid && collision )
    {
        role_.SendRC( report::eRC_DifficultMovementProgression, collision->GetType().GetRealName() );
        return true;
    }
    return false;
}

boost::shared_ptr< DEC_Knowledge_Object > PHY_ActionMove::UpdateCollision()
{
    for( auto it = objectsToAvoid_.begin(); it != objectsToAvoid_.end(); ++it )
    {
        const MIL_Object_ABC* obj = (*it)->GetObjectKnown();
        if( obj && obj->RetrieveAttribute< DisasterAttribute>() )
            return *it;
    }
    double distance = 0;
    boost::shared_ptr< DEC_Knowledge_Object > collision;
    role_.ComputeFutureObjectCollision( objectsToAvoid_, distance, collision, pion_, isBlockedByObject_, true );
    // For some reason when the object is destroyed the knowledge remains
    if( collision && !collision->GetObjectKnown() )
        collision.reset();
    static const double threshold = TER_World::GetWorld().GetWeldValue();
    if( !pMainPath_->GetPathClass().AvoidObjects() &&
        ( distance <= threshold || isBlockedByObject_ ) )
    {
        if( collision && collision != blocking_.lock() )
            MIL_Report::PostEvent( pion_, report::eRC_BlockedByObject, collision );
        blocking_ = collision;
    }
    return collision;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::CreateNewPath
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
void PHY_ActionMove::CreateNewPath()
{
    assert( pMainPath_ );
    assert( pMainPath_->GetState() != DEC_Path_ABC::eComputing );
    T_PointVector nextWaypoints = pMainPath_->GetNextWaypoints();
    nextWaypoints.insert( nextWaypoints.begin(), pion_.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    const DEC_PathType& pathType = pMainPath_->GetPathType();
    const auto computer = boost::make_shared< DEC_PathComputer >( pion_.GetID() );
    const auto pNewPath = boost::make_shared< DEC_Agent_Path >( pion_, nextWaypoints, pathType, computer );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( computer, sword::Pathfind() );
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
        if( blocking_.lock() )
            CreateNewPath();
        blocking_.reset();
        Callback< int >( DEC_PathWalker::eTeleported );
        return;
    }
    if( !pion_.GetRole< PHY_RoleInterface_Deployment >().IsUndeployed() )
        return;
    if( !pMainPath_ )
    {
        Callback< int >( DEC_PathWalker::eNotAllowed );
        return;
    }
    if( pMainPath_->GetState() != DEC_Path_ABC::eComputing
        && ( AvoidObstacles()
            || executionSuspended_
                && ( pMainPath_->GetCurrentKeyOnPath() == pMainPath_->GetResult().end()
                    || pion_.GetRole< PHY_RoleInterface_Location >().GetPosition() != (*pMainPath_->GetCurrentKeyOnPath())->GetPos() ) ) )
    {
        // Recompute Pathfind in order to avoid obstacle or to get back previous path after suspension.
        CreateNewPath();
    }
    if( blocking_.lock() )
    {
        Callback< int >( DEC_PathWalker::eRunning );
        return;
    }
    executionSuspended_ = false;
    isBlockedByObject_ = false;
    int nReturn = role_.Move( pMainPath_ );
    if( nReturn == DEC_PathWalker::eItineraireMustBeJoined )
    {
        CreateNewPath();
        nReturn = role_.Move( pMainPath_ );
    }
    else if( nReturn == DEC_PathWalker::eBlockedByObject )
    {
        // Pathfind will be recomputed in a next tick once knowledge of the object has been obtained.
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
    if( pMainPath_ )
    {
        role_.MoveSuspended( pMainPath_ );
        executionSuspended_ = true;
    }
    Callback< int >( DEC_PathWalker::ePaused );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionMove::StopAction()
{
    if( pMainPath_ )
    {
        role_.MoveCanceled( pMainPath_ );
        role_.NotifyCurrentPathChanged();
        executionSuspended_ = false;
    }
    Callback< int >( DEC_PathWalker::eFinished );
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
