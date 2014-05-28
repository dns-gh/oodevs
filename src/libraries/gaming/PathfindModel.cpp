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

PathfindModel::PathfindModel( kernel::Controller& controller )
    : controller_( controller )
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
        Register( msg.id(), *new Pathfind( controller_, msg ) );
}

void PathfindModel::Delete( const sword::PathfindDestruction& msg )
{
    T_Resolver::Delete( msg.id() );
}
