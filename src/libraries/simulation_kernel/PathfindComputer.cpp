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
#include "MIL_AgentServer.h"
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
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Network/NET_Publisher_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "protocol/ClientSenders.h"
#include "protocol/MessageParameters.h"

#include <boost/assign.hpp>
#include <boost/make_shared.hpp>
#include <boost/serialization/shared_ptr.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PathfindComputer )

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
    results_[ id ] = boost::make_shared< PathRequest >( computer, ctx, clientId, id, message.unit().id(), magic );
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
    msg().set_id( pathfind );
    msg.Send( NET_Publisher_ABC::Publisher() );
    return true;
}

void PathfindComputer::SendStateToNewClient()
{
    for( auto it = results_.begin(); it != results_.end(); ++it )
        it->second->SendStateToNewClient();
}

template< typename Archive >
void PathfindComputer::load( Archive& ar, const unsigned int /*version*/ )
{
    ar  >> ids_
        >> results_;
}

template< typename Archive >
void PathfindComputer::save( Archive& ar, const unsigned int /*version*/ ) const
{
    T_Results entities;
    for( auto it = results_.begin(); it != results_.end(); ++it )
        if( it->second->IsPublished() )
            entities.insert( *it );
    ar  << ids_
        << entities;
}

template< typename Archive >
void save_construct_data( Archive&, const PathfindComputer*, const unsigned int /*version*/ )
{
    // NOTHING
}

template< typename Archive >
void load_construct_data( Archive&, PathfindComputer* ptr, const unsigned int /*version*/ )
{
    auto& paths = MIL_AgentServer::GetWorkspace().GetPathFindManager();
    ::new( ptr ) PathfindComputer( paths, TER_World::GetWorld() );
}

void PathfindComputer::OnPathfindRequest( const sword::PathfindRequest& message, unsigned int nCtx, unsigned int clientId )
{
    try
    {
        const auto& entities = MIL_AgentServer::GetWorkspace().GetEntities();
        protocol::Check( message.has_unit() || message.equipment_types().size() > 0, "must have either a unit or equipment types" );
        const auto& positions = message.positions();
        protocol::Check( positions.size() > 1, "must have at least two points" );
        if( message.equipment_types().size() )
        {
            std::vector< const PHY_ComposanteTypePion* > equipments;
            for( auto it = message.equipment_types().begin(); it != message.equipment_types().end(); ++it )
            {
                const auto type = PHY_ComposanteTypePion::Find( it->id() );
                protocol::Check( type, "invalid dotation type identifier" );
                equipments.push_back( type );
            }
            Compute( equipments, message, nCtx, clientId, boost::none );
        }
        else
        {
            const auto id = message.unit().id();
            if( auto pion = entities.FindAgentPion( id ) )
                Compute( *pion, message, nCtx, clientId, boost::none );
            else if( auto population = entities.FindPopulation( id ) )
                Compute( *population, message, nCtx, clientId, boost::none );
            else
                protocol::Check( false, "invalid crowd or unit identifier" );
        }
    }
    catch( const tools::Exception& e )
    {
        client::ComputePathfindAck ack;
        ack().set_error_code( sword::ComputePathfindAck::error_invalid_parameter );
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
        ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
    }
}

bool PathfindComputer::OnReceivePathfindCreation( const sword::MagicAction& message,
                                                   unsigned int ctx, unsigned int clientId, uint32_t magicId )
{
    const auto& entities = MIL_AgentServer::GetWorkspace().GetEntities();
    const auto& params = message.parameters();
    protocol::CheckCount( params, 1 );
    const auto& value = message.parameters().elem( 0 ).value( 0 );
    protocol::Check( value.has_pathfind_request(), "invalid parameters" );
    const auto& request = value.pathfind_request();
    protocol::Check( request.ignore_dynamic_objects(), "invalid ignore dynamic objects not set" );
    const auto& positions = request.positions();
    protocol::Check( positions.size() > 1, "must have at least two points" );
    protocol::Check( request.has_unit() || request.equipment_types().size() > 0, "must have either a unit or equipment types" );
    if( request.equipment_types().size() )
    {
        std::vector< const PHY_ComposanteTypePion* > equipments;
        for( auto it = request.equipment_types().begin(); it != request.equipment_types().end(); ++it )
        {
            const auto type = PHY_ComposanteTypePion::Find( it->id() );
            protocol::Check( type, "invalid dotation type identifier" );
            equipments.push_back( type );
        }
        Compute( equipments, request, ctx, clientId, magicId );
    }
    else
    {
        const unsigned int id = request.unit().id();
        if( auto pion = entities.FindAgentPion( id ) )
            Compute( *pion, request, ctx, clientId, magicId );
        else if( auto population = entities.FindPopulation( id ) )
            Compute( *population, request, ctx, clientId, magicId );
        else
            protocol::Check( false, "invalid crowd or unit identifier" );
    }
    return true;
}

void PathfindComputer::OnReceivePathfindDestruction( const sword::MagicAction& message, sword::MagicActionAck& ack )
{
    const auto& params = message.parameters();
    protocol::CheckCount( params, 1 );
    const auto id = protocol::GetIdentifier( params, 0 );
    if( Destroy( id ) )
        return;
    ack.set_error_code( sword::MagicActionAck::error_invalid_parameter );
    ack.set_error_msg( "invalid pathfind identifier");
}
