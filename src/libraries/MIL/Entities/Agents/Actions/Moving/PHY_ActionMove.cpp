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

#include "MIL_pch.h"

#include "PHY_ActionMove.h"

#include "PHY_RoleAction_Moving.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Decision/Path/DEC_Path.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Object.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove constructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMove::PHY_ActionMove( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_Action_ABC    ( pion, diaCall )
    , role_              ( pion.GetRole< PHY_RoleAction_Moving >() )
    , diaReturnCode_     ( diaCall.GetParameter( 0 ) )
    , pMainPath_         ( diaCall.GetParameter( 1 ).ToUserPtr( pMainPath_ ) )
    , pJoiningPath_      ( 0 )
    , objectAvoidAttempts_()
{    
    assert( DEC_Tools::CheckTypeItineraire( diaCall.GetParameter( 1 ) ) );

    if( pMainPath_ )
    {
        diaReturnCode_.SetValue( PHY_RoleAction_Moving::eRunning );
        pMainPath_->IncRef();
    }
    else
        diaReturnCode_.SetValue( PHY_RoleAction_Moving::eNotAllowed );        
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove destructor
// Bypassd: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionMove::~PHY_ActionMove()
{
    diaReturnCode_.SetValue( PHY_RoleAction_Moving::eFinished );
    
    DestroyJoiningPath();
    if( pMainPath_ )
    {
        role_.MoveCanceled( *pMainPath_ );
        pMainPath_->DecRef();
    }
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::CreateJoiningPath
// Created: NLD 2004-09-29
// -----------------------------------------------------------------------------
inline
void PHY_ActionMove::CreateJoiningPath()
{
    assert( pMainPath_ );
    assert( pMainPath_->GetState() != DEC_Path::eComputing );
    assert( !pJoiningPath_ );
    const MT_Vector2D& vPionPos = pion_.GetRole< PHY_RolePion_Location >().GetPosition();
    pJoiningPath_ = new DEC_Path( pion_, pMainPath_->GetPointOnPathCloseTo( vPionPos ), pMainPath_->GetPathType() );
    pJoiningPath_->IncRef();
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( *pJoiningPath_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::DestroyJoiningPath
// Created: NLD 2004-09-29
// -----------------------------------------------------------------------------
inline
void PHY_ActionMove::DestroyJoiningPath()
{
    if( !pJoiningPath_ )
        return;
    role_.MoveCanceled( *pJoiningPath_ );
    pJoiningPath_->Cancel();
    pJoiningPath_->DecRef();
    pJoiningPath_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::AvoidObstacles
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
void PHY_ActionMove::AvoidObstacles()
{
    DEC_Path::T_KnowledgeObjectMultimap objectsOnPathMap;
    role_.ComputeFutureObjectCollisions( MIL_RealObjectType::GetObjectTypesToAvoid(), objectsOnPathMap );
    if( objectsOnPathMap.empty() )
        return;

    const uint nObjectToAvoidDiaID = objectsOnPathMap.begin()->second->GetDiaID();
    // Le pion à déjà tenté d'éviter l'obstacle
    if( objectAvoidAttempts_.find( nObjectToAvoidDiaID ) != objectAvoidAttempts_.end() )
        return; 
    objectAvoidAttempts_.insert( nObjectToAvoidDiaID );

    if( pJoiningPath_ )
    {
        DestroyJoiningPath();
        CreateJoiningPath ();
    }
    else
    {
        DEC_Path* pNewMainPath = new DEC_Path( *pMainPath_ );
        pNewMainPath->IncRef();
        MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( *pNewMainPath ); // $$$ à déplacer dans DEC_Path::Initialize()

        role_.MoveCanceled( *pMainPath_ );
        pMainPath_->Cancel();
        pMainPath_->DecRef();
        
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
    if( !pMainPath_ )
    {
        diaReturnCode_.SetValue( PHY_RoleAction_Moving::eNotAllowed );
        return;
    }

    DEC_Path* pCurrentPath = pJoiningPath_ ? pJoiningPath_ : pMainPath_;
    int nReturn = role_.Move( *pCurrentPath );

    if( nReturn == PHY_RoleAction_Moving::eItineraireMustBeJoined )
    {
        role_.MoveSuspended( *pCurrentPath );
        DestroyJoiningPath();
            CreateJoiningPath();
        pCurrentPath = pJoiningPath_;
        nReturn      = role_.Move( *pCurrentPath );
    }

    if( pCurrentPath == pJoiningPath_ && nReturn == PHY_RoleAction_Moving::eFinished )
    {
        DestroyJoiningPath();
        pCurrentPath = pMainPath_;
        nReturn      = PHY_RoleAction_Moving::eRunning;
    }

    AvoidObstacles();
    diaReturnCode_.SetValue( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionMove::ExecuteSuspended
// Bypassd: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_ActionMove::ExecuteSuspended()
{
    DEC_Path* pCurrentPath = pJoiningPath_ ? pJoiningPath_ : pMainPath_;
    if( pCurrentPath )
        role_.MoveSuspended( *pCurrentPath );
    diaReturnCode_.SetValue( PHY_RoleAction_Moving::ePaused );
}
