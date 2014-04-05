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
#include "Decision/DEC_Agent_Path.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PathResult.h"
#include "Decision/DEC_Population_Path.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/assign.hpp>
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: PathfindComputer constructor
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
PathfindComputer::PathfindComputer( DEC_PathFind_Manager& pathfindManager )
    : pathfindManager_( pathfindManager )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionController destructor
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
        if( PathComputed( it->first, it->second ) )
            it = results_.erase( it );
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
void PathfindComputer::Compute( MIL_AgentPion& pion, const std::vector< boost::shared_ptr< MT_Vector2D > >& positions,
                                unsigned int nCtx, unsigned int clientId )
{
    Compute( boost::make_shared< DEC_Agent_Path >( pion, positions, DEC_PathType::movement_ ), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
void PathfindComputer::Compute( const MIL_Population& population, const std::vector< boost::shared_ptr< MT_Vector2D > >& positions,
                                unsigned int nCtx, unsigned int clientId )
{
   Compute( boost::make_shared< DEC_Population_Path >( population, positions ), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::Compute
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
void PathfindComputer::Compute( const boost::shared_ptr< DEC_PathResult >& path,
                                unsigned int nCtx, unsigned int clientId )
{
    path->AddRef();
    results_[ clientId ] = std::make_pair( nCtx, path );
    pathfindManager_.StartCompute( path );
}

// -----------------------------------------------------------------------------
// Name: PathfindComputer::PathComputed
// Created: LGY 2014-03-03
// -----------------------------------------------------------------------------
bool PathfindComputer::PathComputed( unsigned int clientId, const T_Result& content )
{
    auto path = content.second;
    DEC_PathResult::E_State nPathState = path->GetState();
    if( nPathState == DEC_Path_ABC::eComputing )
        return false;

    client::PathfindRequestAck ack;
    if( nPathState == DEC_Path_ABC::eInvalid || nPathState == DEC_Path_ABC::eImpossible )
        ack().set_error_code( sword::PathfindRequestAck::error_path_invalid );
    else
    {
        ack().set_error_code( sword::PathfindRequestAck::no_error );
        path->Serialize( *ack().mutable_path() );
    }
    path->DecRef();
    ack.Send( NET_Publisher_ABC::Publisher(), content.first, clientId );
    return true;
}
