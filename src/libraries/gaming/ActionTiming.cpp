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
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
ActionTiming::ActionTiming( kernel::Controller& controller, const Simulation& simulation, const Action_ABC& owner )
    : controller_( controller )
    , simulation_( simulation )
    , owner_( owner )
    , startDay_( simulation.GetDay() )
    , startTime_( simulation.GetTime() )
    , enabled_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTiming constructor
// Created: SBO 2007-06-28
// -----------------------------------------------------------------------------
ActionTiming::ActionTiming( xml::xistream& xis, kernel::Controller& controller, const Simulation& simulation, const Action_ABC& owner )
    : controller_( controller )
    , simulation_( simulation )
    , owner_( owner )
    , startDay_( simulation.GetDay() )
    , startTime_( simulation.GetTime() )
    , enabled_( true )
{
    // NOTHING
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
// Name: ActionTiming::SetStart
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void ActionTiming::SetStart( xml::xistream& xis )
{
    std::string date;
    xis >> xml::attribute( "startdate", date );
    // $$$$ SBO 2007-06-19: 
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::Serialize
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void ActionTiming::Serialize( xml::xostream& xos ) const
{
    
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
