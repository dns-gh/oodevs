// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MissionController.h"

#include "ActionManager.h"
#include "PopulationFactory_ABC.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "CheckPoints/SerializationTools.h"
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MissionController )

// -----------------------------------------------------------------------------
// Name: MissionController constructor
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
MissionController::MissionController( ActionManager& actions )
    : actions_( actions )
    , loaded_ ( false )
    , counter_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionController destructor
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
MissionController::~MissionController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionController::load
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
template< typename T >
void MissionController::load( T& file, const unsigned int )
{
    file >> boost::serialization::base_object< MissionController_ABC >( *this );
    file >> missions_;
    file >> counter_;
    loaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: MissionController::save
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
template< typename T >
void MissionController::save( T& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MissionController_ABC >( *this );
    file << missions_;
    file << counter_;
}

template< typename Archive >
void save_construct_data( Archive& ar, const MissionController* ptr, const unsigned int /*version*/ )
{
    const ActionManager* actions = &ptr->actions_;
    ar << actions;
}

template< typename Archive >
void load_construct_data( Archive& ar, MissionController* ptr, const unsigned int /*version*/ )
{
    ActionManager* actions;
    ar >> actions;
    ::new( ptr ) MissionController( *actions );
}

// -----------------------------------------------------------------------------
// Name: MissionController::Start
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MissionController::Start( const boost::shared_ptr< MIL_Mission_ABC >& mission )
{
    mission->Start( mission, actions_ );
    if( !mission->GetOwnerId() )
        return;
    Stop( mission );
    missions_[ mission->GetOwnerId() ] = mission;
}

// -----------------------------------------------------------------------------
// Name: MissionController::Stop
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MissionController::Stop( const boost::shared_ptr< MIL_Mission_ABC >& mission )
{
    if( !mission )
        return;
    missions_.erase( mission->GetOwnerId() );
}

// -----------------------------------------------------------------------------
// Name: MissionController::Initialize
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MissionController::Initialize( const tools::Resolver< MIL_AgentPion >& resolver, const PopulationFactory_ABC& populationFactory )
{
    BOOST_FOREACH( const auto& mission, missions_ )
    {
        if( MIL_AgentPion* pion = resolver.Find( mission.first ) )
            pion->GetOrderManager().ReplaceMission( mission.second );
        else if( MIL_Population* population = populationFactory.Find( mission.first ) )
            population->GetOrderManager().ReplaceMission( mission.second );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionController::SendFullState
// Created: LGY 2011-06-30
// -----------------------------------------------------------------------------
void MissionController::SendFullState()
{
    if( !loaded_ )
        return;
    BOOST_FOREACH( const auto& mission, missions_ )
        if( mission.second )
            mission.second->Send( actions_ );
}

// -----------------------------------------------------------------------------
// Name: MissionController::AcquireId
// Created: BAX 2013-08-27
// -----------------------------------------------------------------------------
uint32_t MissionController::AcquireId()
{
    return counter_++;
}

ActionManager& MissionController::GetActionManager() const
{
    return actions_;
}
