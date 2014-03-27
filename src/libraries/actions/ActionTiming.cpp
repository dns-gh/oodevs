// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionTiming.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionTiming constructor
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
ActionTiming::ActionTiming( kernel::Controller& controller, const kernel::Time_ABC& simulation )
    : controller_( controller )
    , simulation_( simulation )
    , enabled_( true )
    , time_( simulation_.GetDateTime() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTiming constructor
// Created: SBO 2010-09-21
// -----------------------------------------------------------------------------
ActionTiming::ActionTiming( kernel::Controller& controller, const kernel::Time_ABC& simulation, const std::string& datetime )
    : controller_( controller )
    , simulation_( simulation )
    , enabled_( true )
    , time_( tools::IsoStringToQDateTime( datetime ) )
{
    if( !time_.isValid() || time_.isNull() )
        time_ = QDateTime::fromString( QString::fromStdString( datetime ), Qt::ISODate );
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
void ActionTiming::SerializeAttributes( xml::xostream& xos ) const
{
    if( !time_.isNull() )
        xos << xml::attribute( "time", time_.toString( Qt::ISODate ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::Display
// Created: SBO 2010-09-21
// -----------------------------------------------------------------------------
void ActionTiming::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "ActionTiming", "Time" ), time_.toString() );
}

// -----------------------------------------------------------------------------
// Name: ActionTiming::DisplayInSummary
// Created: SBO 2010-05-04
// -----------------------------------------------------------------------------
void ActionTiming::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "ActionTiming", "Time" ), time_.toString( Qt::ISODate ) )
             .Display( tools::translate( "ActionTiming", "Enabled" ), enabled_ );
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
// Name: ActionTiming::SetTime
// Created: ABR 2014-01-14
// -----------------------------------------------------------------------------
void ActionTiming::SetTime( const QDateTime& time )
{
    time_ = time;
    controller_.Update( *this );
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

// -----------------------------------------------------------------------------
// Name: ActionTiming::GetIsoTime
// Created: ABR 2014-03-24
// -----------------------------------------------------------------------------
std::string ActionTiming::GetIsoTime() const
{
    return time_.toString( "yyyyMMddThhmmss" ).toStdString();
}
