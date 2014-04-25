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
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PathResult.h"
#include "Decision/DEC_Population_Path.h"
#include "Decision/DEC_PathFind_Manager.h"
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
PathfindComputer::PathfindComputer( DEC_PathFind_Manager& pathfindManager, const TER_World& world )
    : pathfindManager_( pathfindManager )
    , world_          ( world )
    , ids_            ( 0 )
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
    {
        if( it->second->Update() )
            it = results_.erase( it );
        else
            ++it;
    }
}

namespace
{
    std::vector< boost::shared_ptr< MT_Vector2D > > GetPositions( const sword::PathfindRequest& message, const TER_World& world )
    {
        const auto& positions = message.positions();
        std::vector< boost::shared_ptr< MT_Vector2D > > points;
        for( auto i = 0; i < positions.size(); ++i )
        {
            auto point = boost::make_shared< MT_Vector2D >();
            const auto& position = positions.Get( i );
            world.MosToSimMgrsCoord( position.latitude(), position.longitude(), *point );
            points.push_back( point );
        }
        return points;
    }
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
uint32_t PathfindComputer::Compute( MIL_AgentPion& pion, const sword::PathfindRequest& message,
                                    unsigned int ctx, unsigned int clientId, bool store )
{
    return Compute( boost::make_shared< DEC_Agent_Path >( pion, GetPositions( message, world_ ), DEC_PathType::movement_ ), message,
        ctx, clientId, store );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
uint32_t PathfindComputer::Compute( const MIL_Population& population, const sword::PathfindRequest& message,
                                    unsigned int ctx, unsigned int clientId, bool store )
{
    return Compute( boost::make_shared< DEC_Population_Path >( population, GetPositions( message, world_ ) ), message,
        ctx, clientId, store );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
uint32_t PathfindComputer::Compute( boost::shared_ptr< DEC_PathResult > path, const sword::PathfindRequest& request,
                                    unsigned int ctx, unsigned int clientId, bool store )
{
    uint32_t id = ++ids_;
    results_[ id ] = boost::make_shared< PathRequest >( path, request, ctx, clientId, id, store );
    pathfindManager_.StartCompute( path, request.ignore_dynamic_objects() );
    return id;
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Destroy
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathfindComputer::Destroy( unsigned int pathfind )
{
    return results_.erase( pathfind ) > 0;
}
