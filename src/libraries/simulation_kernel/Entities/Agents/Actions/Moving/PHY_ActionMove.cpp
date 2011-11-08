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
    , pion_     ( pion )
    , role_     ( pion.GetRole< moving::PHY_RoleAction_Moving >() )
    , pMainPath_( boost::dynamic_pointer_cast< DEC_Agent_Path >( pPath ) )
{
    if( pMainPath_.get() )
        Callback( static_cast< int >( DEC_PathWalker::eRunning ) );
    else
        Callback( static_cast< int >( DEC_PathWalker::eNotAllowed ) );
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
void PHY_ActionMove::CreateJoiningPath()
{
    assert( pMainPath_.get() );
    assert( pMainPath_->GetState() != DEC_Path_ABC::eComputing );
    assert( !pJoiningPath_.get() );
    const MT_Vector2D& vPionPos = pion_.GetRole< PHY_RoleInterface_Location >().GetPosition();
    pJoiningPath_.reset( new DEC_Agent_Path( pion_, pMainPath_->GetPointOnPathCloseTo( vPionPos ), pMainPath_->GetPathType() ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pJoiningPath_ );
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

    if( pJoiningPath_.get() )
    {
        DestroyJoiningPath();
        CreateJoiningPath ();
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
        DestroyJoiningPath();
        CreateJoiningPath ();
        pCurrentPath = pJoiningPath_;
        nReturn      = role_.Move( pCurrentPath );
    }

    if( pCurrentPath == pJoiningPath_ && nReturn == DEC_PathWalker::eFinished )
    {
        DestroyJoiningPath();
        nReturn      = DEC_PathWalker::eRunning;
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
