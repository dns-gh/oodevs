// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "UnitMagicActionTeleport.h"
#include "protocol/simulationsenders.h"
#include "Parameter_ABC.h"
#include "protocol/publisher_ABC.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMagicActionTeleport constructor
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
UnitMagicActionTeleport::UnitMagicActionTeleport( xml::xistream& xis, kernel::Controller& controller, const MagicActionType& magic, const kernel::Entity_ABC& entity )
    : UnitMagicAction( xis, controller, magic, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMagicActionTeleport destructor
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
UnitMagicActionTeleport::~UnitMagicActionTeleport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMagicActionTeleport::Serialize
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
void UnitMagicActionTeleport::Serialize( xml::xostream& xos ) const
{
    UnitMagicAction::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicActionTeleport::Publish
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
void UnitMagicActionTeleport::Publish( Publisher_ABC& publisher ) const
{
    simulation::MagicActionMoveTo message;
    message().set_oid( GetEntity().GetId() );
    CommitTo( *message().mutable_parametres() );
    message.Send( publisher );
    const_cast< kernel::Entity_ABC& >( GetEntity() ).Update( message() );
    message().Clear();
}
