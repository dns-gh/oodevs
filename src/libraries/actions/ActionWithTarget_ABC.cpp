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
    , target_( controller, target )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionWithTarget_ABC constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ActionWithTarget_ABC::ActionWithTarget_ABC( xml::xistream& xis, kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target )
    : Action_ABC( xis, controller, type )
    , target_( controller, &target )
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

// -----------------------------------------------------------------------------
// Name: ActionWithTarget_ABC::GetTarget
// Created: ABR 2013-12-19
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ActionWithTarget_ABC::GetTarget() const
{
    return target_;
}

// -----------------------------------------------------------------------------
// Name: ActionWithTarget_ABC::GetEntityId
// Created: ABR 2013-12-19
// -----------------------------------------------------------------------------
unsigned int ActionWithTarget_ABC::GetEntityId() const
{
    return target_ ? target_->GetId() : 0;
}
