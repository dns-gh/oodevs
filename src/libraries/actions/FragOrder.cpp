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
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;

namespace
{
    const OrderType& ResolveType( xml::xistream& xis, const tools::Resolver_ABC< kernel::FragOrderType >& missions, const Entity_ABC& entity )
    {
        const unsigned int id = xis.attribute< unsigned int >( "id", 0 );
        const std::string name = xis.attribute< std::string >( "name", "" );
        const OrderType* type = missions.Find( id );
        if( !type )
            throw std::exception( tools::translate( "FragOrder", "Entity '%1' (id: %2) cannot execute fragmentary order '%3' (id: %4)" )
                                    .arg( entity.GetName() ).arg( entity.GetId() ).arg( name.c_str() ).arg( id ) );
        return *type;
    }
}

// -----------------------------------------------------------------------------
// Name: FragOrder constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
FragOrder::FragOrder( const Entity_ABC& entity, const kernel::FragOrderType& fragOrder, Controller& controller, bool registered )
    : ActionWithTarget_ABC( controller, fragOrder, entity )
    , controller_         ( controller )
    , registered_         ( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrder constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
FragOrder::FragOrder( xml::xistream& xis, Controller& controller, const tools::Resolver_ABC< kernel::FragOrderType >& fragOrders, const Entity_ABC& entity )
    : ActionWithTarget_ABC( xis, controller, ResolveType( xis, fragOrders, entity ), entity )
    , controller_         ( controller )
    , registered_         ( true )
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
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: FragOrder::Polish
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void FragOrder::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: FragOrder::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void FragOrder::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType().GetId() )
        << xml::attribute( "type", "fragorder" );
    ActionWithTarget_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: FragOrder::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void FragOrder::Publish( Publisher_ABC& publisher, int ) const
{
    simulation::FragOrder message;
    if( GetEntity().GetTypeName() == "automat" )
        message().mutable_tasker()->mutable_automat()->set_id( GetEntity().GetId() );
    else if( GetEntity().GetTypeName() == "crowd" )
        message().mutable_tasker()->mutable_crowd()->set_id( GetEntity().GetId() );
    else if( GetEntity().GetTypeName() == "agent" )
        message().mutable_tasker()->mutable_unit()->set_id( GetEntity().GetId() );
    message().mutable_type()->set_id( GetType().GetId() );
    CommitTo( *message().mutable_parameters() );
    message.Send( publisher );
}
