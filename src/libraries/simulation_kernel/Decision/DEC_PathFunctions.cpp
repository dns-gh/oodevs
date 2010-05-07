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

#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Rep_PathPoint.h"
#include "Decision/DEC_Rep_PathPoint_Front.h"
#include "Decision/DEC_Rep_PathPoint_Lima.h"


// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPointBM
// Created: MGD 2009-10-31
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_PathFunctions::CreatePathToPointBM( MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > end, int pathType )
{
    return CreatePathToPoint( callerAgent, end.get(), pathType );
}

// -----------------------------------------------------------------------------
// Name: bool DEC_PathFunctions::ShouldEmbark
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::ShouldEmbark( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Path_ABC > path )
{
    double length = path->GetLength();
    double MaxSpeedUnloaded = callerAgent.GetRole< moving::PHY_RoleAction_Moving >().GetTheoricMaxSpeed( false );
    if( MaxSpeedUnloaded == 0 )
            return true;
    double timeUnloaded = length / MaxSpeedUnloaded;
    double timeLoaded = ( length / callerAgent.GetRole< moving::PHY_RoleAction_Moving >().GetTheoricMaxSpeed( true ) ) + DEC_AgentFunctions::GetLoadingTime( callerAgent ) * 60 + DEC_AgentFunctions::GetUnloadingTime( callerAgent ) * 60; //Conversion minutes into hours
    return ( timeLoaded < timeUnloaded );
}

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
// Name: DEC_PathFunctions::CreatePathToPoint
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_PathFunctions::CreatePathToPoint( MIL_AgentPion& callerAgent, MT_Vector2D* pEnd, int pathType, bool loaded )
{
    assert( pEnd );
    
    const DEC_PathType* pPathType = DEC_PathType::Find( pathType );
    assert( pPathType );

    boost::shared_ptr< DEC_Path_ABC > pPath( new DEC_Agent_Path( callerAgent, *pEnd, *pPathType, loaded ) );
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
std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > DEC_PathFunctions::GetNextObjectOnPath( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, float /*oDistance*/, const std::vector< std::string >& params )
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
MT_Vector2D* DEC_PathFunctions::GetRepPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    return (MT_Vector2D*)&pPoint->GetPos();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsAvantPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsAvantPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    return pPoint->GetType() == DEC_PathPoint::eTypePointFront;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    return pPoint->GetType() == DEC_PathPoint::eTypePointSpecial;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypePoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypePoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    return static_cast< int >( pPoint->GetTypePoint() );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_PathPoint > DEC_PathFunctions::GetDestPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{      
    return pPoint->GetDestPoint();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypeLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypeLimaPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    return pPoint->GetTypeLima();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
unsigned int DEC_PathFunctions::GetLimaPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    return pPoint->GetLimaID();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetDistancePath
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
double DEC_PathFunctions::GetDistancePath( const boost::shared_ptr< DEC_Path_ABC > pPath )
{
   return pPath->GetLength();
}

