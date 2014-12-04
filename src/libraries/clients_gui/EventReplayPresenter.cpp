// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventReplayPresenter.h"
#include "moc_EventReplayPresenter.cpp"
#include "Event.h"
#include "EventAction.h"
#include "EventHelpers.h"
#include "EventReplayViewState.h"
#include "EventView_ABC.h"
#include "clients_kernel/TimelineHelpers.h"
#include <boost/assign.hpp>
#include <timeline/api.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter constructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventReplayPresenter::EventReplayPresenter( EventView_ABC< EventReplayViewState >& view )
    : EventSubPresenter_ABC< EventReplayViewState >( eEventTypes_Replay, view )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter destructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventReplayPresenter::~EventReplayPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter::OnLabelChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventReplayPresenter::OnLabelChanged( const QString& label )
{
    state_->label_ = label;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter::OnDescriptionChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventReplayPresenter::OnDescriptionChanged( const QString& description )
{
    state_->description_ = description;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter::OnDescriptionChanged
// Created: SLI 2014-11-27
// -----------------------------------------------------------------------------
void EventReplayPresenter::OnEnabledChanged( bool enabled )
{
    state_->enabled_ = enabled;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter::Trigger
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventReplayPresenter::Trigger( const Event& /*event*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter::Clear
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventReplayPresenter::Clear()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter::FillFrom
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventReplayPresenter::FillFrom( const Event& event )
{
    const timeline::Event& timelineEvent = event.GetEvent();
    state_->Purge();
    state_->label_ = QString::fromStdString( timelineEvent.name );
    state_->description_ = QString::fromStdString( timelineEvent.info );
    state_->enabled_ = gui::event_helpers::ReadReplayPayload( event.GetEvent() ).enabled;
}

// -----------------------------------------------------------------------------
// Name: EventReplayPresenter::CommitTo
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventReplayPresenter::CommitTo( timeline::Event& event ) const
{
    event.name = state_->label_.toStdString();
    event.info = state_->description_.toStdString();
    auto payload = event_helpers::ReadReplayPayload( event );
    payload.enabled = state_->enabled_;
    event_helpers::WriteReplayPayload( payload, event );
    event.action.target = timeline_helpers::CreateEventTarget( EVENT_REPLAY_PROTOCOL, EVENT_SIMULATION_SERVICE );
}
