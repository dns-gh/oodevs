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
#include "ASN_Messages.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/FragOrderType.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

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

namespace
{
    const FragOrderType& ReadFragOrder( xml::xistream& xis, const Resolver_ABC< FragOrderType >& fragOrders )
    {
        unsigned int id = 0;
        xis >> attribute( "id", id );
        return fragOrders.Get( id );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
ActionFragOrder::ActionFragOrder( xml::xistream& xis, Controller& controller, const Resolver_ABC< FragOrderType >& fragOrders, const Entity_ABC& entity )
    : Action_ABC ( controller, ReadFragOrder( xis, fragOrders ), entity )
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
    xos << attribute( "type", "fragorder" );
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

    ASN_MsgFragOrder asn;
    asn().oid = GetEntity().GetId();
    asn().frag_order = GetType().GetId();
    CommitTo( asn().parametres );
    asn.Send( publisher );
    Clean( asn().parametres );
}
