// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "FragOrder.h"
#include "ActionTasker.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/FragOrderType.h"

#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"

#include <xeumeuleu/xml.hpp>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: FragOrder constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
FragOrder::FragOrder( const kernel::FragOrderType* fragOrder, kernel::Controller& controller, bool registered )
    : Action_ABC( controller, fragOrder )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrder destructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
FragOrder::~FragOrder()
{
    if( registered_ )
        controller_.Delete( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: FragOrder::Polish
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void FragOrder::Polish()
{
    if( registered_ )
        controller_.Create( *static_cast< Action_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: FragOrder::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void FragOrder::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType() ? GetType()->GetId() : 0 )
        << xml::attribute( "type", "fragorder" );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: FragOrder::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void FragOrder::Publish( Publisher_ABC& publisher, int context ) const
{
    simulation::FragOrder message;
    const ActionTasker& tasker = Get< ActionTasker >();
    const std::string typeName = tasker.GetTypeName();
    const unsigned int id = tasker.GetId();
    if( typeName == kernel::Agent_ABC::typeName_ )
        message().mutable_tasker()->mutable_unit()->set_id( id );
    else if( typeName == kernel::Automat_ABC::typeName_ )
        message().mutable_tasker()->mutable_automat()->set_id( id );
    else if( typeName == kernel::Population_ABC::typeName_ )
        message().mutable_tasker()->mutable_crowd()->set_id( id );
    message().mutable_type()->set_id( GetType() ? GetType()->GetId() : 0 );
    CommitTo( *message().mutable_parameters() );
    message().set_name( GetName().toStdString() );
    message.Send( publisher, context );
}
