// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionWithTarget_ABC.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionWithTarget_ABC constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ActionWithTarget_ABC::ActionWithTarget_ABC( kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC* target )
    : Action_ABC( controller, type )
    , entityId_( ( target ) ? target->GetId() : 0 )
    , entityTypeName_( ( target ) ? target->GetTypeName() : "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionWithTarget_ABC constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ActionWithTarget_ABC::ActionWithTarget_ABC( xml::xistream& xis, kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target )
    : Action_ABC( xis, controller, type )
    , entityId_( target.GetId() )
    , entityTypeName_( target.GetTypeName() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionWithTarget_ABC destructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ActionWithTarget_ABC::~ActionWithTarget_ABC()
{
    // NOTHING
}
