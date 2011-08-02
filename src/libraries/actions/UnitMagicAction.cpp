// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "UnitMagicAction.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/iterator.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::UnitMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, const QString& name, bool registered /* = true*/)
    : ActionWithTarget_ABC ( controller, magic, entity )
    , controller_( controller )
    , registered_( registered )
{
    Rename( name );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::UnitMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity, const QString& name )
    : ActionWithTarget_ABC( xis, controller, magic, entity )
    , controller_( controller )
    , registered_( true )
{
    Rename( name );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::~MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::~UnitMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UnitMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UnitMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType().GetName() )
        << xml::attribute( "type", "magicunit" );
    ActionWithTarget_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Publish
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void UnitMagicAction::Publish( Publisher_ABC& publisher, int context ) const
{
    sword::UnitMagicAction_Type type = ( sword::UnitMagicAction_Type ) GetType().GetId();
    simulation::UnitMagicAction message;
    const kernel::Entity_ABC& entity = GetEntity();
    if( dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        message().mutable_tasker()->mutable_unit()->set_id( entity.GetId() );
    else if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        message().mutable_tasker()->mutable_automat()->set_id( entity.GetId() );
    else if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        message().mutable_tasker()->mutable_formation()->set_id( entity.GetId() );
    else if( dynamic_cast< const kernel::Population_ABC* >( &entity ) )
        message().mutable_tasker()->mutable_crowd()->set_id( entity.GetId() );
    else if( dynamic_cast< const kernel::Team_ABC* >( &entity ) )
        message().mutable_tasker()->mutable_party()->set_id( entity.GetId() );
    else if( dynamic_cast< const kernel::Inhabitant_ABC* >( &entity ) )
        message().mutable_tasker()->mutable_population()->set_id( entity.GetId() );
    else
        throw std::runtime_error( "Unknown tasker" );
    message().set_type( type );
    CommitTo( *message().mutable_parameters() );
    message.Send( publisher, context );
    if( type == sword::UnitMagicAction_Type_move_to )
        const_cast< kernel::Entity_ABC& >( GetEntity() ).Update( message() );
    message().Clear();
}
