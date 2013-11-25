// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"

// -----------------------------------------------------------------------------
// Name: EntityMission constructor
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
template< typename T >
EntityMission< T >::EntityMission( const kernel::Entity_ABC* entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered /*= true*/ )
    : Mission( entity, mission, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityMission constructor
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
template< typename T >
EntityMission< T >::EntityMission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, bool stub )
    : Mission( xis, controller, missions, entity, stub )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityMission destructor
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
template< typename T >
EntityMission< T >::~EntityMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityMission::Publish
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
template< typename T >
void EntityMission< T >::Publish( Publisher_ABC& publisher, int context ) const
{
    T message;
    message().mutable_tasker()->set_id( entityId_ );
    message().mutable_type()->set_id( GetType().GetId());
    CommitTo( *message().mutable_parameters() );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
}
