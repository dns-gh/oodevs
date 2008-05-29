// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionFragOrder.h"
#include "SimulationMessages.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/FragOrderType.h"
#include "xeumeuleu/xml.h"

using namespace kernel;

namespace
{
    const OrderType& ResolveType( xml::xistream& xis, const Resolver_ABC< FragOrderType >& missions, const Entity_ABC& entity )
    {
        const unsigned int id = xml::attribute< unsigned int >( xis, "id", 0 );
        const std::string name = xml::attribute< std::string >( xis, "name", "" );
        const OrderType* type = missions.Find( id );
        if( !type )
            throw std::exception( tools::translate( "ActionFragOrder", "Entity '%1' (id: %2) cannot execute fragmentary order '%3' (id: %4)" )
                                    .arg( entity.GetName() ).arg( entity.GetId() ).arg( name.c_str() ).arg( id ) );
        return *type;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
ActionFragOrder::ActionFragOrder( const Entity_ABC& entity, const FragOrderType& fragOrder, Controller& controller, bool registered )
    : Action_ABC ( controller, fragOrder, entity )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionFragOrder::ActionFragOrder( xml::xistream& xis, Controller& controller, const Resolver_ABC< FragOrderType >& fragOrders, const Entity_ABC& entity )
    : Action_ABC ( xis, controller, ResolveType( xis, fragOrders, entity ), entity )
    , controller_( controller )
    , registered_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder destructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
ActionFragOrder::~ActionFragOrder()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder::Polish
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void ActionFragOrder::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionFragOrder::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", "fragorder" );
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionFragOrder::Publish( Publisher_ABC& publisher ) const
{
    // $$$$ SBO 2007-06-26: check profile! CanBeOrdered
    // $$$$ SBO 2007-06-26: check engaged/disengaged

    simulation::FragOrder asn;
    asn().oid = GetEntity().GetId();
    asn().frag_order = GetType().GetId();
    CommitTo( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}
