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
#include "MIL_AgentServer.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Decision/DEC_Rep_PathPoint_Lima.h"
#include "Tools/MIL_Tools.h"
#include "OnComponentComputer_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPointBM
// Created: MGD 2009-10-31
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_PathFunctions::CreatePathToPointBM( MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > end, int pathType )
{
    return CreatePathToPoint( callerAgent, end.get(), pathType );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::ShouldEmbark
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::ShouldEmbark( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Path_ABC > path )
{
    if( !path )
        throw MASA_EXCEPTION( "invalid parameter." );
    const double length = path->GetLength();
    const double maxSpeedUnloaded = callerAgent.GetRole< moving::PHY_RoleAction_Moving >().GetTheoricMaxSpeed( false );
    if( maxSpeedUnloaded == 0 )
        return true;
    const double timeUnloaded = length / maxSpeedUnloaded;
    const double speed = callerAgent.GetRole< moving::PHY_RoleAction_Moving >().GetTheoricMaxSpeed( true );
    if( !speed )
        return false;
    const double timeLoaded = length / speed
        + callerAgent.GetRole< transport::PHY_RoleAction_Loading >().GetLoadingTime()
        + callerAgent.GetRole< transport::PHY_RoleAction_Loading >().GetUnloadingTime();
    return timeLoaded < timeUnloaded;
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
    boost::shared_ptr< DEC_Agent_Path > pPath( new DEC_Agent_Path( callerAgent, *pEnd, *pPathType ) );
    pPath->ComputePath( pPath );
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
    boost::shared_ptr< DEC_Agent_Path > pPath( new DEC_Agent_Path( callerAgent, listPt, *pPathType ) );
    pPath->ComputePath( pPath );
    return pPath;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetPathState
// Created: NLD 2004-09-23
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetPathState( MIL_AgentPion& /*callerAgent*/, DEC_Path_ABC* pPath )
{
    if( !pPath )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPath->GetState();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::ExtrapolatePosition
// Created: NLD 2004-05-13
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PathFunctions::ExtrapolatePosition( const MIL_AgentPion& callerAgent, const double time, bool bBoundOnPath )
{
    double simulationTime = MIL_Tools::ConvertMinutesToSim( time );
    boost::shared_ptr< MT_Vector2D > pPos( new MT_Vector2D( callerAgent.GetRole< moving::PHY_RoleAction_Moving >().ExtrapolatePosition( simulationTime, bBoundOnPath ) ) );
    return pPos;
}

namespace
{
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPath( MIL_ObjectFilter& filter, const MIL_Agent_ABC& callerAgent, bool bCheckBypassed = true )
    {
        std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > result;
        boost::shared_ptr< DEC_Knowledge_Object > pObjectColliding;
        double rDistanceCollision = 0.;
        T_KnowledgeObjectVector knowledges;
        callerAgent.GetKnowledgeGroup()->GetKnowledgeObjectContainer().GetObjectsAtInteractionHeight( knowledges, callerAgent, filter, bCheckBypassed );
        if( knowledges.empty() || !callerAgent.GetRole< moving::PHY_RoleAction_Moving >().ComputeFutureObjectCollision( knowledges, rDistanceCollision, pObjectColliding, callerAgent, false, false ) )
        {
            result.first = false;
            return result;
        }
        if( !pObjectColliding || !pObjectColliding->IsValid() )
            throw MASA_EXCEPTION( "invalid parameter." );
        result.first = true;
        result.second.first = pObjectColliding;
        result.second.second = MIL_Tools::ConvertSimToMeter( rDistanceCollision );
        return result;
    }

    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPath( const MIL_Agent_ABC& callerAgent, const std::vector< std::string >& params, bool bCheckBypassed = true )
    {
        if( params.empty() )
        {
            MIL_DangerousObjectFilter filter;
            return ::GetNextObjectOnPath( filter, callerAgent, bCheckBypassed );
        }
        else
        {
            MIL_ObjectFilter filter( params );
            return ::GetNextObjectOnPath( filter, callerAgent, bCheckBypassed );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetNextObjectOnPath
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > DEC_PathFunctions::GetNextObjectOnPath( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, float /*oDistance*/, const std::vector< std::string >& params )
{
    return ::GetNextObjectOnPath( callerAgent, params );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetNextObjectOnPathWithBypassed
// Created: MMC 2013-06-06
// -----------------------------------------------------------------------------
std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > DEC_PathFunctions::GetNextObjectOnPathWithBypassed( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, float /*oDistance*/, const std::vector< std::string >& params )
{
    return ::GetNextObjectOnPath( callerAgent, params, false );
}

class CanRemoveFromPathComputer : public OnComponentComputer_ABC
{
public:
    CanRemoveFromPathComputer( const MIL_ObjectType_ABC& type )
        : type_( type )
        , canRemove_( false )
    {}

    virtual void ApplyOnComponent( PHY_ComposantePion& component )
    {
        if( !canRemove_ && component.CanRemoveFromPath( type_, false ) ) // $$$$ BCI 2011-06-20: comment savoir si l'objet est min� ou pas?
            canRemove_ = true;
    }
    bool canRemove_;
private:
    const MIL_ObjectType_ABC& type_;
};

class RemovableFromPathObjectFilter : public MIL_ObjectFilter
{
public:
    RemovableFromPathObjectFilter( MIL_Agent_ABC& agent )
        : agent_( agent )
    {}

    virtual bool Test ( const MIL_ObjectType_ABC& type ) const
    {
        CanRemoveFromPathComputer computer( type );
        agent_.Execute< OnComponentComputer_ABC >( computer );
        return computer.canRemove_;
    }

private:
    MIL_Agent_ABC& agent_;
};

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetNextRemovableObjectOnPath
// Created: BCI 2011-06-20
// -----------------------------------------------------------------------------
std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > DEC_PathFunctions::GetNextRemovableObjectOnPath( const DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > oId, float /*oDistance*/ )
{
    RemovableFromPathObjectFilter filter( callerAgent.GetPion() );
    return ::GetNextObjectOnPath( filter, callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLastPointOfPath
// Created: JVT 2004-11-30
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PathFunctions::GetLastPointOfPath( const MIL_AgentPion& /*callerAgent*/, const DEC_Path_ABC* pPath )
{
    assert( pPath );
    const DEC_PathResult* path = dynamic_cast< const DEC_PathResult* > ( pPath );
    if( !path || path->GetResult().empty() || !path->GetResult().back() )
    {
        return boost::shared_ptr< MT_Vector2D >();
    }
    boost::shared_ptr< MT_Vector2D > pPos ( new MT_Vector2D( path->GetResult().back()->GetPos() ) );
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
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return (MT_Vector2D*)&pPoint->GetPos();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsAvantPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsAvantPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPoint->GetType() == DEC_PathPoint::eTypePointFront;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPoint->GetType() == DEC_PathPoint::eTypePointSpecial;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypePoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypePoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return static_cast< int >( pPoint->GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_PathPoint > DEC_PathFunctions::GetDestPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPoint->GetDestPoint();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypeLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypeLimaPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPoint->GetTypeLima();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
unsigned int DEC_PathFunctions::GetLimaPoint( boost::shared_ptr< DEC_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPoint->GetLimaID();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetDistancePath
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
double DEC_PathFunctions::GetDistancePath( const boost::shared_ptr< DEC_Path_ABC > pPath )
{
    if( !pPath )
        throw MASA_EXCEPTION( "invalid parameter." );
   return pPath->GetLength();
}
