// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "MagicAction.h"

#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

#include "ENT/ENT_Tr.h"

#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: MagicAction constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicAction::MagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /* = true*/ )
    : Action_ABC ( controller, &magic )
    , controller_( controller )
    , registered_( registered )
{
    Rename( ENT_Tr::ConvertFromMagicActionType( ENT_Tr::ConvertToMagicActionType( magic.GetName() ) ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MagicAction constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicAction::MagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic )
    : Action_ABC( xis, controller, &magic )
    , controller_( controller )
    , registered_( true )
{
    Rename( ENT_Tr::ConvertFromMagicActionType( ENT_Tr::ConvertToMagicActionType( magic.GetName() ) ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MagicAction destructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicAction::~MagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: MagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void MagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: MagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void MagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType()->GetName() )
        << xml::attribute( "type", "magic" );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: MagicAction::Publish
// Created: JSR 2010-04-23
// -----------------------------------------------------------------------------
void MagicAction::Publish( Publisher_ABC& publisher, int context ) const
{
    sword::MagicAction_Type type = ( sword::MagicAction_Type ) GetType()->GetId();
    simulation::MagicAction message;
    message().set_type( type );
    CommitTo( *message().mutable_parameters() );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
}
