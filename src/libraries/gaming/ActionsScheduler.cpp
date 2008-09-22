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
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "ActionTiming.h"
#include "game_asn/SimulationSenders.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionsScheduler constructor
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
ActionsScheduler::ActionsScheduler( QObject* parent, Controllers& controllers, const Simulation& simulation, const actions::ActionsModel& actions, Publisher_ABC& publisher )
    : QObject( parent )
    , controllers_( controllers )
    , simulation_( simulation )
    , actions_( actions )
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
// Created: SBO 2007-07-13
// -----------------------------------------------------------------------------
void ActionsScheduler::NotifyUpdated( const Simulation::sStartTick& )
{
    currentTime_ = simulation_.GetDateTime();
    Iterator< const actions::Action_ABC& > it( actions_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const actions::Action_ABC& action = it.NextElement();
        if( const ActionTiming* timing = action.Retrieve< ActionTiming >() )
        {
            const QDateTime dateTime = timing->GetTime();
            if( timing->IsEnabled() && dateTime <= currentTime_ && dateTime.secsTo( currentTime_ ) < int( simulation_.GetTickDuration() ) )
                action.Publish( publisher_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::GetDateTime
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
QDateTime ActionsScheduler::GetDateTime() const
{
    return currentTime_;
}

namespace
{
    std::string MakeGDHString( const QDateTime& datetime )
    {
        // $$$$ SBO 2008-04-25: ...
        QString str = datetime.toString( Qt::ISODate );
        str.remove( ':' );
        str.remove( '-' );
        return str.ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsScheduler::Shift
// Created: SBO 2007-07-16
// -----------------------------------------------------------------------------
void ActionsScheduler::Shift( long secs )
{
    QDateTime newTime = simulation_.GetDateTime().addSecs( secs );
    if( newTime < simulation_.GetInitialDateTime() )
        newTime = simulation_.GetInitialDateTime();
    simulation::ControlDatetimeChange asn;
    asn().date_time = MakeGDHString( newTime ).c_str();
    asn.Send( publisher_ );    
}
