// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TrafficabilityInteractionEventListener.h"
#include "RoleAction_Moving.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "simulation_terrain/TER_World.h"
#include <core/Model.h>
#include <core/ModelVisitor_ABC.h>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: TrafficabilityInteractionEventListener constructor
// Created: MMC 2013-04-16
// -----------------------------------------------------------------------------
TrafficabilityInteractionEventListener::TrafficabilityInteractionEventListener( const core::Model& model, core::Facade& facade )
    : EventListenerBase( facade, "trafficability interaction" )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityInteractionEventListener destructor
// Created: MMC 2013-04-16
// -----------------------------------------------------------------------------
TrafficabilityInteractionEventListener::~TrafficabilityInteractionEventListener()
{
    // NOTHING
}

namespace
{
    struct ElementVisitor : public core::ModelVisitor_ABC
    {
        explicit ElementVisitor( MIL_AgentPion& pion ) : pion_( pion ){}
        virtual void Visit( int64_t /*value*/ ) {}
        virtual void Visit( uint64_t /*value*/ ) {}
        virtual void Visit( double /*value*/ ) {}
        virtual void Visit( const std::string& /*value*/ ) {}
        virtual void Visit( const std::string& /*key*/, const core::Model& /*child*/ ) {}
        virtual void Visit( std::size_t /*key*/, const core::Model& /*child*/ ) {}
        virtual void Visit( const core::Model& element )
        {
           MIL_AgentPion& other = element[ "data" ].GetUserData< MIL_AgentPion >();
           pion_.InteractWithTraffic( other );
        }
        virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& /*data*/ ) {}
    private:
        MIL_AgentPion& pion_;
    };
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityInteractionEventListener::Notify
// Created: MMC 2013-04-16
// -----------------------------------------------------------------------------
void TrafficabilityInteractionEventListener::Notify( const core::Model& event )
{
    const unsigned int identifier = event[ "entity" ];
    const core::Model& entity = model_[ "entities" ][ identifier ];
    ElementVisitor visitor( entity[ "data" ].GetUserData< MIL_AgentPion >() );
    event[ "interactions" ].Accept( visitor );
}
