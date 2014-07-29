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

PathfindModel::PathfindModel( kernel::Controller& controller,
                              const kernel::ActionController& actions,
                              const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                              const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                              const kernel::CoordinateConverter_ABC& converter )
    : controller_( controller )
    , actions_( actions )
    , agents_( agents )
    , populations_( populations )
    , converter_( converter )
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

void PathfindModel::Create( const sword::Pathfind& msg )
{
    if( !T_Resolver::Find( msg.id() ) )
    {
        auto entity = new Pathfind( controller_, actions_, converter_, agents_, populations_, msg, false );
        Register( msg.id(), *entity );
        entity->Polish();
    }
}

void PathfindModel::Delete( const sword::PathfindDestruction& msg )
{
    T_Resolver::Delete( msg.id() );
}
