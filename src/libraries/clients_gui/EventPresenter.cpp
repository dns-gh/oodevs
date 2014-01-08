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
#include "EventOrderPresenter.h"
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
EventPresenter::EventPresenter( EventView_ABC& view,
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
void EventPresenter::AddSubPresenter( E_EventTypes type,
                                      const EventSubPresenter_ABC::T_SharedPtr& presenter )
{
    if( presenters_.find( type ) != presenters_.end() )
        throw MASA_EXCEPTION( "EventPresent already have a sub presenter for type " + ENT_Tr::ConvertFromEventType( type, ENT_Tr::eToSim ) );
    presenters_[ type ] = presenter;
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::GetPresenter
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
EventSubPresenter_ABC& EventPresenter::GetCurrentPresenter() const
{
    return GetPresenter( event_ ? event_->GetType() : eNbrEventTypes );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::GetPresenter
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
EventSubPresenter_ABC& EventPresenter::GetPresenter( E_EventTypes type ) const
{
    auto presenter = presenters_.find( type );
    if( presenter == presenters_.end() || !presenter->second )
        throw MASA_EXCEPTION( "EventPresenter can't found a sub presenter for type " + ENT_Tr::ConvertFromEventType( type, ENT_Tr::eToSim ) );
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
bool EventPresenter::IsCurrentEvent( const gui::Event& event )
{
    return event_ && event_->GetEvent().uuid == event.GetEvent().uuid;
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
    BuildView( eEventDockModes_None );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::InternalPurge
// Created: ABR 2013-12-06
// -----------------------------------------------------------------------------
void EventPresenter::InternalPurge()
{
    view_.Purge();
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
    BuildView( eEventDockModes_Create, true );
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
    BuildView( event.GetEvent().done
               ? eEventDockModes_DisplayTriggered
               : eEventDockModes_EditPlanned,
               raise );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnEventContentChanged
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::OnEventContentChanged()
{
    if( state_->mode_ == eEventDockModes_None )
        return;
    UpdateCurrent();
    UpdateViewAfterEdition();
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
        BuildView( eEventDockModes_EditTriggered, false, state_->warningColor_, state_->warning_ );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnWarningChanged
// Created: ABR 2013-12-03
// -----------------------------------------------------------------------------
void EventPresenter::OnWarningChanged( const std::string& warning, const QColor& warningColor )
{
    state_->warningColor_ = warningColor;
    state_->warning_ = warning;
    UpdateView();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnBeginDateChanged
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::OnBeginDateChanged( const QDateTime& begin )
{
    CheckEvent();
    event_->GetEvent().begin = begin.toString( EVENT_DATE_FORMAT ).toStdString();
    UpdateViewAfterEdition();
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
    UpdateViewAfterEdition();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::OnEndDateChanged
// Created: ABR 2013-11-20
// -----------------------------------------------------------------------------
void EventPresenter::OnEndDateChanged( const QDateTime& end )
{
    CheckEvent();
    event_->GetEvent().end = end.toString( EVENT_DATE_FORMAT ).toStdString();
    UpdateViewAfterEdition();
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
    UpdateView();
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
    if( state_->mode_ == eEventDockModes_EditPlanned )
    {
        UpdateCurrent();
        UpdateRemote();
    }
    else if( state_->mode_ == eEventDockModes_Create )
        Plan();
    BuildView( eEventDockModes_EditPlanned );
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
    Plan();
    BuildView( eEventDockModes_EditPlanned );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Trigger
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::Trigger()
{
    GetCurrentPresenter().Trigger();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Plan
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::Plan()
{
    CheckEvent();
    event_->GetEvent().uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    event_->GetEvent().done = false;
    if( !timelineHandler_ )
        throw MASA_EXCEPTION( "Can't plan an event without a timeline handler" );
    timelineHandler_->CreateEvent( event_->GetEvent(), true );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateRemote
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::UpdateRemote()
{
    CheckEvent();
    if( event_->GetEvent().done )
        throw MASA_EXCEPTION( "Can't edit an already triggered event" );
    if( !timelineHandler_ )
        throw MASA_EXCEPTION( "Can't edit an event without a timeline handler" );
    timelineHandler_->EditEvent( event_->GetEvent() );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateCurrent
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventPresenter::UpdateCurrent()
{
    CheckEvent();
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
    if( !timelineHandler_ )
        throw MASA_EXCEPTION( "Can't delete an event without a timeline handler" );
    timelineHandler_->DeleteEvent( event_->GetEvent().uuid );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::Select
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventPresenter::Select()
{
    CheckEvent();
    if( !timelineHandler_ )
        throw MASA_EXCEPTION( "Can't select an event without a timeline handler" );
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
// Name: EventPresenter::BuildView
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventPresenter::BuildView( E_EventDockModes mode,
                                bool raise /* = false */,
                                const QColor& warningColor /* = Qt::transparent */,
                                const std::string& warning /* = "" */ )
{
    ChangeMode( mode, raise, false, warningColor, warning );
    if( state_->event_ )
        GetCurrentPresenter().FillFrom( *state_->event_ );
    view_.Build( *state_ );
    UpdateView();
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateView
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventPresenter::UpdateView()
{
    state_->trigger_ = HasCurrentPresenter() ? GetCurrentPresenter().ShouldEnableTrigger() : false;
    state_->clear_ = HasCurrentPresenter() ? GetCurrentPresenter().ShouldEnableClear() : false;
    view_.Update( *state_ );
}

// -----------------------------------------------------------------------------
// Name: EventPresenter::UpdateViewAfterEdition
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventPresenter::UpdateViewAfterEdition()
{
    ChangeMode( state_->mode_ == eEventDockModes_DisplayTriggered
                    ? eEventDockModes_EditTriggered
                    : state_->mode_,
                false,
                false,
                Qt::transparent,
                "" );
    UpdateView();
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
        state_->warning_ = tools::translate( "EventPresenter", "Read only" ).toStdString();
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
