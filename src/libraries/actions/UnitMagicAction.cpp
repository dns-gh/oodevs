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
#include "ActionTasker.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"

#include "ENT/ENT_Tr.h"

#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"

#include <tools/Iterator.h>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::UnitMagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /* = true*/)
    : Action_ABC ( controller, &magic )
    , controller_( controller )
    , registered_( registered )
{
    Rename( ENT_Tr::ConvertFromUnitMagicActionType( ENT_Tr::ConvertToUnitMagicActionType( magic.GetName() ) ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::~MagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
UnitMagicAction::~UnitMagicAction()
{
    if( registered_ )
        controller_.Delete( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UnitMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void UnitMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType()->GetName() )
        << xml::attribute( "type", "magicunit" );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicAction::Publish
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void UnitMagicAction::Publish( Publisher_ABC& publisher, int context ) const
{
    const ActionTasker& tasker = Get< ActionTasker >();
    const std::string typeName = tasker.GetTypeName();
    const unsigned int id = tasker.GetId();
    sword::UnitMagicAction_Type type = ( sword::UnitMagicAction_Type ) GetType()->GetId();
    simulation::UnitMagicAction message;
    if( typeName == kernel::Agent_ABC::typeName_ )
        message().mutable_tasker()->mutable_unit()->set_id( id );
    else if( typeName == kernel::Automat_ABC::typeName_ )
        message().mutable_tasker()->mutable_automat()->set_id( id );
    else if( typeName == kernel::Formation_ABC::typeName_ )
        message().mutable_tasker()->mutable_formation()->set_id( id );
    else if( typeName == kernel::Population_ABC::typeName_ )
        message().mutable_tasker()->mutable_crowd()->set_id( id );
    else if( typeName == kernel::Team_ABC::typeName_ )
        message().mutable_tasker()->mutable_party()->set_id( id );
    else if( typeName == kernel::Inhabitant_ABC::typeName_ )
        message().mutable_tasker()->mutable_population()->set_id( id );
    message().set_type( type );
    CommitTo( *message().mutable_parameters() );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
    const kernel::Entity_ABC* entity = tasker.GetTasker();
    if( type == sword::UnitMagicAction_Type_move_to && entity )
        const_cast< kernel::Entity_ABC* >( entity )->Update( message() );
}
