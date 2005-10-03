// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_PathFunctions.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 17:14 $
// $Revision: 10 $
// $Workfile: DEC_PathFunctions.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_PathFunctions.h"

#include "Decision/Path/DEC_PathType.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Decision/Path/Agent/DEC_Agent_Path.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObjectTypeFilter.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "DEC_FunctionsTools.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPoint
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_PathFunctions::CreatePathToPoint( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    MT_Vector2D* pEnd = call.GetParameter( 0 ).ToUserPtr( pEnd   );

    DEC_PathType pathType( (DEC_PathType::E_PathType)call.GetParameter( 1 ).ToId() );

    assert( pEnd );
    assert( !_isnan( pEnd->rX_ ) );
    assert( !_isnan( pEnd->rY_ ) );

    DEC_Agent_Path* pPath = new DEC_Agent_Path( callerAgent, *pEnd, pathType );
    pPath->IncDIARef();  
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( *pPath );
    call.GetResult().SetValue( pPath, &DEC_Tools::GetTypeItineraire() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPointList
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_PathFunctions::CreatePathToPointList( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeListePoints( call.GetParameter( 0 ) ) );

    T_PointVector* pListPt = call.GetParameter( 0 ).ToUserPtr( pListPt );

    DEC_PathType pathType( (DEC_PathType::E_PathType)call.GetParameter( 1 ).ToId() );

    assert( pListPt );
    assert( !pListPt->empty() );

    DEC_Agent_Path* pPath = new DEC_Agent_Path( callerAgent, *pListPt, pathType );
    pPath->IncDIARef();  
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( *pPath );
    call.GetResult().SetValue( pPath, &DEC_Tools::GetTypeItineraire() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::DeletePath
// Created: AGN 02-12-19
//-----------------------------------------------------------------------------
void DEC_PathFunctions::DeletePath( DIA_Call_ABC& call, MIL_AgentPion& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeItineraire( call.GetParameter( 0 ) ) );
    DEC_Agent_Path* pPath = call.GetParameter( 0 ).ToUserPtr( pPath );
    assert( pPath );
    pPath->Cancel();
    pPath->DecDIARef();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetPathState
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetPathState( DIA_Call_ABC& call, MIL_AgentPion& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeItineraire( call.GetParameter( 0 ) ) );
    DEC_Agent_Path* pPath = call.GetParameter( 0 ).ToUserPtr( pPath );
    assert( pPath );
    call.GetResult().SetValue( pPath->GetState() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::ExtrapolatePosition
// Created: NLD 2004-05-13
// -----------------------------------------------------------------------------
void DEC_PathFunctions::ExtrapolatePosition( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    MT_Float        rTime        = MIL_Tools::ConvertMinutesToSim( call.GetParameter( 0 ).ToFloat() );
    bool            bBoundOnPath = call.GetParameter( 1 ).ToBool();

    MT_Vector2D*    pPos   = new MT_Vector2D( callerAgent.GetRole< PHY_RoleAction_Moving >().ExtrapolatePosition( rTime, bBoundOnPath ) ); // $$$$ RAM
    call.GetResult().SetValue( (void*)pPos, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetNextObjectOnPath
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetNextObjectOnPath( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    MIL_RealObjectTypeFilter objectsFilter( call.GetParameters(), 2 );

    const DEC_Knowledge_Object* pObjectColliding   = 0;
          MT_Float              rDistanceCollision = 0.;
    
    if( !callerAgent.GetRole< PHY_RoleAction_Moving >().ComputeFutureObjectCollisions( objectsFilter, rDistanceCollision, &pObjectColliding ) )
    {
        call.GetResult().SetValue( false );
        return;
    }
    assert( pObjectColliding );

    call.GetParameter( 0 ).SetValue( (void*)pObjectColliding->GetDiaID(), &DEC_Tools::GetTypeConnaissanceObjet() );
    call.GetParameter( 1 ).SetValue( (float)rDistanceCollision );
    call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetNextObjectOnPionPath
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetNextObjectOnPionPath( DIA_Call_ABC& call, const MIL_Automate& callerAutomate )
{
    assert( DEC_Tools::CheckTypePion( call.GetParameter( 0 ) ) );

    DEC_RolePion_Decision* pPion = call.GetParameter( 0 ).ToUserObject( pPion );
    
    assert( pPion );
    assert( std::find( callerAutomate.GetPions().begin(), callerAutomate.GetPions().end(), &pPion->GetPion() ) != callerAutomate.GetPions().end() );

    MIL_RealObjectTypeFilter objectsFilter( call.GetParameters(), 3 );
    
    const DEC_Knowledge_Object* pObjectColliding   = 0;
          MT_Float              rDistanceCollision = 0.;
    if( !pPion->GetPion().GetRole< PHY_RoleAction_Moving >().ComputeFutureObjectCollisions( objectsFilter, rDistanceCollision, &pObjectColliding ) )
    {
        call.GetResult().SetValue( false );
        return;
    }
    assert( pObjectColliding );
    
    call.GetParameter( 1 ).SetValue( (void*)pObjectColliding->GetDiaID(), &DEC_Tools::GetTypeConnaissanceObjet() );
    call.GetParameter( 2 ).SetValue( (float)rDistanceCollision );
    call.GetResult().SetValue( true );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLastPointOfPath
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetLastPointOfPath( DIA_Call_ABC& call, const MIL_AgentPion& /*callerAgent*/ )
{
    assert( DEC_Tools::CheckTypeItineraire( call.GetParameter( 0 ) ) );

    DEC_Agent_Path* pPath = call.GetParameter( 0 ).ToUserPtr( pPath );
    assert( pPath );

    MT_Vector2D* pPos = new MT_Vector2D( pPath->GetResult().back()->GetPos() );
    call.GetResult().SetValue( (void*)pPos, &DEC_Tools::GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsMovingOnPath
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
void DEC_PathFunctions::IsMovingOnPath( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypeItineraire( call.GetParameter( 0 ) ) );

    DEC_Agent_Path* pPath = call.GetParameter( 0 ).ToUserPtr( pPath );
    
    call.GetResult().SetValue( pPath ? callerAgent.GetRole< PHY_RoleAction_Moving >().IsMovingOn( *pPath ) : false );    
}

