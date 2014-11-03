// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventMarkerPresenter.h"
#include "moc_EventMarkerPresenter.cpp"
#include "Event.h"
#include "EventAction.h"
#include "EventHelpers.h"
#include "EventMarkerViewState.h"
#include "EventView_ABC.h"
#include "clients_kernel/TimelineHelpers.h"
#include <boost/assign.hpp>
#include <timeline/api.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter constructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventMarkerPresenter::EventMarkerPresenter( EventView_ABC< EventMarkerViewState >& view, const std::string& uuid )
    : EventSubPresenter_ABC< EventMarkerViewState >( eEventTypes_Marker, view )
    , uuid_( uuid )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter destructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventMarkerPresenter::~EventMarkerPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnLabelChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnLabelChanged( const QString& label )
{
    state_->label_ = label.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnDescriptionChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnDescriptionChanged( const QString& description )
{
    state_->description_ = description.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnResetDrawingsChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnResetDrawingsChanged( bool resetDrawings )
{
    state_->resetDrawings_ = resetDrawings;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnDrawingsPathChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnDrawingsPathChanged( const tools::Path& path )
{
    state_->drawingsPath_ = path;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::OnConfigurationPathChanged
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::OnConfigurationPathChanged( const tools::Path& path )
{
    state_->configurationPath_ = path;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::ShouldEnableTrigger
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
bool EventMarkerPresenter::ShouldEnableTrigger() const
{
    return true; // TODO
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::Trigger
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::Trigger( const Event& /*event*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::Clear
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::Clear()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::FillFrom
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::FillFrom( const Event& event )
{
    const timeline::Event& timelineEvent = event.GetEvent();
    state_->Purge();
    state_->label_ = timelineEvent.name;
    state_->description_ = timelineEvent.info;
    std::map< std::string, std::string > jsonPayload = boost::assign::map_list_of
        ( event_helpers::resetDrawingsKey, event_helpers::BoolToString( false ) )
        ( event_helpers::drawingsPathKey, "" )
        ( event_helpers::configurationPathKey, "" );
    event_helpers::ReadJsonPayload( timelineEvent, jsonPayload );
    state_->resetDrawings_ = event_helpers::StringToBool( jsonPayload[ event_helpers::resetDrawingsKey ] );
    state_->drawingsPath_ = tools::Path::FromUTF8( jsonPayload[ event_helpers::drawingsPathKey ] );
    state_->configurationPath_ = tools::Path::FromUTF8( jsonPayload[ event_helpers::configurationPathKey ] );
}

// -----------------------------------------------------------------------------
// Name: EventMarkerPresenter::CommitTo
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
void EventMarkerPresenter::CommitTo( timeline::Event& event ) const
{
    event.name = state_->label_;
    event.info = state_->description_;
    event.action.apply = false;
    const std::map< std::string, std::string > jsonPayload = boost::assign::map_list_of
        ( event_helpers::resetDrawingsKey, event_helpers::BoolToString( state_->resetDrawings_ ) )
        ( event_helpers::drawingsPathKey, state_->drawingsPath_.ToUTF8() )
        ( event_helpers::configurationPathKey, state_->configurationPath_.ToUTF8() );
    event_helpers::WriteJsonPayload( jsonPayload, event );
    event.action.apply = true;
    event.action.target = timeline_helpers::CreateEventTarget( EVENT_MARKER_PROTOCOL, uuid_ );
}
