// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventAction.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TimelineHelpers.h"
#include "protocol/Protocol.h"
#include <timeline/api.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventAction constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventAction::EventAction( E_EventTypes type,
                          const timeline::Event& event,
                          actions::ActionsModel& model,
                          kernel::Controllers& controllers )
    : Event( type, event )
    , model_( model )
    , controllers_( controllers )
{
    Event::Update();
}

// -----------------------------------------------------------------------------
// Name: EventAction destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventAction::~EventAction()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: EventAction::Clone
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
Event* EventAction::Clone() const
{
    return new EventAction( type_, *event_, model_, controllers_ );
}

// -----------------------------------------------------------------------------
// Name: EventAction::Purge
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
void EventAction::Purge()
{
    if( action_ )
    {
        if( controllers_.actions_.IsSelected( action_.get() ) )
            controllers_.actions_.DeselectAll();
        controllers_.controller_.Delete( *action_ );
        action_.reset();
    }
}

namespace
{
    E_EventTypes GetEventType( const sword::ClientToSim& msg )
    {
        if( msg.message().has_unit_order() ||
            msg.message().has_automat_order() ||
            msg.message().has_crowd_order() ||
            msg.message().has_frag_order() )
            return eEventTypes_Order;
        if( msg.message().has_set_automat_mode() ||
            msg.message().has_magic_action() ||
            msg.message().has_unit_magic_action() ||
            msg.message().has_knowledge_magic_action() ||
            msg.message().has_object_magic_action() )
            return eEventTypes_Magic;
        return eNbrEventTypes;
    }
}

// -----------------------------------------------------------------------------
// Name: EventAction::Update
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventAction::Update( const timeline::Event& event )
{
    Event::Update( event );
    bool wasSelected_ = action_ && controllers_.actions_.IsSelected( action_.get() );
    Purge();
    sword::ClientToSim msg;
    msg.ParsePartialFromString( event.action.payload );
    if( msg.has_message() )
    {
        action_.reset( model_.CreateAction( msg ) );
        if( action_ && action_->GetName().isEmpty() && !event.name.empty() )
            action_->Rename( QString::fromStdString( event.name ) );
        UpdateTiming();
        type_ = ::GetEventType( msg );
    }
    if( action_ && wasSelected_ )
        action_->Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: EventAction::GetAction
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
const actions::Action_ABC* EventAction::GetAction() const
{
    return action_.get();
}

// -----------------------------------------------------------------------------
// Name: EventAction::GetName
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
QString EventAction::GetName() const
{
    return ( action_ ) ? action_->GetName() : Event::GetName();
}

// -----------------------------------------------------------------------------
// Name: EventAction::GetTooltip
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
QString EventAction::GetTooltip() const
{
    return ( action_ ) ? action_->GetTooltip() : Event::GetTooltip();
}

// -----------------------------------------------------------------------------
// Name: EventAction::Select
// Created: ABR 2014-09-17
// -----------------------------------------------------------------------------
void EventAction::Select( kernel::ActionController& controller ) const
{
    controller.Select( static_cast< const Event& >( *this ) );
    if( action_ && event_ )
        controllers_.actions_.SetSelected( *action_, false );
}

// -----------------------------------------------------------------------------
// Name: EventAction::UpdateTiming
// Created: ABR 2014-01-14
// -----------------------------------------------------------------------------
void EventAction::UpdateTiming()
{
    if( action_ && event_ )
        if( actions::ActionTiming* timing = action_->Retrieve< actions::ActionTiming >() )
            timing->SetTime( QDateTime::fromString( QString::fromStdString( event_->begin ), EVENT_DATE_FORMAT ) );
}
