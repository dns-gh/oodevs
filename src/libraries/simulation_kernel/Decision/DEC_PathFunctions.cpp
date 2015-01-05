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
#include "PathfindComputer.h"
#include "Decision/Brain.h"
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Itinerary.h"
#include "Decision/DEC_PathType.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Terrain/PHY_RoleInterface_TerrainAnalysis.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionParameter_ABC.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Entities/Orders/MIL_ItineraryParameter.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Decision/DEC_Rep_PathPoint_Front.h"
#include "Decision/DEC_Rep_PathPoint_Lima.h"
#include "Tools/MIL_Tools.h"
#include "OnComponentComputer_ABC.h"
#include "simulation_terrain/TER_Pathfinder.h"
#include "simulation_terrain/TER_World.h"
#include <geometry/Types.h>
#include <boost/smart_ptr/make_shared.hpp>

namespace
{

boost::shared_ptr< DEC_Itinerary > GetItineraryById( uint32_t id )
{
    auto& pathfinder = MIL_AgentServer::GetWorkspace().GetPathfindComputer();
    boost::shared_ptr< DEC_Itinerary > itinerary;
    if( auto path = pathfinder.GetPathfind( id ) )
        itinerary = boost::make_shared< DEC_Itinerary >( *path );
    return itinerary;
}

bool BindItineraryToEntity( uint32_t pathId, const DEC_Decision_ABC* entity )
{
    if( !entity )
        return false;
    auto& pathfinder = MIL_AgentServer::GetWorkspace().GetPathfindComputer();
    return pathfinder.BindPathToEntity( pathId, entity->GetID() );
}

bool UnbindItineraryFromEntity( uint32_t pathId, const DEC_Decision_ABC* entity )
{
    if( !entity )
        return false;
    auto& pathfinder = MIL_AgentServer::GetWorkspace().GetPathfindComputer();
    return pathfinder.UnbindPathFromEntity( pathId, entity->GetID() );
}

std::vector< uint32_t > GetEntityItineraries( const DEC_Decision_ABC* entity )
{
    if( !entity )
        return std::vector< uint32_t >();
    auto& pathfinder = MIL_AgentServer::GetWorkspace().GetPathfindComputer();
    return pathfinder.GetEntityPaths( entity->GetID() );
}

}  // namespace

void DEC_PathFunctions::Register( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_CreerItineraire", &DEC_PathFunctions::CreatePathToPoint );
    brain.RegisterFunction( "_DEC_CreerItineraireBM", &DEC_PathFunctions::CreatePathToPointBM );
    brain.RegisterFunction( "_DEC_CreerItineraireListe", &DEC_PathFunctions::CreatePathToPointList );
    brain.RegisterFunction( "DEC_Itineraire_Etat", &DEC_PathFunctions::GetPathState );
    brain.RegisterFunction( "_DEC_Itineraire_EstEnMouvementSur", &DEC_PathFunctions::IsMovingOnPath );
    brain.RegisterFunction( "_DEC_GetNextObjectOnPath", &DEC_PathFunctions::GetNextObjectOnPath );
    brain.RegisterFunction( "_DEC_GetNextObjectOnPathWithBypassed", &DEC_PathFunctions::GetNextObjectOnPathWithBypassed );
    brain.RegisterFunction( "DEC_Itineraire_DernierPoint", &DEC_PathFunctions::GetLastPointOfPath );
    brain.RegisterFunction( "DEC_GetCurrentObjectCollision", &DEC_PathFunctions::GetCurrentObjectCollision );

    // Rep_Points
    brain.RegisterFunction( "DEC_GetRepPoint", &DEC_PathFunctions::GetRepPoint );
    brain.RegisterFunction( "DEC_IsAvantPoint", &DEC_PathFunctions::IsAvantPoint );
    brain.RegisterFunction( "DEC_IsPoint", &DEC_PathFunctions::IsPoint );
    brain.RegisterFunction( "DEC_GetTypePoint", &DEC_PathFunctions::GetTypePoint );
    brain.RegisterFunction( "DEC_GetDestPoint", &DEC_PathFunctions::GetDestPoint );
    brain.RegisterFunction( "DEC_GetTypeLimaPoint", &DEC_PathFunctions::GetTypeLimaPoint );
    brain.RegisterFunction( "DEC_GetLimaPoint", &DEC_PathFunctions::GetLimaPoint );
    brain.RegisterFunction( "DEC_GetNextRemovableObjectOnPath", &DEC_PathFunctions::GetNextRemovableObjectOnPath );
    brain.RegisterFunction( "DEC_GetClosestPath", &DEC_PathFunctions::GetClosestPath );

    // Itineraries
    brain.RegisterFunction( "DEC_Itinerary_GetById", &GetItineraryById );
    brain.RegisterFunction( "DEC_Itinerary_Bind", &BindItineraryToEntity );
    brain.RegisterFunction( "DEC_Itinerary_Unbind", &UnbindItineraryFromEntity );
    brain.RegisterFunction( "DEC_Itinerary_GetEntityItineraries", &GetEntityItineraries );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPointBM
// Created: MGD 2009-10-31
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Path_ABC > DEC_PathFunctions::CreatePathToPointBM( DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > end, int pathType )
{
    return CreatePathToPoint( callerAgent, end.get(), pathType );
}

namespace
{
    bool IsDestinationTrafficable( const MIL_Agent_ABC& agent, const std::vector< MT_Vector2D >& points )
    {
        const PHY_RoleInterface_TerrainAnalysis& analysis = agent.GetRole< PHY_RoleInterface_TerrainAnalysis >();
        return analysis.CanMoveOnUrbanBlock( points ) &&
               analysis.CanMoveOnBurningCells( points ) &&
               analysis.CanMoveOnKnowledgeObject( points );
    }

    template< typename Functor >
    class Visitor : public MIL_MissionParameterVisitor_ABC
    {
    public:
        explicit Visitor( const Functor& functor )
            : functor_( functor )
        {}
        void Accept( const std::string& /*p*/, const MIL_OrderTypeParameter& /*type*/, MIL_MissionParameter_ABC& element )
        {
            functor_( element );
        }
        Functor functor_;
    };

    template< typename Functor >
    void FindItinerary( DEC_Decision_ABC& decision, const Functor& functor )
    {
        const auto mission = decision.GetMission();
        if( !mission )
            return;
        Visitor< Functor > visitor( functor );
        mission->Visit( visitor );
    }

    boost::shared_ptr< DEC_Agent_Path > StartCompute( MIL_Agent_ABC& agent, const T_PointVector& points,
                                                      const DEC_PathType& pathType )
    {
        const auto path = boost::make_shared< DEC_Agent_Path >( agent, points, pathType );
        if( !IsDestinationTrafficable( agent, points ) )
        {
            path->Cancel();
            return path;
        }

        sword::Pathfind pathfind;
        const PHY_RoleInterface_Supply* role = agent.RetrieveRole< PHY_RoleInterface_Supply >();
        if( role && role->IsConvoy() )
            role->ToItinerary( pathfind );
        else
            FindItinerary( agent.GetRole< DEC_Decision_ABC >(),
                [&]( MIL_MissionParameter_ABC& element )
                { 
                    element.ToItinerary( pathfind );
                } );

        path->StartCompute( PathfindToPoints( pathfind ) );
        return path;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPoint
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Path_ABC > DEC_PathFunctions::CreatePathToPoint( DEC_Decision_ABC* callerAgent, MT_Vector2D* pEnd, int pathType )
{
    assert( pEnd );
    MIL_AgentPion& agent = callerAgent->GetPion();
    std::vector< MT_Vector2D > points;
     points.push_back( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    points.push_back( *pEnd );
    const DEC_PathType* pPathType = DEC_PathType::Find( pathType );
    assert( pPathType );
    return StartCompute( agent, points, *pPathType );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::CreatePathToPointList
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Path_ABC > DEC_PathFunctions::CreatePathToPointList( DEC_Decision_ABC* callerAgent, std::vector< boost::shared_ptr< MT_Vector2D > > listPt, int pathType )
{
    assert( !listPt.empty() );
    const DEC_PathType* pPathType = DEC_PathType::Find( pathType );
    assert( pPathType );
    MIL_AgentPion& agent = callerAgent->GetPion();
    std::vector< MT_Vector2D > points;
    points.push_back( agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    for( auto it = listPt.begin(); it != listPt.end(); ++it )
        points.push_back( **it );
    return StartCompute( agent, points, *pPathType );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetPathState
// Created: NLD 2004-09-23
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetPathState( TER_Path_ABC* pPath )
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
        if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
            container->GetObjectsAtInteractionHeight( knowledges, callerAgent, filter, bCheckBypassed );
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
std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > DEC_PathFunctions::GetNextObjectOnPath( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, float /*oDistance*/, const std::vector< std::string >& params )
{
    return ::GetNextObjectOnPath( callerAgent->GetPion(), params );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetNextObjectOnPathWithBypassed
// Created: MMC 2013-06-06
// -----------------------------------------------------------------------------
std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > DEC_PathFunctions::GetNextObjectOnPathWithBypassed( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, float /*oDistance*/, const std::vector< std::string >& params )
{
    return ::GetNextObjectOnPath( callerAgent->GetPion(), params, false );
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
        if( !canRemove_ && component.CanRemoveFromPath( type_, false ) ) // $$$$ BCI 2011-06-20: comment savoir si l'objet est miné ou pas?
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
boost::shared_ptr< MT_Vector2D > DEC_PathFunctions::GetLastPointOfPath( const TER_Path_ABC* pPath )
{
    assert( pPath );
    const DEC_PathResult* path = dynamic_cast< const DEC_PathResult* > ( pPath );
    if( !path || path->GetResult().empty() || !path->GetResult().back() )
        return boost::shared_ptr< MT_Vector2D >();
    return boost::make_shared< MT_Vector2D >( path->GetResult().back()->GetPos() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsMovingOnPath
// Created: JVT 2004-11-30
// Created: RPD 2009-08-04
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsMovingOnPath( const DEC_Decision_ABC* callerAgent, const TER_Path_ABC* pPath )
{
    return pPath && callerAgent->GetPion().GetRole< moving::PHY_RoleAction_Moving >().IsMovingOn( *pPath );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetRepPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PathFunctions::GetRepPoint( boost::shared_ptr< TER_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return boost::make_shared< MT_Vector2D >( pPoint->GetPos() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsAvantPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsAvantPoint( boost::shared_ptr< TER_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPoint->GetType() == TER_PathPoint::eTypePointFront;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::IsPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
bool DEC_PathFunctions::IsPoint( boost::shared_ptr< TER_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pPoint->GetType() == TER_PathPoint::eTypePointSpecial;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypePoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypePoint( boost::shared_ptr< TER_PathPoint > pPoint )
{
    if( !pPoint )
        throw MASA_EXCEPTION( "invalid parameter." );
    return static_cast< int >( pPoint->GetTypePoint() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetDestPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathPoint > DEC_PathFunctions::GetDestPoint( boost::shared_ptr< TER_PathPoint > pPoint )
{
    const auto* front = dynamic_cast< const DEC_Rep_PathPoint_Front* >( pPoint.get() );
    if( !front )
        throw MASA_EXCEPTION( "invalid front path point" );
    return front->GetDestPoint();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetTypeLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
int DEC_PathFunctions::GetTypeLimaPoint( boost::shared_ptr< TER_PathPoint > pPoint )
{
    const auto* lima = dynamic_cast< const DEC_Rep_PathPoint_Lima* >( pPoint.get() );
    if( !lima )
        throw MASA_EXCEPTION( "invalid lima path point" );
    return lima->GetTypeLima();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetLimaPoint
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
unsigned int DEC_PathFunctions::GetLimaPoint( boost::shared_ptr< TER_PathPoint > pPoint )
{
    const auto* lima = dynamic_cast< const DEC_Rep_PathPoint_Lima* >( pPoint.get() );
    if( !lima )
        throw MASA_EXCEPTION( "invalid lima path point" );
    return lima->GetLimaID();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFunctions::GetClosestPath
// Created: SLI 2014-09-05
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_PathFunctions::GetClosestPath( DEC_Decision_ABC* callerAgent,
                                                                                   const boost::shared_ptr< MT_Vector2D >& begin,
                                                                                   const boost::shared_ptr< MT_Vector2D >& end )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    FindItinerary( *callerAgent, [&]( MIL_MissionParameter_ABC& element )
    { 
        if( auto itinerary = dynamic_cast< MIL_ItineraryParameter* >( &element ) )
            result = itinerary->AddClosestWaypoints( begin, end );
    } );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object >& DEC_PathFunctions::GetCurrentObjectCollision
// Created: LDC 2014-12-01
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_PathFunctions::GetCurrentObjectCollision( DEC_Decision_ABC* callerAgent )
{
    return callerAgent->GetPion().GetRole< moving::PHY_RoleAction_Moving >().GetCurrentObjectCollision();
}
