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
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Urban/MIL_UrbanCache.h"
#include <core/Convert.h>

using namespace sword;

DECLARE_HOOK( CleanPathAfterComputation, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( ExecutePathfind, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, TerrainPathfinder& pathfind ) )
DECLARE_HOOK( PathGetLength, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( PathGetState, DEC_Path_ABC::E_State, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )

DECLARE_HOOK( AvoidEnemies, bool, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( GetEnemyCostAtSecurityRange, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( GetEnemyCostOnContact, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )

DECLARE_HOOK( AvoidObjects, bool, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( GetFirstPoint, void, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, void(*callback)( const MT_Vector2D& point, void* userData ), void* userData ) )
DECLARE_HOOK( GetObjectCost, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, unsigned int type ) )
DECLARE_HOOK( GetThreshold, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )

DECLARE_HOOK( HandlePopulations, bool, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( GetPopulationSecurityRange, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( GetCostOutsideOfPopulation, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path ) )
DECLARE_HOOK( GetPopulationAttitudeCost, double, ( const boost::shared_ptr< sword::movement::Path_ABC >& path, unsigned int type ) )

DEFINE_HOOK( GetFuseauxCost, 11, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path,
                                           const MT_Vector2D& from, const MT_Vector2D& to,
                                           double rMaximumFuseauDistance, double rMaximumFuseauDistanceWithAutomata, // $$$$ MCO : all those configuration values should stay out of the movement module
                                           double rFuseauCostPerMeterOut, double rComfortFuseauDistance, double rFuseauCostPerMeterIn,
                                           double rMaximumAutomataFuseauDistance, double rAutomataFuseauCostPerMeterOut ) )
{
    return sword::PathAdapter::Add( *core::Convert( entity ), path )->GetFuseauxCost( from, to, rMaximumFuseauDistance, rMaximumFuseauDistanceWithAutomata, rFuseauCostPerMeterOut, rComfortFuseauDistance, rFuseauCostPerMeterIn, rMaximumAutomataFuseauDistance, rAutomataFuseauCostPerMeterOut );
}

DEFINE_HOOK( GetObjectsCost, 6, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path,
                                          const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) )
{
    return PathAdapter::Add( *core::Convert( entity ), path )->GetObjectsCost( from, to, nToTerrainType, nLinkTerrainType );
}

DEFINE_HOOK( GetUrbanBlockCost, 4, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to ) )
{
    return sword::PathAdapter::Add( *core::Convert( entity ), path )->GetUrbanBlockCost( from, to );
}

DEFINE_HOOK( GetEnemiesCost, 7, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to,
                                          const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rEnemyMaximumCost ) )
{
    return sword::PathAdapter::Add( *core::Convert( entity ), path )->GetEnemiesCost( from, to, nToTerrainType, nLinkTerrainType, rEnemyMaximumCost );
}

DEFINE_HOOK( GetPopulationsCost, 7, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to,
                                              const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rPopulationMaximumCost ) )
{
    return sword::PathAdapter::Add( *core::Convert( entity ), path )->GetPopulationsCost( from, to, nToTerrainType, nLinkTerrainType, rPopulationMaximumCost );
}

DEFINE_HOOK( GetAltitudeCost, 5, double, ( const SWORD_Model* entity, const boost::shared_ptr< sword::movement::Path_ABC >& path, const MT_Vector2D& from, const MT_Vector2D& to, double rAltitudeCostPerMeter ) )
{
    return sword::PathAdapter::Add( *core::Convert( entity ), path )->GetAltitudeCost( from, to, rAltitudeCostPerMeter );
}

namespace
{
    typedef std::map< boost::shared_ptr< movement::Path_ABC >, boost::shared_ptr< PathAdapter > > T_Paths;
    T_Paths paths;
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Add
// Created: MCO 2012-05-21
// -----------------------------------------------------------------------------
const boost::shared_ptr< PathAdapter >& PathAdapter::Add( const core::Model& entity, const boost::shared_ptr< movement::Path_ABC >& path )
{
    boost::shared_ptr< PathAdapter >& result = paths[ path ];
    if( ! result )
        result.reset( new PathAdapter( entity, path ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Remove
// Created: MCO 2012-05-21
// -----------------------------------------------------------------------------
boost::shared_ptr< PathAdapter > PathAdapter::Remove( const boost::shared_ptr< movement::Path_ABC >& path )
{
    boost::shared_ptr< PathAdapter > result;
    T_Paths::iterator it = paths.find( path );
    if( it != paths.end() )
    {
        result = it->second;
        paths.erase( it );
    }
    return result;
}

namespace
{
    double Square( double x )
    {
        return x * x;
    }
}

// -----------------------------------------------------------------------------
// Name: PathAdapter constructor
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
PathAdapter::PathAdapter( const core::Model& entity, const boost::shared_ptr< movement::Path_ABC >& path )
    : path_        ( path )
    , altitudeData_( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() )
    , weight_      ( entity[ "data" ].GetUserData< MIL_AgentPion >().GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight() ) // $$$$ MCO 2012-05-23: read from model
    , squareSlope_ ( Square( entity[ "movement/max-slope" ] ) )
    , height_      ( entity[ "data" ].GetUserData< MIL_AgentPion >().GetType().GetUnitType().GetCrossingHeight() ) // $$$$ MCO 2012-05-23: read from model
{
    MIL_AgentPion& pion = entity[ "data" ].GetUserData< MIL_AgentPion >();
    fuseau_= pion.GetOrderManager().GetFuseau();
    automateFuseau_ = pion.GetAutomate().GetOrderManager().GetFuseau();
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = entity[ "data" ].GetUserData< MIL_AgentPion >().GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itPion = reinforcements.begin(); itPion != reinforcements.end(); ++itPion )
        weight_ = std::max( weight_, ( *itPion )->GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight() );
    InitializePathKnowledges( entity, pion );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter destructor
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
PathAdapter::~PathAdapter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Get
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
const boost::shared_ptr< movement::Path_ABC >& PathAdapter::Get() const
{
    return path_;
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::Execute
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
void PathAdapter::Execute( TerrainPathfinder& pathfind )
{
    GET_HOOK( ExecutePathfind )( path_, pathfind );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::CleanAfterComputation
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
void PathAdapter::CleanAfterComputation()
{
    GET_HOOK( CleanPathAfterComputation )( path_ );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetLength
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
double PathAdapter::GetLength() const
{
    return GET_HOOK( PathGetLength )( path_ );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetState
// Created: MCO 2012-01-26
// -----------------------------------------------------------------------------
DEC_Path_ABC::E_State PathAdapter::GetState() const
{
    return GET_HOOK( PathGetState )( path_ );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetObjectsCost
// Created: MCO 2012-05-23
// -----------------------------------------------------------------------------
double PathAdapter::GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
{
    // default cost : outside all objects
    double rObjectCost = rCostOutsideOfAllObjects_;
    for( CIT_PathKnowledgeObjectByTypesVector itType = pathKnowledgeObjects_.begin(); itType != pathKnowledgeObjects_.end(); ++itType )
    {
        bool bInsideObjectType = false;
        const T_PathKnowledgeObjectVector& knowledges = *itType;
        for( CIT_PathKnowledgeObjectVector itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )
        {
            double rCurrentObjectCost = (*itKnowledge)->ComputeCost( from, to, nToTerrainType, nLinkTerrainType, weight_ );
            if( rCurrentObjectCost != std::numeric_limits< double >::min() )
            {
                if( !bInsideObjectType )
                {
                    rObjectCost -= (*itKnowledge)->GetCostOut();
                    bInsideObjectType = true;
                }
                if( rCurrentObjectCost < 0. ) // Impossible move (for example destroyed bridge)
                    return rCurrentObjectCost;
                rObjectCost += rCurrentObjectCost;
            }
        }
    }
    return rObjectCost;
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
    if( GET_HOOK( AvoidEnemies )( path_ ) )
    {
        const T_KnowledgeAgentVector& enemies = pion.GetKnowledgeGroup()->GetKnowledge().GetEnemies();
        for( CIT_KnowledgeAgentVector itKnowledgeAgent = enemies.begin(); itKnowledgeAgent != enemies.end(); ++itKnowledgeAgent )
        {
            const DEC_Knowledge_Agent& knowledge = **itKnowledgeAgent;
            if( knowledge.IsValid() && fuseau_.IsInside( knowledge.GetPosition() ) )
                pathKnowledgeAgents_.push_back( DEC_Path_KnowledgeAgent( knowledge, pion,
                    GET_HOOK( GetEnemyCostAtSecurityRange )( path_ ), GET_HOOK( GetEnemyCostOnContact )( path_ ) ) );
        }
    }
    // Objects
    if( GET_HOOK( AvoidObjects )( path_ ) )
    {
        T_KnowledgeObjectVector knowledgesObject;
        MIL_DangerousObjectFilter filter;
        pion.GetArmy().GetKnowledge().GetObjectsAtInteractionHeight( knowledgesObject, pion, filter );
        T_PointVector firstPointVector;
        GET_HOOK( GetFirstPoint )( path_, &AddPoint, &firstPointVector );
        for( CIT_KnowledgeObjectVector itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
        {
            const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
            if( knowledge.CanCollideWith( pion ) )
            {
                if( knowledge.IsObjectInsidePathPoint( firstPointVector, pion ) )
                {
                    if( const MIL_Object_ABC* pObject = knowledge.GetObjectKnown() )
                    {
                        TerrainData data;
                        double rMaxSpeed = entity[ "data" ].GetUserData< MIL_AgentPion >().GetRole< RoleAction_Moving >().GetSpeedWithReinforcement( data, *pObject );
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
                    const double cost = GET_HOOK( GetObjectCost )( path_, knowledge.GetType().GetID() );
                    if( cost != 0 && knowledge.GetLocalisation().GetType() != TER_Localisation::eNone )
                        pathKnowledges.push_back( boost::shared_ptr< DEC_Path_KnowledgeObject_ABC >( new DEC_Path_KnowledgeObject( knowledge, cost, GET_HOOK( GetThreshold )( path_ ) ) ) );
                }
                if( empty && pathKnowledges.size() == 1 && pathKnowledges.front()->GetCostOut() > 0 )
                    rCostOutsideOfAllObjects_ += pathKnowledges.front()->GetCostOut();
            }
        }
    }
    // Populations
    if( GET_HOOK( HandlePopulations )( path_ ) )
    {
        T_KnowledgePopulationVector knowledgesPopulation;
        pion.GetKnowledgeGroup()->GetKnowledge().GetPopulations( knowledgesPopulation );
        pathKnowledgePopulations_.reserve( knowledgesPopulation.size() );
        for( CIT_KnowledgePopulationVector it = knowledgesPopulation.begin(); it != knowledgesPopulation.end(); ++it )
            pathKnowledgePopulations_.push_back( DEC_Path_KnowledgePopulation( **it, *this, !pion.GetType().IsTerrorist() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetPopulationSecurityRange
// Created: MCO 2012-09-20
// -----------------------------------------------------------------------------
double PathAdapter::GetPopulationSecurityRange() const
{
    return GET_HOOK( GetPopulationSecurityRange )( path_ );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetCostOutsideOfPopulation
// Created: MCO 2012-09-20
// -----------------------------------------------------------------------------
double PathAdapter::GetCostOutsideOfPopulation() const
{
    return GET_HOOK( GetCostOutsideOfPopulation )( path_ );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetPopulationAttitudeCost
// Created: MCO 2012-09-20
// -----------------------------------------------------------------------------
double PathAdapter::GetPopulationAttitudeCost( unsigned int attitudeID ) const
{
    return GET_HOOK( GetPopulationAttitudeCost )( path_, attitudeID );
}

// -----------------------------------------------------------------------------
// Name: PathAdapter::GetAltitudeCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double PathAdapter::GetAltitudeCost( const MT_Vector2D& from, const MT_Vector2D& to, double rAltitudeCostPerMeter ) const
{
    const double rAltitudeFrom = altitudeData_.GetAltitude( from );
    const double rAltitudeTo   = altitudeData_.GetAltitude( to );
    const double rSquareDelta = Square( rAltitudeTo - rAltitudeFrom );
    const double rSquareGroundDistance = rSquareDelta + from.SquareDistance( to );
    if( rSquareGroundDistance > 0 && rSquareDelta / rSquareGroundDistance > squareSlope_ )
        return -1;
    if( rAltitudeCostPerMeter > 0 )
        return ( altitudeData_.GetMaxAltitude() - rAltitudeTo ) * rAltitudeCostPerMeter;
    if( rAltitudeCostPerMeter < 0 )
        return ( altitudeData_.GetMinAltitude() - rAltitudeTo ) * rAltitudeCostPerMeter;
    return 0;
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
    const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rEnemyMaximumCost ) const
{
    double rEnemyCost = 0.;
    for( CIT_PathKnowledgeAgentVector it = pathKnowledgeAgents_.begin(); it != pathKnowledgeAgents_.end(); ++it )
    {
        double rCurrentEnemyCost = it->ComputeCost( from, to, nToTerrainType, nLinkTerrainType );
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
double PathAdapter::GetPopulationsCost( const MT_Vector2D& from, const MT_Vector2D& to,
    const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rPopulationMaximumCost ) const
{
    double rCost = 0.;
    for( CIT_PathKnowledgePopulationVector it = pathKnowledgePopulations_.begin(); it != pathKnowledgePopulations_.end(); ++it )
    {
        double rCurrentCost = it->ComputeCost( from, to, nToTerrainType, nLinkTerrainType );
        if( rCurrentCost < 0. ) // Impossible move
            return rCurrentCost;
        rCost += rCurrentCost;
    }
    return std::min( rCost, rPopulationMaximumCost );
}
