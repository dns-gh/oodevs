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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
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
boost::shared_ptr< DEC_Path_ABC > DEC_PathFunctions::CreatePathToPoint( MIL_AgentPion& callerAgent, MT_Vector2D* pEnd, int pathType )
{
    assert( pEnd );
    
    const DEC_PathType* pPathType = DEC_PathType::Find( pathType );
    assert( pPathType );

    boost::shared_ptr< DEC_Path_ABC > pPath( new DEC_Agent_Path( callerAgent, *pEnd, *pPathType ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pPath );
    return pPath;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPointList
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_PathFunctions::CreatePathToPointList( MIL_AgentPion& callerAgent, std::vector< boost::shared_ptr< MT_Vector2D > > listPt, int pathType  )
{
    assert( !listPt.empty() );

    const DEC_PathType* pPathType = DEC_PathType::Find( pathType );
    assert( pPathType );

    boost::shared_ptr< DEC_Path_ABC > pPath( new DEC_Agent_Path( callerAgent, listPt, *pPathType ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pPath );
    return pPath;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetPathState
// Created: NLD 2004-09-23
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetPathState( MIL_AgentPion& /*callerAgent*/, DEC_Path_ABC* pPath )
{
    assert( pPath );
    return pPath->GetState();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::ExtrapolatePosition
// Created: NLD 2004-05-13
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PathFunctions::ExtrapolatePosition( const MIL_AgentPion& callerAgent, const MT_Float time, bool bBoundOnPath )
{
    MT_Float simulationTime        = MIL_Tools::ConvertMinutesToSim( time );
    boost::shared_ptr< MT_Vector2D > pPos( new MT_Vector2D( callerAgent.GetRole< moving::PHY_RoleAction_Moving >().ExtrapolatePosition( simulationTime, bBoundOnPath ) ) );
    return pPos;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetNextObjectOnPath
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > DEC_PathFunctions::GetNextObjectOnPath( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, float /*oDistance*/, const std::vector< std::string >& params )
{
    MIL_ObjectFilter filter( params );
    
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > result;
    
    boost::shared_ptr< DEC_Knowledge_Object > pObjectColliding;
    
    MT_Float rDistanceCollision = 0.;

    const PHY_RoleInterface_Location& roleLocation = callerAgent.GetRole< PHY_RoleInterface_Location >();
    const MT_Float                    rHeight      = roleLocation.GetHeight  ();
    const MT_Vector2D&                position     = roleLocation.GetPosition();

    T_KnowledgeObjectVector knowledges;
    callerAgent.GetArmy().GetKnowledge().GetObjectsAtInteractionHeight( knowledges, rHeight, filter );

    if( knowledges.empty() || !callerAgent.GetRole< moving::PHY_RoleAction_Moving >().ComputeFutureObjectCollision( position, knowledges, rDistanceCollision, pObjectColliding ) )
    {
        result.first = false;
        return result;
    }
    assert( pObjectColliding && pObjectColliding->IsValid() );

    result.first = true;
    result.second.first = pObjectColliding;
    result.second.second = MIL_Tools::ConvertSimToMeter( rDistanceCollision );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLastPointOfPath
// Created: JVT 2004-11-30
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PathFunctions::GetLastPointOfPath( const MIL_AgentPion& /*callerAgent*/, const DEC_Path_ABC* pPath )
{
    assert( pPath );
    boost::shared_ptr< MT_Vector2D > pPos ( new MT_Vector2D( dynamic_cast< const DEC_PathResult* > ( pPath )->GetResult().back()->GetPos() ) );
    return pPos;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsMovingOnPath
// Created: JVT 2004-11-30
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsMovingOnPath( const MIL_AgentPion& callerAgent, const DEC_Path_ABC* pPath )
{
    return pPath ? callerAgent.GetRole< moving::PHY_RoleAction_Moving >().IsMovingOn( *pPath ) : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetRepPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
MT_Vector2D* DEC_PathFunctions::GetRepPoint( DEC_PathPoint* pPoint )
{
    return (MT_Vector2D*)&pPoint->GetPos();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsAvantPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsAvantPoint( DEC_PathPoint* pPoint )
{
    DEC_Rep_PathPoint_Front* pFront = dynamic_cast< DEC_Rep_PathPoint_Front* >( pPoint );
    return pFront ? true : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsPoint( DEC_PathPoint* pPoint )
{
    DEC_Rep_PathPoint* pPathPoint = dynamic_cast< DEC_Rep_PathPoint* >( pPoint );
    return pPathPoint ? true : false;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypePoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypePoint( DEC_PathPoint* pPoint )
{
    DEC_Rep_PathPoint* pPathPoint = dynamic_cast< DEC_Rep_PathPoint* >( pPoint );
    return static_cast< int >( pPathPoint->GetTypePoint() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
DEC_PathPoint* DEC_PathFunctions::GetDestPoint( DEC_PathPoint* pPoint )
{
    DEC_Rep_PathPoint_Front* pFront = dynamic_cast< DEC_Rep_PathPoint_Front* >( pPoint );
    return &pFront->GetDestPoint();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypeLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypeLimaPoint( DEC_PathPoint* pPoint )
{
    DEC_Rep_PathPoint_Lima* pLimaPoint = dynamic_cast< DEC_Rep_PathPoint_Lima* >( pPoint );
    return pLimaPoint->GetTypeLima();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
unsigned int DEC_PathFunctions::GetLimaPoint( DEC_PathPoint* pPoint )
{
    DEC_Rep_PathPoint_Lima* pLimaPoint = dynamic_cast< DEC_Rep_PathPoint_Lima* >( pPoint );
    return pLimaPoint->GetLimaID();
}
