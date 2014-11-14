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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"

PathfindModel::PathfindModel( kernel::Controller& controller,
                              actions::ActionsModel& actionsModel,
                              const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                              const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                              const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                              const tools::Resolver_ABC< kernel::Formation_ABC >& formations,
                              const kernel::CoordinateConverter_ABC& converter,
                              const kernel::Profile_ABC& profile )
    : controller_( controller )
    , actionsModel_( actionsModel )
    , agents_( agents )
    , populations_( populations )
    , automats_( automats )
    , formations_( formations )
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
                                 const tools::Resolver_ABC< kernel::Automat_ABC >& automats,
                                 const tools::Resolver_ABC< kernel::Formation_ABC >& formations,
                                 uint32_t id )
    {
         kernel::Entity_ABC* result = populations.Find( id ) ;
         if( result )
             return *result;
         result = agents.Find( id );
         if( result )
             return *result;
         result = automats.Find( id );
         if( result )
             return *result;
         return formations.Get( id );
    }
}

void PathfindModel::Create( const sword::Pathfind& msg )
{
    if( !T_Resolver::Find( msg.id() ) )
    {
        auto& unit = GetUnit( agents_, populations_, automats_, formations_, msg.request().unit().id() );
        auto entity = new Pathfind( controller_, actionsModel_, converter_, unit, msg, false, &profile_ );
        Register( msg.id(), *entity );
        entity->Polish();
    }
}

void PathfindModel::Delete( const sword::PathfindDestruction& msg )
{
    T_Resolver::Delete( msg.id() );
}

void PathfindModel::Update( const sword::Pathfind& msg )
{
    kernel::Pathfind_ABC& pathfind = Get( msg.id() );
    pathfind.UpdateMessage( msg, GetUnit( agents_, populations_, automats_, formations_, msg.request().unit().id() ) );
    controller_.Update( pathfind );
}
