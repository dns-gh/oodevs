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
    , enabled_( true )
    , time_( simulation_.GetDateTime() )
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
    , enabled_( true )
{
    std::string datetime;
    xis >> attribute( "time", datetime );
    time_ = QDateTime::fromString( datetime.c_str(), Qt::ISODate );
    if( !time_.isValid() )
        time_ = simulation_.GetInitialDateTime().addSecs( QString( datetime.c_str() ).toInt() * 10 ); // $$$$ SBO 2008-04-25: time factor
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
    xos << attribute( "time", time_.toString( Qt::ISODate ) );
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
QDateTime ActionTiming::GetTime() const
{
    return time_;
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::Shift
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void ActionTiming::Shift( long secs )
{
    time_ = time_.addSecs( secs );
    if( time_ < simulation_.GetInitialDateTime() )
        time_ = simulation_.GetInitialDateTime();
    controller_.Update( *this );
}
