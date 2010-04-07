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
#include "protocol/simulationsenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include <xeumeuleu/xml.h>

using namespace Common;
using namespace kernel;
using namespace actions;

using namespace MsgsClientToSim;

namespace
{
    const OrderType& ResolveType( xml::xistream& xis, const tools::Resolver_ABC< FragOrderType >& missions, const Entity_ABC& entity )
    {
        const unsigned int id = xml::attribute< unsigned int >( xis, "id", 0 );
        const std::string name = xml::attribute< std::string >( xis, "name", "" );
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
FragOrder::FragOrder( const Entity_ABC& entity, const FragOrderType& fragOrder, Controller& controller, bool registered )
    : Action_ABC ( controller, fragOrder, entity )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrder constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
FragOrder::FragOrder( xml::xistream& xis, Controller& controller, const tools::Resolver_ABC< FragOrderType >& fragOrders, const Entity_ABC& entity )
    : Action_ABC ( xis, controller, ResolveType( xis, fragOrders, entity ), entity )
    , controller_( controller )
    , registered_( true )
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
    xos << xml::attribute( "type", "fragorder" );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: FragOrder::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void FragOrder::Publish( Publisher_ABC& publisher ) const
{
    simulation::FragOrder message;
    message().set_oid( GetEntity().GetId() );
    message().set_frag_order( GetType().GetId() );
    CommitTo( *message().mutable_parametres() );
    message.Send( publisher );
    message().Clear();
}
// -----------------------------------------------------------------------------
// Name: FragOrder::Accept
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void FragOrder::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

