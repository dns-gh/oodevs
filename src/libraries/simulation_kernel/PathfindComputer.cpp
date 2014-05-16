// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PathfindComputer.h"
#include "PathRequest.h"
#include "Decision/DEC_Agent_PathfinderRule.h"
#include "Decision/DEC_AgentContext.h"
#include "Decision/DEC_Agent_PathClass.h"
#include "Decision/DEC_EquipmentListContext.h"
#include "Decision/DEC_PathComputer.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_PathSection.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PopulationContext.h"
#include "Decision/DEC_Population_PathfinderRule.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Network/NET_Publisher_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "protocol/ClientSenders.h"
#include <boost/assign.hpp>
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: PathfindComputer constructor
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
PathfindComputer::PathfindComputer( DEC_PathFind_Manager& manager, const TER_World& world )
    : manager_( manager )
    , world_( world )
    , ids_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer destructor
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
PathfindComputer::~PathfindComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Update
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
void PathfindComputer::Update()
{
    for( auto it = results_.begin(); it != results_.end(); )
        if( it->second->Update() )
            it = results_.erase( it );
        else
            ++it;
}

namespace
{
    std::vector< MT_Vector2D > GetPositions( const sword::PathfindRequest& message, const TER_World& world )
    {
        const auto& positions = message.positions();
        std::vector< MT_Vector2D > points;
        for( auto i = 0; i < positions.size(); ++i )
        {
            MT_Vector2D point;
            const auto& position = positions.Get( i );
            world.MosToSimMgrsCoord( position.latitude(), position.longitude(), point );
            points.push_back( point );
        }
        return points;
    }
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
void PathfindComputer::Compute( MIL_AgentPion& pion, const sword::PathfindRequest& message,
                                unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic )
{
    const auto points = GetPositions( message, world_ );
    if( points.empty() )
        throw MASA_EXCEPTION( "invalid empty path point list" );
    const auto computer = boost::make_shared< DEC_PathComputer >( pion.GetID() );
    const auto& pathClass = DEC_Agent_PathClass::GetPathClass( DEC_PathType::movement_, pion );
    const auto context = boost::make_shared< DEC_AgentContext >( pion, pathClass, points );
    for( auto it = points.begin(); it != points.end() - 1; ++it )
    {
        std::unique_ptr< TerrainRule_ABC > rule( new DEC_Agent_PathfinderRule( context, *it, *(it + 1) ) );
        computer->RegisterPathSection( *new DEC_PathSection( *computer, std::move( rule ), *it, *(it + 1), false, false ) );
    }
    Compute( computer, message, ctx, clientId, magic );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
void PathfindComputer::Compute( const MIL_Population& population, const sword::PathfindRequest& message,
                                unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic )
{
    const auto points = GetPositions( message, world_ );
    if( points.empty() )
        throw MASA_EXCEPTION( "invalid empty path point list" );
    const auto computer = boost::make_shared< DEC_PathComputer >( population.GetID() );
    const auto context = boost::make_shared< DEC_PopulationContext >( population, points );
    for( auto it = points.begin(); it != points.end() - 1; ++it )
    {
        std::unique_ptr< TerrainRule_ABC > rule( new DEC_Population_PathfinderRule( context ) );
        computer->RegisterPathSection( *new DEC_PathSection( *computer, std::move( rule ), *it, *(it + 1), false, false ) );
    }
    Compute( computer, message, ctx, clientId, magic );
}

void PathfindComputer::Compute( const std::vector< const PHY_ComposanteTypePion* >& equipments,
                                const sword::PathfindRequest& message,
                                unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic )
{
    const auto points = GetPositions( message, world_ );
    if( points.empty() )
        throw MASA_EXCEPTION( "invalid empty path point list" );
    const auto computer = boost::make_shared< DEC_PathComputer >( 0 );
    const auto context = boost::make_shared< DEC_EquipmentListContext >( equipments );
    for( auto it = points.begin(); it != points.end() - 1; ++it )
    {
        std::unique_ptr< TerrainRule_ABC > rule( new DEC_Agent_PathfinderRule( context, *it, *(it + 1) ) );
        computer->RegisterPathSection( *new DEC_PathSection( *computer, std::move( rule ), *it, *(it + 1), false, false ) );
    }
    Compute( computer, message, ctx, clientId, magic );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
void PathfindComputer::Compute( const boost::shared_ptr< DEC_PathComputer >& computer, const sword::PathfindRequest& message,
                                unsigned int ctx, unsigned int clientId, const boost::optional< uint32_t >& magic )
{
    const uint32_t id = ++ids_;
    results_[ id ] = boost::make_shared< PathRequest >( computer, message, ctx, clientId, id, magic );
    manager_.StartCompute( computer, message.ignore_dynamic_objects() );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Destroy
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathfindComputer::Destroy( uint32_t pathfind )
{
    if( !results_.erase( pathfind ) )
        return false;
    client::PathfindDestruction msg;
    msg().mutable_id()->set_id( pathfind );
    msg.Send( NET_Publisher_ABC::Publisher() );
    return true;
}
