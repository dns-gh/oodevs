// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionTiming.h"
#include "Simulation.h"
#include "clients_kernel/Controller.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionTiming constructor
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
ActionTiming::ActionTiming( kernel::Controller& controller, unsigned long startTime, const Action_ABC& owner )
    : controller_( controller )
    , owner_( owner )
    , enabled_( true )
    , time_( startTime )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTiming constructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
ActionTiming::ActionTiming( kernel::Controller& controller, const Simulation& simulation, const Action_ABC& owner )
    : controller_( controller )
    , owner_( owner )
    , enabled_( true )
    , time_( simulation.GetCurrentTick() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTiming constructor
// Created: SBO 2007-06-28
// -----------------------------------------------------------------------------
ActionTiming::ActionTiming( xml::xistream& xis, kernel::Controller& controller, const Action_ABC& owner )
    : controller_( controller )
    , owner_( owner )
    , enabled_( true )
{
    xis >> attribute( "time", time_ );
}

// -----------------------------------------------------------------------------
// Name: ActionTiming destructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
ActionTiming::~ActionTiming()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::Serialize
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void ActionTiming::Serialize( xml::xostream& xos ) const
{
    xos << attribute( "time", time_ );
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::GetAction
// Created: SBO 2007-06-28
// -----------------------------------------------------------------------------
const Action_ABC& ActionTiming::GetAction() const
{
    return owner_;
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::ToggleEnabled
// Created: SBO 2007-06-28
// -----------------------------------------------------------------------------
void ActionTiming::ToggleEnabled()
{
    enabled_ = !enabled_;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::IsEnabled
// Created: SBO 2007-06-28
// -----------------------------------------------------------------------------
bool ActionTiming::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::GetTime
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
unsigned long ActionTiming::GetTime() const
{
    return time_;
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::Shift
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void ActionTiming::Shift( long shift )
{
    time_ = std::max< long >( 0, long( time_ ) + shift );
    controller_.Update( *this );
}
