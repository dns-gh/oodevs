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
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_PathWalker.h"
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_PathPoint.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/AttritionCapacity.h"
#include "Entities/Objects/ContaminationCapacity.h"
#include "Entities/Objects/InterferenceCapacity.h"
#include "Entities/Objects/AvoidanceCapacity.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMove::PHY_ActionMove( MIL_AgentPion& pion, boost::shared_ptr< DEC_Path_ABC > pPath )
    : PHY_DecisionCallbackAction_ABC( pion )
    , pion_                         ( pion )
    , role_                         ( pion.GetRole< moving::PHY_RoleAction_Moving >() )
    , pMainPath_                    ( boost::dynamic_pointer_cast< DEC_Agent_Path >( pPath ) )
    , forceNextPoint_               ( false )
    , isTreatingJoining_            ( false )
{
    // NOTHING
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
// Name: PHY_ActionMove::StopAction
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
void PHY_ActionMove::StopAction()
{
    Callback( static_cast< int >( DEC_PathWalker::eFinished ) );

    DestroyJoiningPath();
    if( pMainPath_.get() )
        role_.MoveCanceled( pMainPath_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::CreateJoiningPath
// Created: NLD 2004-09-29
// -----------------------------------------------------------------------------
bool PHY_ActionMove::CreateJoiningPath( const MT_Vector2D& lastJoiningPoint, bool forceNextPoint, double minDistance )
{
    assert( pMainPath_.get() );
    assert( pMainPath_->GetState() != DEC_Path_ABC::eComputing );
    assert( !pJoiningPath_.get() );
    const MT_Vector2D& vPionPos = pion_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Vector2D& vTestPos = pMainPath_->GetPointOnPathCloseTo( vPionPos, lastJoiningPoint, forceNextPoint, minDistance );
    if( vPionPos == vTestPos )
        return false;
    pJoiningPath_.reset( new DEC_Agent_Path( pion_, vTestPos, pMainPath_->GetPathType() ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pJoiningPath_ );
    isTreatingJoining_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::DestroyJoiningPath
// Created: NLD 2004-09-29
// -----------------------------------------------------------------------------
void PHY_ActionMove::DestroyJoiningPath()
{
    if( !pJoiningPath_.get() )
        return;
    role_.MoveCanceled( pJoiningPath_ );
    pJoiningPath_->Cancel();
    pJoiningPath_.reset();
    isTreatingJoining_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::GetLastPointAndDestroyJoiningPath
// Created: CMA 2011-12-08
// -----------------------------------------------------------------------------
MT_Vector2D PHY_ActionMove::GetLastPointAndDestroyJoiningPath()
{
    if( !pJoiningPath_.get() )
        return MT_Vector2D();
    role_.MoveCanceled( pJoiningPath_ );
    pJoiningPath_->Cancel();
    const DEC_PathResult::T_PathPointList& pathResult = pJoiningPath_->GetResult( false );
    if( pathResult.empty() )
    {
        pJoiningPath_.reset();
        isTreatingJoining_ = false;
        return MT_Vector2D();
    }
    boost::shared_ptr< DEC_PathPoint > lastPoint = pathResult.back();
    MT_Vector2D point = lastPoint.get() ? lastPoint->GetPos() : MT_Vector2D();
    pJoiningPath_.reset();
    isTreatingJoining_ = false;
    return point;
}

namespace
{
    class MIL_DangerousObjectFilter : public MIL_ObjectFilter
    {
        virtual bool Test( const MIL_ObjectType_ABC& type ) const
        {
            return type.GetCapacity< AttritionCapacity >() != 0 ||
                   type.GetCapacity< ContaminationCapacity >() != 0 ||
                   type.GetCapacity< InterferenceCapacity >() != 0 ||
                   type.GetCapacity< AvoidanceCapacity >() != 0;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::UpdateObjectsToAvoid
// Created: NLD 2006-04-28
// -----------------------------------------------------------------------------
bool PHY_ActionMove::UpdateObjectsToAvoid()
{
    T_KnowledgeObjectVector knowledges;

    const double rHeight = pion_.GetRole< PHY_RoleInterface_Location >().GetHeight();

    MIL_DangerousObjectFilter filter;
    pion_.GetArmy().GetKnowledge().GetObjectsAtInteractionHeight( knowledges, rHeight, filter );
    if( knowledges != objectsToAvoid_ )
    {
        objectsToAvoid_ = knowledges;
        geometrySignatures_.clear();
        for( T_KnowledgeObjectVector::const_iterator it = knowledges.begin(); it != knowledges.end(); ++it )
            geometrySignatures_.push_back( (*it)->GetLocalisation() );
        return true;
    }
    else
    {
        bool modified = false;
        for( int i = 0; i < knowledges.size(); ++i )
        {
            if( geometrySignatures_[i] != knowledges[i]->GetLocalisation() )
            {
                geometrySignatures_[i] = knowledges[i]->GetLocalisation();
                objectAvoidAttempts_.erase( knowledges[i]->GetID() );
                modified = true;
            }
        }
        if( modified )
        {
            objectsToAvoid_ = knowledges;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::AvoidObstacles
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
void PHY_ActionMove::AvoidObstacles()
{
    if( !UpdateObjectsToAvoid() )
        return;

    double rDistanceBeforeCollision = 0.;
    double rDistanceAfterCollision = 0.;

    boost::shared_ptr< DEC_Knowledge_Object > obstacle;
    if( !role_.ComputeFutureObjectCollision( pion_.GetRole< PHY_RoleInterface_Location >().GetPosition(), objectsToAvoid_, rDistanceBeforeCollision, rDistanceAfterCollision, obstacle ) )
        return;

    assert( obstacle && obstacle->IsValid() );
    const unsigned int nObjectToAvoidDiaID = obstacle->GetID();
    // Le pion � d�j� tent� d'�viter l'obstacle - $$$$ RC LDC Si l'obstacle croise 2 branches de pathfind, c'est tres douteux comme optimisation
    if( objectAvoidAttempts_.find( nObjectToAvoidDiaID ) != objectAvoidAttempts_.end() )
        return;
    objectAvoidAttempts_.insert( nObjectToAvoidDiaID );

    if( !isTreatingJoining_ )
    {
        DestroyJoiningPath();
        if( !CreateJoiningPath( MT_Vector2D(), forceNextPoint_, rDistanceAfterCollision ) )
            CreateFinalPath();
        role_.SendRC( MIL_Report::eReport_DifficultTerrain );
    }
    return;
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

    AvoidObstacles();

    boost::shared_ptr< DEC_PathResult > pCurrentPath( pJoiningPath_.get() ? pJoiningPath_ : pMainPath_ );
    int nReturn = role_.Move( pCurrentPath );

    if( nReturn == DEC_PathWalker::eItineraireMustBeJoined )
    {
        MT_Vector2D lastJoiningPoint = GetLastPointAndDestroyJoiningPath();
        nReturn = CreateAdaptedPath( pCurrentPath, lastJoiningPoint, forceNextPoint_ );
        forceNextPoint_ = false;
    }
    else if( nReturn == DEC_PathWalker::eBlockedByObject )
    {
        if( ( pCurrentPath != pJoiningPath_ && !pJoiningPath_.get() ) || pCurrentPath == pJoiningPath_ )
        {
            const MT_Vector2D& vPionPos = pion_.GetRole< PHY_RoleInterface_Location >().GetPosition();
            MT_Vector2D lastJoiningPoint = GetLastPointAndDestroyJoiningPath();
            if( vPionPos == lastBlockedPoint_.first.first && lastJoiningPoint == lastBlockedPoint_.first.second )
            {
                if( lastBlockedPoint_.second > 1 )
                    forceNextPoint_ = true;
                lastBlockedPoint_.second++;
            }
            else
                lastBlockedPoint_ = std::make_pair( std::make_pair( vPionPos, lastJoiningPoint ), 1 );
            obstacle_ = role_.GetCurrentObstacle();
            nReturn = CreatePathAfterObjectCollision( pCurrentPath, obstacle_ );
        }
        forceNextPoint_ = false;
    }
    else if( pCurrentPath == pJoiningPath_ )
    {
        forceNextPoint_ = false;
        if( nReturn == DEC_PathWalker::eFinished )
        {
            DestroyJoiningPath();
            nReturn = DEC_PathWalker::eRunning;
        }
        else if( nReturn == DEC_PathWalker::ePartialPath )
        {
            forceNextPoint_ = true;
            MT_Vector2D lastJoiningPoint = GetLastPointAndDestroyJoiningPath();
            nReturn = CreateAdaptedPath( pCurrentPath, lastJoiningPoint, forceNextPoint_ );
        }
    }
    else
        forceNextPoint_ = false;

    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::ExecuteSuspended
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionMove::ExecuteSuspended()
{
    boost::shared_ptr< DEC_Agent_Path > pCurrentPath = pJoiningPath_.get() ? pJoiningPath_ : pMainPath_;
    if( pCurrentPath )
        role_.MoveSuspended( pCurrentPath );
    Callback( static_cast< int >( DEC_PathWalker::ePaused ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::CreateFinalPath
// Bypassd: CMA 2011-11-21
// -----------------------------------------------------------------------------
void PHY_ActionMove::CreateFinalPath()
{
    assert( pMainPath_.get() );
    const MT_Vector2D& vPionPos = pion_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    boost::shared_ptr< DEC_Agent_Path > pNewMainPath( new DEC_Agent_Path( pion_, vPionPos, pMainPath_->GetPathType() ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pNewMainPath );
    role_.MoveCanceled( pMainPath_ );
    pMainPath_->Cancel();
    pMainPath_ = pNewMainPath;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::CreatePathAfterObjectCollision
// Bypassd: LDC 2012-01-12
// -----------------------------------------------------------------------------
int PHY_ActionMove::CreatePathAfterObjectCollision( boost::shared_ptr< DEC_PathResult > pCurrentPath, MIL_Object_ABC* obstacle )
{
    role_.MoveSuspended( pCurrentPath );
    const MT_Vector2D& vPionPos = pion_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Vector2D& vTestPos = pMainPath_->GetNextPointOutsideObstacle( vPionPos, obstacle );
    if( vPionPos != vTestPos )
    {
        pJoiningPath_.reset( new DEC_Agent_Path( pion_, vTestPos, pMainPath_->GetPathType() ) );
        MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pJoiningPath_ );
        isTreatingJoining_ = true;
        pCurrentPath = pJoiningPath_;
        return role_.Move( pCurrentPath );
    }
    CreateFinalPath();
    return DEC_PathWalker::eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::CreatePathAfterObjectCollision
// Bypassd: CMA 2011-11-22
// -----------------------------------------------------------------------------
int PHY_ActionMove::CreateAdaptedPath( boost::shared_ptr< DEC_PathResult > pCurrentPath, const MT_Vector2D& lastJoiningPoint, bool forceNextPoint )
{
    role_.MoveSuspended( pCurrentPath );
    if( CreateJoiningPath( lastJoiningPoint, forceNextPoint, 0. ) )
    {
        pCurrentPath = pJoiningPath_;
        return role_.Move( pCurrentPath );
    }
    CreateFinalPath();
    return DEC_PathWalker::eRunning;
}