// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventTaskPresenter.h"
#include "moc_EventTaskPresenter.cpp"
#include "Event.h"
#include "EventTaskViewState.h"
#include "EventView_ABC.h"

#include <timeline/api.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter constructor
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
EventTaskPresenter::EventTaskPresenter( EventView_ABC< EventTaskViewState >& view )
    : EventSubPresenter_ABC< EventTaskViewState >( eEventTypes_Task, view )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter destructor
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
EventTaskPresenter::~EventTaskPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnLabelChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnLabelChanged( const QString& label )
{
    state_->label_ = label.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnDescriptionChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnDescriptionChanged( const QString& description )
{
    state_->description_ = description.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnUrlChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnUrlChanged( const QString& url )
{
    state_->url_ = url.toStdString();
    state_->isUrlValid_ = !state_->url_.empty();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnPayloadChanged
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnPayloadChanged( const QString& payload )
{
    state_->payload_ = payload.toStdString();
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::OnShowClicked
// Created: ABR 2013-12-10
// -----------------------------------------------------------------------------
void EventTaskPresenter::OnShowClicked()
{
    state_->isPayloadVisible_ = !state_->isPayloadVisible_;
    BuildView();
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::Trigger
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::Trigger()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::Clear
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::Clear()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::FillFrom
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventTaskPresenter::FillFrom( const gui::Event& event )
{
    const timeline::Event& timelineEvent = event.GetEvent();
    state_->Purge();
    state_->label_ = timelineEvent.name;
    state_->description_ = timelineEvent.info;
    state_->url_ = timelineEvent.action.target;
    const QString data = QByteArray::fromBase64( timelineEvent.action.payload.c_str() );
    state_->bytes_ = data.size();
    state_->payload_ = data.toStdString();
    state_->isUrlValid_ = !state_->url_.empty();
    state_->isPayloadVisible_ = false;
}

// -----------------------------------------------------------------------------
// Name: EventTaskPresenter::CommitTo
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
void EventTaskPresenter::CommitTo( timeline::Event& event ) const
{
    event.name = state_->label_;
    event.info = state_->description_;
    event.action.target = state_->url_;
    event.action.apply = false;
    if( !state_->url_.empty() )
    {
        QByteArray data( state_->payload_.c_str() );
        event.action.payload = data.toBase64().data();
    }
}
