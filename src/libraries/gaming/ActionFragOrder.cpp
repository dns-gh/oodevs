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
#include "clients_kernel/Controller.h"
#include "clients_kernel/FragOrderType.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionFragOrder constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
ActionFragOrder::ActionFragOrder( const kernel::Entity_ABC& entity, const kernel::FragOrderType& fragOrder, kernel::Controller& controller )
    : Action_ABC( controller, fragOrder, entity )
    , controller_( controller )
{
    controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder destructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
ActionFragOrder::~ActionFragOrder()
{
    controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionFragOrder::Serialize( xml::xostream& xos ) const
{
    xos << start( "fragorder" );
    Action_ABC::Serialize( xos );
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder::Publish
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionFragOrder::Publish( Publisher_ABC& publisher ) const
{
    // $$$$ SBO 2007-05-21: TODO
}
