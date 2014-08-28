// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PathfindModel.h"
#include "Pathfind.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"

PathfindModel::PathfindModel( kernel::Controller& controller,
                              actions::ActionsModel& actionsModel,
                              const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                              const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                              const kernel::CoordinateConverter_ABC& converter,
                              const kernel::Profile_ABC& profile )
    : controller_( controller )
    , actionsModel_( actionsModel )
    , agents_( agents )
    , populations_( populations )
    , converter_( converter )
    , profile_( profile )
{
    controller_.Register( *this );
}

PathfindModel::~PathfindModel()
{
    controller_.Unregister( *this );
}

void PathfindModel::Purge()
{
    tools::Resolver< kernel::Pathfind_ABC >::DeleteAll();
}

namespace
{
    kernel::Entity_ABC& GetUnit( const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                                 const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                                 uint32_t id )
    {
        if( auto popu = populations.Find( id ) )
            return *popu;
        return agents.Get( id );
    }
}

void PathfindModel::Create( const sword::Pathfind& msg )
{
    if( !T_Resolver::Find( msg.id() ) )
    {
        auto& unit = GetUnit( agents_, populations_, msg.request().unit().id() );
        auto entity = new Pathfind( controller_, actionsModel_, converter_, unit, msg, false,
                                    [&]( const kernel::Pathfind_ABC& pathfind ){ return profile_.CanBeOrdered( pathfind.GetUnit() ); } );
        Register( msg.id(), *entity );
        entity->Polish();
    }
}

void PathfindModel::Delete( const sword::PathfindDestruction& msg )
{
    T_Resolver::Delete( msg.id() );
}
