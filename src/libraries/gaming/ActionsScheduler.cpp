// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionsScheduler.h"
#include "Simulation.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ReplaySenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionsScheduler constructor
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
ActionsScheduler::ActionsScheduler( QObject* parent, Controllers& controllers, const Simulation& simulation, Publisher_ABC& publisher )
    : QObject( parent )
    , controllers_( controllers )
    , simulation_( simulation )
    , publisher_( publisher )
    , currentTime_()
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler destructor
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
ActionsScheduler::~ActionsScheduler()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::NotifyUpdated
// Created: JSR 2010-11-04
// -----------------------------------------------------------------------------
void ActionsScheduler::NotifyUpdated( const Simulation& )
{
    if( simulation_.IsInitialized() && simulation_.GetCurrentTick() == 0 )
        currentTime_ = simulation_.GetDateTime();
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::NotifyUpdated
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
void ActionsScheduler::NotifyUpdated( const Simulation::sStartTick& )
{
    currentTime_ = simulation_.GetDateTime();
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::GetDateTime
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
QDateTime ActionsScheduler::GetDateTime() const
{
    return currentTime_;
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::Shift
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void ActionsScheduler::Shift( long secs )
{
    SetDate( simulation_.GetDateTime().addSecs( secs ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::SetDate
// Created: SBO 2010-05-31
// -----------------------------------------------------------------------------
void ActionsScheduler::SetDate( const QDateTime& dateTime )
{
    QDateTime newTime( dateTime );
    if( newTime < simulation_.GetInitialDateTime() )
        newTime = simulation_.GetInitialDateTime();
    simulation::ControlDateTimeChange message;
    message().mutable_date_time()->set_data( tools::QDateTimeToGDHString( dateTime ) );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::SkipToDate
// Created: NPT 2013-02-28
// -----------------------------------------------------------------------------
void ActionsScheduler::SkipToDate( const QDateTime& dateTime )
{
    replay::ControlSkipToDate skip;
    skip().mutable_date_time()->set_data( tools::QDateTimeToGDHString( dateTime ) );
    skip.Send( publisher_ );
}
