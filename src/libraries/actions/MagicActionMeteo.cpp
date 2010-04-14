// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "MagicActionMeteo.h"
#include "protocol/simulationsenders.h"
#include "Parameter_ABC.h"
#include "protocol/publisher_ABC.h"
#include "actions/Numeric.h"
#include "clients_kernel/OrderType.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: MagicActionMeteo constructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
MagicActionMeteo::MagicActionMeteo( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /*= true*/ )
    : MagicAction( magic, controller, registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicActionMeteo constructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
MagicActionMeteo::MagicActionMeteo( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic )
    : MagicAction( xis, controller, magic )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicActionMeteo destructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
MagicActionMeteo::~MagicActionMeteo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicActionMeteo::Serialize
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
void MagicActionMeteo::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", GetType().GetName() );
    MagicAction::Serialize( xos );
}
// -----------------------------------------------------------------------------
// Name: MagicActionMeteo::Publish
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
void MagicActionMeteo::Publish( Publisher_ABC& publisher ) const
{
    simulation::ControlMeteo message;
    CommitTo( *message().mutable_parametres() );
    message.Send( publisher );
    message().Clear();
}
