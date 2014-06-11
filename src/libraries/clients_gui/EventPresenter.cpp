// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventPresenter.h"
#include "moc_EventPresenter.cpp"
#include "Event.h"
#include "EventViewState.h"
#include "EventView_ABC.h"
#include "EventFactory.h"
#include "clients_kernel/TimelineHandler_ABC.h"
#include "clients_kernel/TimelineHelpers.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr.h"

#include <timeline/api.h>

#include <boost/lexical_cast.hpp>
#pragma warning( push )
#pragma warning( disable : 4724 )
#include <boost/uuid/random_generator.hpp>
#pragma warning( pop )
#include <boost/uuid/uuid_io.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventPresenter constructor
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
EventPresenter::EventPresenter( EventView_ABC< EventViewState >& view,
                                const EventFactory& factory )
    : view_( view )
    , factory_( factory )
    , state_( new EventViewState() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventPresenter destructor
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
EventPresenter::~EventPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::AddSubPresenter
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::AddSubPresenter( const EventPresenter_ABC::T_SharedPtr& presenter )
{
    if( presenters_.find( presenter->GetType() ) != presenters_.end() )
        throw MASA_EXCEPTION( "EventPresent already have a sub presenter for type " + ENT_Tr::ConvertFromEventTypes( presenter->GetType(), ENT_Tr::eToSim ) );
    presenters_[ presenter->GetType() ] = presenter;
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::GetPresenter
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
EventPresenter_ABC& EventPresenter::GetCurrentPresenter() const
{
    return GetPresenter( event_ ? event_->GetType() : eNbrEventTypes );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::GetPresenter
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
EventPresenter_ABC& EventPresenter::GetPresenter( E_EventTypes type ) const
{
    auto presenter = presenters_.find( type );
    if( presenter == presenters_.end() || !presenter->second )
        throw MASA_EXCEPTION( "EventPresenter can't found a sub presenter for type " + ENT_Tr::ConvertFromEventTypes( type, ENT_Tr::eToSim ) );
    return *presenter->second;
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::CheckEvent
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::CheckEvent() const
{
    if( state_->mode_ == eEventDockModes_None && event_ )
        throw MASA_EXCEPTION( "EventPresenter event is set while it should not be" );
    else if( state_->mode_ != eEventDockModes_None && !event_ )
        throw MASA_EXCEPTION( "EventPresenter event not set while it shoud be" );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::CheckHandler
// Created: ABR 2014-06-04
// -----------------------------------------------------------------------------
void EventPresenter::CheckHandler() const
{
    if( !timelineHandler_ )
        throw MASA_EXCEPTION( "Unable to process this action without a timeline handler" );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::SetTimelineHandler
// Created: ABR 2013-12-06
// -----------------------------------------------------------------------------
void EventPresenter::SetTimelineHandler( const boost::shared_ptr< kernel::TimelineHandler_ABC >& timelineHandler )
{
    timelineHandler_ = timelineHandler;
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::IsCurrentEvent
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
bool EventPresenter::IsCurrentEvent( const gui::Event& event ) const
{
    return event_ && event_->GetEvent().uuid == event.GetEvent().uuid;
}


// -----------------------------------------------------------------------------
// Name: EventPresenter::GetConsistencyError
// Created: ABR 2014-06-05
// -----------------------------------------------------------------------------
boost::optional< std::string > EventPresenter::GetConsistencyError( bool creation ) const
{
    CheckEvent();
    CheckHandler();
    const auto& event = event_->GetEvent();
    if( event.begin == event.end )
        return tr( "Error: unable to save a task with the same begin and end date" ).toStdString();
    const auto parent = timelineHandler_->GetCurrentParent();
    if( !parent.empty() && !event.end.empty() && ( parent != event.uuid || creation ) )
        return tr( "Error: unable to save a task with a duration and a parent" ).toStdString();
    return boost::none;
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::DisplayConsistencyWarning
// Created: ABR 2014-06-05
// -----------------------------------------------------------------------------
bool EventPresenter::DisplayConsistencyWarning( bool creation )
{
    auto error = GetConsistencyError( creation );
    if( !error )
        return false;
    OnWarningChanged( *error, Qt::red );
    return true;
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Draw
// Created: ABR 2014-01-09
// -----------------------------------------------------------------------------
void EventPresenter::Draw( gui::Viewport_ABC& viewport ) const
{
    if( HasCurrentPresenter() )
        GetCurrentPresenter().Draw( viewport );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Purge
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventPresenter::Purge()
{
    event_.reset();
    state_->Purge();
    InternalPurge();
    ResetView( eEventDockModes_None );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::InternalPurge
// Created: ABR 2013-12-06
// -----------------------------------------------------------------------------
void EventPresenter::InternalPurge()
{
    view_.BlockSignals( true );
    view_.Purge();
    view_.BlockSignals( false );
    for( auto it = presenters_.begin(); it != presenters_.end(); ++it )
        if( it->second.get() )
            it->second->Purge();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::StartCreation
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::StartCreation( E_EventTypes type, const QDateTime& beginDate )
{
    event_.reset( factory_.Create( type ) );
    event_->GetEvent().begin = beginDate.toString( EVENT_DATE_FORMAT ).toStdString();
    state_->Purge();
    InternalPurge();
    ResetView( eEventDockModes_Create, true );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::StartEdition
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::StartEdition( const gui::Event& event,
                                   bool raise,
                                   bool purge /* = true */ )
{
    event_.reset( event.Clone() );
    if( purge )
        InternalPurge();
    ResetView( event.GetType() == eEventTypes_Order && event.GetEvent().done
               ? eEventDockModes_DisplayTriggered
               : eEventDockModes_EditPlanned,
               raise );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnEventContentChanged
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::OnEventContentChanged( bool cleanWarning )
{
    if( state_->mode_ == eEventDockModes_None )
        return;
    UpdateCurrent();
    ChangeMode( state_->mode_ == eEventDockModes_DisplayTriggered
                    ? eEventDockModes_EditTriggered
                    : state_->mode_,
                false,
                false,
                cleanWarning ? Qt::transparent : state_->warningColor_,
                cleanWarning ? "" : state_->warning_ );
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnEventUpdated
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventPresenter::OnEventUpdated( const gui::Event& event )
{
    if( IsCurrentEvent( event ) )
        StartEdition( event, false );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnEventDeleted
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventPresenter::OnEventDeleted( const gui::Event& event )
{
    if( IsCurrentEvent( event ) )
        ResetView( eEventDockModes_EditTriggered, false, state_->warningColor_, state_->warning_ );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnWarningChanged
// Created: ABR 2013-12-03
// -----------------------------------------------------------------------------
void EventPresenter::OnWarningChanged( const std::string& warning, const QColor& warningColor )
{
    state_->warningColor_ = warningColor;
    state_->warning_ = warning;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnBeginDateChanged
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::OnBeginDateChanged( const QDateTime& begin )
{
    CheckEvent();
    event_->GetEvent().begin = begin.toString( EVENT_DATE_FORMAT ).toStdString();
    BuildViewAfterEdition();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnEndDateActivated
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::OnEndDateActivated( bool activated )
{
    CheckEvent();
    event_->GetEvent().end = activated
        ? event_->GetEvent().begin
        : "";
    BuildViewAfterEdition();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnEndDateChanged
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::OnEndDateChanged( const QDateTime& end )
{
    CheckEvent();
    event_->GetEvent().end = end.toString( EVENT_DATE_FORMAT ).toStdString();
    BuildViewAfterEdition();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnTriggerClicked
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::OnTriggerClicked()
{
    if( state_->mode_ == eEventDockModes_None )
        throw MASA_EXCEPTION( "Trigger button should be disabled" );
    if( state_->mode_ == eEventDockModes_EditPlanned )
        Delete();
    Trigger();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnClearClicked
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventPresenter::OnClearClicked()
{
    GetCurrentPresenter().Clear();
    OnWarningChanged( "", Qt::transparent );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnDetailClicked
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventPresenter::OnDetailClicked()
{
    CheckEvent();
    ChangeMode( state_->mode_,
                false,
                !state_->detail_,
                state_->warningColor_,
                state_->warning_ );
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnSaveClicked
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::OnSaveClicked()
{
    if( state_->mode_ == eEventDockModes_None ||
        state_->mode_ == eEventDockModes_DisplayTriggered ||
        state_->mode_ == eEventDockModes_EditTriggered )
        throw MASA_EXCEPTION( "Save button should be disabled" );
    if( DisplayConsistencyWarning( state_->mode_ == eEventDockModes_Create ) )
        return;
    if( state_->mode_ == eEventDockModes_EditPlanned )
    {
        UpdateCurrent();
        UpdateRemote();
        ResetView( eEventDockModes_EditPlanned );
    }
    else if( state_->mode_ == eEventDockModes_Create )
        Plan();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnSaveAsClicked
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::OnSaveAsClicked()
{
    if( state_->mode_ == eEventDockModes_None ||
        state_->mode_ == eEventDockModes_Create )
        throw MASA_EXCEPTION( "Save as button should be disabled" );
    if( DisplayConsistencyWarning( true ) )
        return;
    Plan();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Trigger
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::Trigger()
{
    GetCurrentPresenter().Trigger( *event_ );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Plan
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::Plan()
{
    CheckEvent();
    CheckHandler();
    auto& event = event_->GetEvent();
    event.uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    event.done = false;
    event.error_code = 0;
    event.error_text.clear();
    event.read_only = false;
    event.parent = timelineHandler_->GetCurrentParent();
    timelineHandler_->CreateEvent( event_->GetEvent(), true );
    ResetView( eEventDockModes_EditPlanned );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateRemote
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::UpdateRemote()
{
    CheckEvent();
    CheckHandler();
    if( event_->GetType() == eEventTypes_Order && event_->GetEvent().done )
        throw MASA_EXCEPTION( "Can't edit an already triggered event" );
    timelineHandler_->EditEvent( event_->GetEvent() );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateCurrent
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::UpdateCurrent()
{
    CheckEvent();
    event_->GetEvent().error_code = 0;
    event_->GetEvent().error_text.clear();
    if( HasCurrentPresenter() )
        GetCurrentPresenter().CommitTo( event_->GetEvent() );
    event_->Update();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Delete
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::Delete()
{
    CheckEvent();
    CheckHandler();
    timelineHandler_->DeleteEvent( event_->GetEvent().uuid );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Select
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventPresenter::Select()
{
    CheckEvent();
    CheckHandler();
    timelineHandler_->SelectEvent( event_->GetEvent().uuid );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::HasCurrentPresenter
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
bool EventPresenter::HasCurrentPresenter() const
{
    return event_ && presenters_.find( event_->GetType() ) != presenters_.end();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::ResetView
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventPresenter::ResetView( E_EventDockModes mode,
                                bool raise /* = false */,
                                const QColor& warningColor /* = Qt::transparent */,
                                const std::string& warning /* = "" */ )
{
    ChangeMode( mode, raise, false, warningColor, warning );
    if( state_->event_ && HasCurrentPresenter() )
    {
        EventPresenter_ABC& current = GetCurrentPresenter();
        if( state_->event_->GetType() != current.GetType() )
            throw MASA_EXCEPTION( "Trying to fill a presenter of type "
                                  + ENT_Tr::ConvertFromEventTypes( current.GetType() )
                                  + " with an event of type "
                                  + ENT_Tr::ConvertFromEventTypes( state_->event_->GetType() ) );
        current.FillFrom( *state_->event_ );
    }
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateView
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventPresenter::BuildView()
{
    if( HasCurrentPresenter() )
    {
        EventPresenter_ABC& current = GetCurrentPresenter();
        current.BuildView();
        state_->trigger_ = current.ShouldEnableTrigger();
        state_->clear_ = current.ShouldEnableClear();
    }
    view_.BlockSignals( true );
    view_.Build( *state_ );
    view_.BlockSignals( false );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateViewAfterEdition
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventPresenter::BuildViewAfterEdition()
{
    ChangeMode( state_->mode_ == eEventDockModes_DisplayTriggered
                    ? eEventDockModes_EditTriggered
                    : state_->mode_,
                false,
                false,
                Qt::transparent,
                "" );
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::ChangeMode
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventPresenter::ChangeMode( E_EventDockModes mode,
                                 bool raise,
                                 bool detail,
                                 const QColor& warningColor,
                                 const std::string& warning )
{
    state_->Purge();
    state_->mode_ = mode;
    state_->event_ = event_;
    CheckEvent();
    state_->warningColor_ = warningColor;
    state_->warning_ = warning;
    state_->raise_ = raise;
    state_->trigger_ = false;
    state_->clear_ = false;
    state_->detail_ = detail;
    state_->bottomToolBar_ = event_ && event_->GetType() == eEventTypes_Order;
    switch( state_->mode_ )
    {
    case eEventDockModes_None:
        state_->save_ = false;
        state_->saveAs_ = false;
        state_->clear_ = false;
        break;
    case eEventDockModes_DisplayTriggered:
        state_->save_ = false;
        state_->saveAs_ = true;
        state_->warningColor_ = Qt::darkGreen;
        state_->warning_ = tools::translate( "EventPresenter", "Order acknowledged" ).toStdString();
        break;
    case eEventDockModes_EditTriggered:
        state_->save_ = false;
        state_->saveAs_ = true;
        break;
    case eEventDockModes_EditPlanned:
        state_->save_ = true;
        state_->saveAs_ = true;
        break;
    case eEventDockModes_Create:
        state_->save_ = true;
        state_->saveAs_ = false;
        break;
    default:
        break;
    }
}
