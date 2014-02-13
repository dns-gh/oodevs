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
EntityMission< T >::EntityMission( const kernel::MissionType* mission, kernel::Controller& controller, bool registered /*= true*/ )
    : Mission( mission, controller, registered )
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
    message().mutable_tasker()->set_id( Get< ActionTasker >().GetId() );
    message().mutable_type()->set_id( GetType() ? GetType()->GetId() : 0 );
    CommitTo( *message().mutable_parameters() );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
}
