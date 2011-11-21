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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/MIL_Army.h"
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
bool PHY_ActionMove::CreateJoiningPath( const MT_Vector2D& lastJoiningPoint, bool forceNextPoint )
{
    assert( pMainPath_.get() );
    assert( pMainPath_->GetState() != DEC_Path_ABC::eComputing );
    assert( !pJoiningPath_.get() );
    const MT_Vector2D& vPionPos = pion_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Vector2D& vTestPos = pMainPath_->GetPointOnPathCloseTo( vPionPos, lastJoiningPoint, forceNextPoint );
    if( vPionPos == vTestPos )
        return false;
    pJoiningPath_.reset( new DEC_Agent_Path( pion_, vTestPos, pMainPath_->GetPathType() ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pJoiningPath_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::DestroyJoiningPath
// Created: NLD 2004-09-29
// -----------------------------------------------------------------------------
MT_Vector2D PHY_ActionMove::DestroyJoiningPath()
{
    if( !pJoiningPath_.get() )
        return MT_Vector2D();
    role_.MoveCanceled( pJoiningPath_ );
    pJoiningPath_->Cancel();
    MT_Vector2D lastPoint = pJoiningPath_->GetResult().back()->GetPos();
    pJoiningPath_.reset();
    return lastPoint;
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

    boost::shared_ptr< DEC_Knowledge_Object > pObjectColliding;

    double rDistanceCollision = 0.;

    if( !role_.ComputeFutureObjectCollision( pion_.GetRole< PHY_RoleInterface_Location >().GetPosition(), objectsToAvoid_, rDistanceCollision, pObjectColliding ) )
        return;

    assert( pObjectColliding && pObjectColliding->IsValid() );
    const unsigned int nObjectToAvoidDiaID = pObjectColliding->GetID();
    // Le pion à déjà tenté d'éviter l'obstacle
    if( objectAvoidAttempts_.find( nObjectToAvoidDiaID ) != objectAvoidAttempts_.end() )
        return;
    objectAvoidAttempts_.insert( nObjectToAvoidDiaID );

    if( !isTreatingJoining_ )
    {
        if( pJoiningPath_.get() )
        {
            MT_Vector2D lastJoiningPoint = DestroyJoiningPath();
            if( !CreateJoiningPath( lastJoiningPoint, forceNextPoint_ ) )
                CreateFinalPath();
        }
        else
        {
            assert( pMainPath_.get() );
            boost::shared_ptr< DEC_Agent_Path > pNewMainPath( new DEC_Agent_Path( *pMainPath_ ) );
            MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pNewMainPath ); // $$$ à déplacer dans DEC_Agent_Path::Initialize()
            role_.MoveCanceled( pMainPath_ );
            pMainPath_->Cancel();
            pMainPath_ = pNewMainPath;
        }
    }
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
        role_.MoveSuspended( pCurrentPath );
        MT_Vector2D lastJoiningPoint = DestroyJoiningPath();
        if( CreateJoiningPath( lastJoiningPoint, forceNextPoint_ ) )
        {
            pCurrentPath = pJoiningPath_;
            nReturn = role_.Move( pCurrentPath );
        }
        else
            CreateFinalPath();
        forceNextPoint_ = false;
        isTreatingJoining_ = false;
    }
    else if( nReturn == DEC_PathWalker::eBlockedByObject )
    {
        isTreatingJoining_ = false;
        if( ( pCurrentPath != pJoiningPath_ && !pJoiningPath_.get() ) || pCurrentPath == pJoiningPath_ )
        {
            role_.MoveSuspended( pCurrentPath );
            DestroyJoiningPath();
            if( CreateJoiningPath( MT_Vector2D(), forceNextPoint_ ) )
            {
                pCurrentPath = pJoiningPath_;
                nReturn = role_.Move( pCurrentPath );
            }
            else
                CreateFinalPath();
            isTreatingJoining_ = true;
        }
        forceNextPoint_ = false;
    }
    else if( pCurrentPath == pJoiningPath_ )
    {
        forceNextPoint_ = false;
        isTreatingJoining_ = false;
        if( nReturn == DEC_PathWalker::eFinished )
        {
            DestroyJoiningPath();
            nReturn = DEC_PathWalker::eRunning;
        }
        else if( nReturn == DEC_PathWalker::ePartialPath )
        {
            DestroyJoiningPath();
            forceNextPoint_ = true;
        }
    }
    else
    {
        forceNextPoint_ = false;
        isTreatingJoining_ = false;
    }

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
