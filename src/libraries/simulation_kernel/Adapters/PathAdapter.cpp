// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PathAdapter.h"
#include "Hook.h"
#include "MIL_AgentServer.h"
#include "RoleAction_Moving.h"
#include "Decision/DEC_Path_KnowledgeAgent.h"
#include "Decision/DEC_Path_KnowledgeObject.h"
#include "Decision/DEC_Path_KnowledgeObjectFlood.h"
#include "Decision/DEC_Path_KnowledgeObjectBurnSurface.h"
#include "Decision/DEC_Path_KnowledgePopulation.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/FloodCapacity.h"
#include "Entities/Objects/BurnSurfaceCapacity.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/RawVisionData/Elevation.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Urban/MIL_UrbanCache.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include <core/Convert.h>
#include <boost/lexical_cast.hpp>
#include <boost/thread/mutex.hpp>

using namespace sword;

DECLARE_HOOK( ExecutePathfind, void, ( size_t path, TER_Pathfinder_ABC& pathfind ) )
DECLARE_HOOK( PathGetLength, double, ( size_t path ) )
DECLARE_HOOK( PathGetState, DEC_Path_ABC::E_State, ( size_t path ) )

DECLARE_HOOK( AvoidEnemies, bool, ( size_t path ) )
DECLARE_HOOK( GetEnemyCostAtSecurityRange, double, ( size_t path ) )
DECLARE_HOOK( GetEnemyCostOnContact, double, ( size_t path ) )

DECLARE_HOOK( AvoidObjects, bool, ( size_t path ) )
DECLARE_HOOK( GetFirstPoint, void, ( size_t path, void(*callback)( const MT_Vector2D& point, void* userData ), void* userData ) )
DECLARE_HOOK( GetObjectCost, double, ( size_t path, unsigned int type ) )
DECLARE_HOOK( GetThreshold, double, ( size_t path ) )

DECLARE_HOOK( HandlePopulations, bool, ( size_t path ) )
DECLARE_HOOK( GetPopulationSecurityRange, double, ( size_t path ) )
DECLARE_HOOK( GetCostOutsideOfPopulation, double, ( size_t path ) )
DECLARE_HOOK( GetPopulationAttitudeCost, double, ( size_t path, unsigned int type ) )
DECLARE_HOOK( RemovePath, void, ( size_t path ) )

DEFINE_HOOK( NotifyPathCreation, 0, size_t, () )
{
    return sword::PathAdapter::Add();
}
DEFINE_HOOK( GetPathHandler, 1, const void*, ( size_t identifier ) )
{
    return PathAdapter::Get( identifier ).get();
}
DEFINE_HOOK( InitializePath, 2, void, ( size_t path, const SWORD_Model* entity ) )
{
    sword::PathAdapter::Get( path )->Initialize( *core::Convert( entity ) );
}

DEFINE_HOOK( GetFuseauxCost, 10, double, ( const void* handler,
                                           const MT_Vector2D& from, const MT_Vector2D& to,
                                           double rMaximumFuseauDistance, double rMaximumFuseauDistanceWithAutomata, // $$$$ MCO : all those configuration values should stay out of the movement module
                                           double rFuseauCostPerMeterOut, double rComfortFuseauDistance, double rFuseauCostPerMeterIn,
                                           double rMaximumAutomataFuseauDistance, double rAutomataFuseauCostPerMeterOut ) )
{
    return static_cast< const PathAdapter* >( handler )->GetFuseauxCost( from, to, rMaximumFuseauDistance, rMaximumFuseauDistanceWithAutomata, rFuseauCostPerMeterOut, rComfortFuseauDistance, rFuseauCostPerMeterIn, rMaximumAutomataFuseauDistance, rAutomataFuseauCostPerMeterOut );
}

DEFINE_HOOK( GetObjectsCost, 5, double, ( const void* handler,
                                          const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) )
{
    return static_cast< const PathAdapter* >( handler )->GetObjectsCost( from, to, nToTerrainType, nLinkTerrainType );
}

DEFINE_HOOK( GetUrbanBlockCost, 3, double, ( const void* handler, const MT_Vector2D& from, const MT_Vector2D& to ) )
{
    return static_cast< const PathAdapter* >( handler )->GetUrbanBlockCost( from, to );
}

DEFINE_HOOK( GetEnemiesCost, 6, double, ( const void* handler, const MT_Vector2D& from, const MT_Vector2D& to,
                                          const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rEnemyMaximumCost ) )
{
    return static_cast< const PathAdapter* >( handler )->GetEnemiesCost( from, to, nToTerrainType, nLinkTerrainType, rEnemyMaximumCost );
}

DEFINE_HOOK( GetPopulationsCost, 6, double, ( const void* handler, const MT_Vector2D& from, const MT_Vector2D& to,
                                              const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rPopulationMaximumCost ) )
{
    return static_cast< const PathAdapter* >( handler )->GetPopulationsCost( from, to, nToTerrainType, nLinkTerrainType, rPopulationMaximumCost );
}

DEFINE_HOOK( GetAltitudeCost, 4, double, ( const void* handler, const MT_Vector2D& from, const MT_Vector2D& to, double rAltitudeCostPerMeter ) )
{
    return static_cast< const PathAdapter* >( handler )->GetAltitudeCost( from, to, rAltitudeCostPerMeter );
}

namespace
{
    typedef std::map< std::size_t, boost::shared_ptr< PathAdapter > > T_Paths;
    T_Paths paths;
    boost::mutex mutex;
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Add
// Created: SLI 2012-12-12
// -----------------------------------------------------------------------------
std::size_t PathAdapter::Add()
{
    boost::shared_ptr< PathAdapter > path( new PathAdapter() );
    boost::mutex::scoped_lock lock( mutex );
    paths[ path->GetID() ] = path;
    return path->GetID();
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Get
// Created: MCO 2012-05-21
// -----------------------------------------------------------------------------
boost::shared_ptr< PathAdapter > PathAdapter::Get( std::size_t identifier )
{
    boost::mutex::scoped_lock lock( mutex );
    boost::shared_ptr< PathAdapter > result = paths[ identifier ];
    if( result )
        return result;
    throw MASA_EXCEPTION( "Could not retrieve path '" + boost::lexical_cast< std::string >( identifier ) + "'" );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Remove
// Created: MCO 2012-05-21
// -----------------------------------------------------------------------------
void PathAdapter::Remove( std::size_t identifier )
{
    boost::mutex::scoped_lock lock( mutex );
    paths.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter constructor
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
PathAdapter::PathAdapter()
    : entity_           ( std::numeric_limits< std::size_t >::max() )
    , data_             ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() )
    , weight_           ( 0 )
    , squareSlope_      ( 0 )
    , outsideObjectCost_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathAdapter destructor
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
PathAdapter::~PathAdapter()
{
    GET_HOOK( RemovePath )( GetID() );
}

namespace
{
    double Square( double x )
    {
        return x * x;
    }
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Initialize
// Created: SLI 2012-12-14
// -----------------------------------------------------------------------------
void PathAdapter::Initialize( const core::Model& entity )
{
    entity_ = entity[ "identifier" ];
    weight_ = entity[ "data" ].GetUserData< MIL_AgentPion >().GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight(); // $$$$ MCO 2012-05-23: read from model
    squareSlope_ = Square( entity[ "movement/max-slope" ] );
    height_ = entity[ "data" ].GetUserData< MIL_AgentPion >().GetType().GetUnitType().GetCrossingHeight(); // $$$$ MCO 2012-05-23: read from model
    MIL_AgentPion& pion = entity[ "data" ].GetUserData< MIL_AgentPion >();
    fuseau_= pion.GetOrderManager().GetFuseau();
    automateFuseau_ = pion.GetAutomate().GetOrderManager().GetFuseau();
    const auto& reinforcements = entity[ "data" ].GetUserData< MIL_AgentPion >().GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( auto it = reinforcements.begin(); it != reinforcements.end(); ++it )
        weight_ = std::max( weight_, ( *it )->GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight() );
    InitializePathKnowledges( entity, pion );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Execute
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
void PathAdapter::Execute( TER_Pathfinder_ABC& pathfind )
{
    pathfind.SetId( entity_ );
    GET_HOOK( ExecutePathfind )( GetID(), pathfind );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::CleanAfterComputation
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
void PathAdapter::CleanAfterComputation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::IsPathForUnit
// Created: JSR 2013-03-11
// -----------------------------------------------------------------------------
bool PathAdapter::IsPathForUnit( MIL_Agent_ABC* /*pion*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetLength
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
double PathAdapter::GetLength() const
{
    return GET_HOOK( PathGetLength )( GetID() );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetState
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
DEC_Path_ABC::E_State PathAdapter::GetState() const
{
    return GET_HOOK( PathGetState )( GetID() );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetObjectsCost
// Created: MCO 2012-05-23
// -----------------------------------------------------------------------------
double PathAdapter::GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
{
    double cost = outsideObjectCost_;
    for( auto itType = pathKnowledgeObjects_.begin(); itType != pathKnowledgeObjects_.end(); ++itType )
    {
        bool bInsideObjectType = false;
        const T_PathKnowledgeObjectVector& knowledges = *itType;
        for( auto itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )
        {
            double rCurrentObjectCost = (*itKnowledge)->ComputeCost( from, to, nToTerrainType, nLinkTerrainType, weight_ );
            if( rCurrentObjectCost != std::numeric_limits< double >::min() )
            {
                if( !bInsideObjectType )
                {
                    cost -= (*itKnowledge)->GetCostOut();
                    bInsideObjectType = true;
                }
                if( rCurrentObjectCost < 0. ) // Impossible move (for example destroyed bridge)
                    return rCurrentObjectCost;
                cost += rCurrentObjectCost;
            }
        }
    }
    return cost;
}

namespace
{
    void AddPoint( const MT_Vector2D& point, void* userData )
    {
        static_cast< std::vector< MT_Vector2D >* >( userData )->push_back( point );
    }
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::InitializePathKnowledges
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void PathAdapter::InitializePathKnowledges( const core::Model& entity, const MIL_AgentPion& pion )
{
    // Enemies
    if( GET_HOOK( AvoidEnemies )( GetID() ) )
    {
        auto bbKg = pion.GetKnowledgeGroup()->GetKnowledge();
        if( bbKg )
        {
            const T_KnowledgeAgentVector& enemies = bbKg->GetEnemies();
            for( auto itKnowledgeAgent = enemies.begin(); itKnowledgeAgent != enemies.end(); ++itKnowledgeAgent )
            {
                const DEC_Knowledge_Agent& knowledge = **itKnowledgeAgent;
                if( knowledge.IsValid() && fuseau_.IsInside( knowledge.GetPosition() ) )
                {
                    const double factor = GET_HOOK( GetEnemyCostOnContact )( GetID() );
                    if( factor > 0 )
                        pathKnowledgeAgents_.push_back( DEC_Path_KnowledgeAgent( knowledge.GetPosition(),
                            GET_HOOK( GetEnemyCostAtSecurityRange )( GetID() ), factor, knowledge.GetMaxRangeToFireOn( pion, 0 ) ) );
                }
            }
        }
    }
    // Objects
    if( GET_HOOK( AvoidObjects )( GetID() ) )
    {
        T_KnowledgeObjectVector knowledgesObject;
        MIL_DangerousObjectFilter filter;
        if( DEC_BlackBoard_CanContainKnowledgeObject* container = pion.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
            container->GetObjectsAtInteractionHeight( knowledgesObject, pion, filter );
        T_PointVector firstPointVector;
        GET_HOOK( GetFirstPoint )( GetID(), &AddPoint, &firstPointVector );
        for( auto itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
        {
            const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
            if( knowledge.CanCollideWith( pion ) )
            {
                if( knowledge.IsObjectInsidePathPoint( firstPointVector, pion ) )
                {
                    if( const MIL_Object_ABC* pObject = knowledge.GetObjectKnown() )
                    {
                        TerrainData data;
                        double rMaxSpeed = entity[ "data" ].GetUserData< MIL_AgentPion >().GetRole< RoleAction_Moving >().GetSpeed( data, *pObject );
                        if( rMaxSpeed == 0 || rMaxSpeed == std::numeric_limits< double >::max() )
                            continue;
                    }
                }
                if( pathKnowledgeObjects_.size() <= knowledge.GetType().GetID() )
                    pathKnowledgeObjects_.resize( knowledge.GetType().GetID() + 1 );
                T_PathKnowledgeObjectVector& pathKnowledges = pathKnowledgeObjects_[ knowledge.GetType().GetID() ];
                bool empty = pathKnowledges.empty();
                if( knowledge.GetType().GetCapacity< FloodCapacity >() )
                    pathKnowledges.push_back( boost::shared_ptr< DEC_Path_KnowledgeObject_ABC >( new DEC_Path_KnowledgeObjectFlood( height_, knowledge ) ) );
                else if( knowledge.GetType().GetCapacity< BurnSurfaceCapacity >() )
                    pathKnowledges.push_back( boost::shared_ptr< DEC_Path_KnowledgeObject_ABC >( new DEC_Path_KnowledgeObjectBurnSurface( knowledge ) ) );
                else
                {
                    const double cost = GET_HOOK( GetObjectCost )( GetID(), knowledge.GetType().GetID() );
                    if( cost != 0 && knowledge.GetLocalisation().GetType() != TER_Localisation::eNone )
                        pathKnowledges.push_back( boost::shared_ptr< DEC_Path_KnowledgeObject_ABC >( new DEC_Path_KnowledgeObject( knowledge, cost, GET_HOOK( GetThreshold )( GetID() ) ) ) );
                }
                if( empty && pathKnowledges.size() == 1 && pathKnowledges.front()->GetCostOut() > 0 )
                    outsideObjectCost_ += pathKnowledges.front()->GetCostOut();
            }
        }
    }
    // Populations
    if( GET_HOOK( HandlePopulations )( GetID() ) )
    {
        T_KnowledgePopulationVector knowledgesPopulation;
        auto bbKg = pion.GetKnowledgeGroup()->GetKnowledge();
        if( bbKg )
        {
            bbKg->GetPopulations( knowledgesPopulation );
            pathKnowledgePopulations_.reserve( knowledgesPopulation.size() );
            for( auto it = knowledgesPopulation.begin(); it != knowledgesPopulation.end(); ++it )
                pathKnowledgePopulations_.push_back( boost::shared_ptr< DEC_Path_KnowledgePopulation >( new DEC_Path_KnowledgePopulation( **it, *this, !pion.GetType().IsTerrorist() ) ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetPopulationSecurityRange
// Created: MCO 2012-09-20
// -----------------------------------------------------------------------------
double PathAdapter::GetPopulationSecurityRange() const
{
    return GET_HOOK( GetPopulationSecurityRange )( GetID() );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetCostOutsideOfPopulation
// Created: MCO 2012-09-20
// -----------------------------------------------------------------------------
double PathAdapter::GetCostOutsideOfPopulation() const
{
    return GET_HOOK( GetCostOutsideOfPopulation )( GetID() );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetPopulationAttitudeCost
// Created: MCO 2012-09-20
// -----------------------------------------------------------------------------
double PathAdapter::GetPopulationAttitudeCost( unsigned int attitudeID ) const
{
    return GET_HOOK( GetPopulationAttitudeCost )( GetID(), attitudeID );
}

namespace
{
    bool IsSlopeTooSteep( const PHY_RawVisionData& data, const MT_Vector2D& from, const MT_Vector2D& to, double maxSquareSlope )
    {
        const double delta = data.GetAltitude( to ) - data.GetAltitude( from );
        return Square( delta ) > from.SquareDistance( to ) * maxSquareSlope;
    }
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetAltitudeCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double PathAdapter::GetAltitudeCost( const MT_Vector2D& from, const MT_Vector2D& to, double costPerMeter ) const
{
    auto elevationChecker = boost::bind( &IsSlopeTooSteep, boost::cref( data_ ),
            from, _1, squareSlope_ );
    if( Elevation( data_.GetCellSize() ).FindPath( from, to, elevationChecker ) )
        return -1;
    const double altitudeTo = data_.GetAltitude( to );
    const double altitude = costPerMeter > 0 ? data_.GetMaxAltitude() : data_.GetMinAltitude();
    return ( altitude - altitudeTo ) * costPerMeter;
}

namespace
{
    double ComputeFuseauMaximumDistance( const MIL_Fuseau& fuseau, const MIL_Fuseau& automateFuseau,
                                         const MT_Vector2D& from, const MT_Vector2D& to, double rMaximumFuseauDistance, double rMaxFuseauDistanceWithAutomata )
    {
        if( fuseau.IsNull() )
            return 0;
        if( ! fuseau.IsInside( from ) )
            rMaximumFuseauDistance += fuseau.Distance( from ) * 1.3; // $$$$ AGE 2005-06-08:
        if( ! automateFuseau.IsNull() ) // I have a higher fuseau
            rMaximumFuseauDistance = std::max( rMaximumFuseauDistance, rMaxFuseauDistanceWithAutomata );
        if( ! fuseau.IsInside( to ) ) // our destination is outside
            return std::numeric_limits< double >::max();
        return rMaximumFuseauDistance;
    }
    double ComputeAutomateFuseauMaximumDistance( const MIL_Fuseau& automateFuseau, const MT_Vector2D& from, double rMaximumAutomataFuseauDistance )
    {
        if( automateFuseau.IsNull() )
            return 0;
        if( ! automateFuseau.IsInside( from ) )
            rMaximumAutomataFuseauDistance += automateFuseau.Distance( from ) * 1.3; // $$$$ AGE
        return rMaximumAutomataFuseauDistance;
    }
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetFuseauxCost
// Created: MCO 2012-05-23
// -----------------------------------------------------------------------------
double PathAdapter::GetFuseauxCost( const MT_Vector2D& from, const MT_Vector2D& to,
    double rMaximumFuseauDistance, double rMaximumFuseauDistanceWithAutomata,
    double rFuseauCostPerMeterOut, double rComfortFuseauDistance, double rFuseauCostPerMeterIn,
    double rMaximumAutomataFuseauDistance, double rAutomataFuseauCostPerMeterOut ) const
{
    double rCost = 0;
    // Vérification si le link est dans le fuseau de l'agent
    {
        const double rFuseauCost = fuseau_.GetCost( from, to,
            ComputeFuseauMaximumDistance( fuseau_, automateFuseau_, from, to, rMaximumFuseauDistance, rMaximumFuseauDistanceWithAutomata ),
            rFuseauCostPerMeterOut, rComfortFuseauDistance, rFuseauCostPerMeterIn );
        if( rFuseauCost < 0 )
            return -1;
        rCost += rFuseauCost;
    }
    // $$$$ AGE 2005-06-24: Going out of the automate fuseau is a no-no. Avoid precision issues
    {
        const double rAutomateFuseauCost = automateFuseau_.GetCost( from, to,
            ComputeAutomateFuseauMaximumDistance( automateFuseau_, from, rMaximumAutomataFuseauDistance ),
            rAutomataFuseauCostPerMeterOut, 0, 0 );
        if( rAutomateFuseauCost < 0 )
            return -1;
        rCost += rAutomateFuseauCost;
    }
    return rCost;
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetUrbanBlockCost
// Created: MCO 2012-05-23
// -----------------------------------------------------------------------------
double PathAdapter::GetUrbanBlockCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    if( ! MIL_AgentServer::IsInitialized() ) // $$$$ MCO 2012-05-24: not thread-safe
        return 0;
    return MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlockCost( static_cast< float >( weight_ ), from, to ); // $$$$ MCO 2012-05-24: not thread-safe
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetEnemiesCost
// Created: MCO 2012-05-23
// -----------------------------------------------------------------------------
double PathAdapter::GetEnemiesCost( const MT_Vector2D& from, const MT_Vector2D& to,
    const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/, double rEnemyMaximumCost ) const
{
    double rEnemyCost = 0.;
    const MT_Line lineLink( from, to );
    const DEC_Path_KnowledgeAgent::BoundingBox box( from, to );
    for( auto it = pathKnowledgeAgents_.begin(); it != pathKnowledgeAgents_.end(); ++it )
    {
        double rCurrentEnemyCost = it->ComputeCost( lineLink, box );
        if( rCurrentEnemyCost < 0. ) // Impossible move (for example destroyed bridge)
            return rCurrentEnemyCost;
        rEnemyCost += rCurrentEnemyCost;
    }
    return std::min( rEnemyCost, rEnemyMaximumCost );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetPopulationsCost
// Created: MCO 2012-05-23
// -----------------------------------------------------------------------------
double PathAdapter::GetPopulationsCost( const MT_Vector2D& /*from*/, const MT_Vector2D& to,
    const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/, double rPopulationMaximumCost ) const
{
    double rCost = 0.;
    for( auto it = pathKnowledgePopulations_.begin(); it != pathKnowledgePopulations_.end(); ++it )
    {
        double rCurrentCost = (*it)->ComputeCost( to );
        if( rCurrentCost < 0. ) // Impossible move
            return rCurrentCost;
        rCost += rCurrentCost;
    }
    return std::min( rCost, rPopulationMaximumCost );
}
