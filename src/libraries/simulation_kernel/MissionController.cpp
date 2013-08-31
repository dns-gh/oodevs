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
#include "AgentFactory_ABC.h"
#include "PopulationFactory_ABC.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "CheckPoints/SerializationTools.h"
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MissionController )

// -----------------------------------------------------------------------------
// Name: MissionController constructor
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
MissionController::MissionController()
    : loaded_ ( false )
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
void MissionController::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MissionController_ABC >( *this )
         >> missions_
         >> counter_;
    loaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: MissionController::save
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MissionController::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MissionController_ABC >( *this )
         << missions_
         << counter_;
}

// -----------------------------------------------------------------------------
// Name: MissionController::Start
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MissionController::Start( boost::shared_ptr< MIL_Mission_ABC > mission )
{
    if( mission.get() )
    {
        unsigned int id = mission->GetOwnerId();
        if( id != 0 )
        {
            Stop( mission );
            missions_[ id ] = mission ;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MissionController::Stop
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MissionController::Stop( boost::shared_ptr< MIL_Mission_ABC > mission )
{
    if( mission.get() )
    {
        unsigned int id = mission->GetOwnerId();
        if( id != 0 )
            missions_.erase( id );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionController::Initialize
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MissionController::Initialize( tools::Resolver< MIL_AgentPion >& resolver, PopulationFactory_ABC& populationFactory )
{
    BOOST_FOREACH( const T_Missions::value_type& mission, missions_ )
        if( MIL_AgentPion* pion = resolver.Find( mission.first ) )
            pion->GetOrderManager().ReplaceMission( mission.second );
        else if( MIL_Population* population = populationFactory.Find( mission.first ) )
            population->GetOrderManager().ReplaceMission( mission.second );
}

// -----------------------------------------------------------------------------
// Name: MissionController::SendFullState
// Created: LGY 2011-06-30
// -----------------------------------------------------------------------------
void MissionController::SendFullState()
{
    if( loaded_ )
        BOOST_FOREACH( const T_Missions::value_type& mission, missions_ )
            if( mission.second )
                 mission.second->Send();
}

// -----------------------------------------------------------------------------
// Name: MissionController::AcquireId
// Created: BAX 2013-08-27
// -----------------------------------------------------------------------------
uint32_t MissionController::AcquireId()
{
    return counter_++;
}
