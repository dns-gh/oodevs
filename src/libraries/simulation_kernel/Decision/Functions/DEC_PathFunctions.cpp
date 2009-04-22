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

#include "simulation_kernel_pch.h"
#include "DEC_PathFunctions.h"

#include "Decision/Path/DEC_PathType.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Decision/Path/Agent/DEC_Agent_Path.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_FunctionsTools.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Front.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Lima.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPoint
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_PathFunctions::CreatePathToPoint( DIA_Call_ABC& call, MIL_AgentPion& callerAgent )
{
    assert( DEC_Tools::CheckTypePoint( call.GetParameter( 0 ) ) );

    MT_Vector2D* pEnd = call.GetParameter( 0 ).ToUserPtr( pEnd   );
    assert( pEnd );

    const DEC_PathType* pPathType = DEC_PathType::Find( call.GetParameter( 1 ).ToId() );
    assert( pPathType );

    DEC_Agent_Path* pPath = new DEC_Agent_Path( callerAgent, *pEnd, *pPathType );
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

    assert( pListPt );
    assert( !pListPt->empty() );

    const DEC_PathType* pPathType = DEC_PathType::Find( call.GetParameter( 1 ).ToId() );
    assert( pPathType );

    DEC_Agent_Path* pPath = new DEC_Agent_Path( callerAgent, *pListPt, *pPathType );
    pPath->IncDIARef();  
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( *pPath );
    call.GetResult().SetValue( pPath, &DEC_Tools::GetTypeItineraire() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_ABC::DeletePath
// Created: AGN 02-12-19
//-----------------------------------------------------------------------------
void DEC_PathFunctions::DeletePath( DIA_Call_ABC& /*call*/, MIL_AgentPion& /*callerAgent*/ )
{
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
    MIL_ObjectFilter filter( call.GetParameters(), 2 );

    const DEC_Knowledge_Object* pObjectColliding   = 0;
          MT_Float              rDistanceCollision = 0.;

    const PHY_RolePion_Location& roleLocation = callerAgent.GetRole< PHY_RolePion_Location >();
    const MT_Float               rHeight      = roleLocation.GetHeight  ();
    const MT_Vector2D&           position     = roleLocation.GetPosition();

    T_KnowledgeObjectVector knowledges;
    callerAgent.GetArmy().GetKnowledge().GetObjectsAtInteractionHeight( knowledges, rHeight, filter );

    if( knowledges.empty() || !callerAgent.GetRole< PHY_RoleAction_Moving >().ComputeFutureObjectCollision( position, knowledges, rDistanceCollision, &pObjectColliding ) )
    {
        call.GetResult().SetValue( false );
        return;
    }
    assert( pObjectColliding );

    call.GetParameter( 0 ).SetValue( (void*)pObjectColliding->GetID(), &DEC_Tools::GetTypeConnaissanceObjet() );
    call.GetParameter( 1 ).SetValue( (float)MIL_Tools::ConvertSimToMeter( rDistanceCollision ) );
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

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetRepPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetRepPoint( DIA_Call_ABC& call )
{
    DEC_PathPoint* pPoint = dynamic_cast< DEC_PathPoint* >( call.GetParameter( 0 ).ToObject() );
    call.GetResult().SetValue( (void*)&pPoint->GetPos(), &DEC_Tools::GetTypePoint(), 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsAvantPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void DEC_PathFunctions::IsAvantPoint( DIA_Call_ABC& call )
{
    DEC_Rep_PathPoint_Front* pFront = dynamic_cast< DEC_Rep_PathPoint_Front* >( call.GetParameter( 0 ).ToObject() );
    call.GetResult().SetValue( pFront ? true : false );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void DEC_PathFunctions::IsPoint( DIA_Call_ABC& call )
{
    DEC_Rep_PathPoint* pPoint = dynamic_cast< DEC_Rep_PathPoint* >( call.GetParameter( 0 ).ToObject() );
    call.GetResult().SetValue( pPoint ? true : false );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypePoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetTypePoint( DIA_Call_ABC& call )
{
    DEC_Rep_PathPoint* pPoint = dynamic_cast< DEC_Rep_PathPoint* >( call.GetParameter( 0 ).ToObject() );
    call.GetResult().SetValue( pPoint->GetTypePoint() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetDestPoint( DIA_Call_ABC& call )
{
    DEC_Rep_PathPoint_Front* pFront = dynamic_cast< DEC_Rep_PathPoint_Front* >( call.GetParameter( 0 ).ToObject() );
    call.GetResult().SetValue( pFront->GetDestPoint() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypeLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetTypeLimaPoint( DIA_Call_ABC& call )
{
    DEC_Rep_PathPoint_Lima* pPoint = dynamic_cast< DEC_Rep_PathPoint_Lima* >( call.GetParameter( 0 ).ToObject() );
    call.GetResult().SetValue( pPoint->GetTypeLima() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void DEC_PathFunctions::GetLimaPoint( DIA_Call_ABC& call )
{
    DEC_Rep_PathPoint_Lima* pPoint = dynamic_cast< DEC_Rep_PathPoint_Lima* >( call.GetParameter( 0 ).ToObject() );
    call.GetResult().SetValue( pPoint->GetLimaID(), &DEC_Tools::GetTypeLima() );
}
