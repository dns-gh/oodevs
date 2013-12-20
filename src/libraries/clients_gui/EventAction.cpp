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
    , action_( controllers )
    , missionType_( eNbrMissionType )
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
        if( controllers_.eventActions_.IsSelected( action_ ) )
            controllers_.eventActions_.DeselectAll();
        model_.Destroy( *action_ );
        action_ = 0;
    }
}

namespace
{
    E_MissionType GetMissionType( const sword::ClientToSim& msg )
    {
        if( msg.message().has_unit_order() )
            return eMissionType_Pawn;
        else if( msg.message().has_automat_order() )
            return eMissionType_Automat;
        else if( msg.message().has_crowd_order() )
            return eMissionType_Population;
        else if( msg.message().has_frag_order() )
            return eMissionType_FragOrder;
        return eNbrMissionType;
    }
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
    const std::string oldPayload = GetEvent().action.payload;
    Event::Update( event );
    if( action_ && oldPayload == event.action.payload )
        return;
    bool wasSelected_ = action_ && controllers_.actions_.IsSelected( action_ );
    Purge();
    sword::ClientToSim msg;
    msg.ParsePartialFromString( event.action.payload );
    if( msg.has_message() )
    {
        action_ = model_.CreateAction( msg, true );
        UpdateTiming();
        missionType_ = ::GetMissionType( msg );
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
    return action_;
}

// -----------------------------------------------------------------------------
// Name: EventAction::GetMissionType
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
E_MissionType EventAction::GetMissionType() const
{
    return missionType_;
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
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventAction::Select( kernel::ActionController& eventController, kernel::ActionController& actionController ) const
{
    eventController.Select( static_cast< const Event& >( *this ) );
    if( action_ && event_ && event_->done )
        actionController.Select( *action_ );
}

// -----------------------------------------------------------------------------
// Name: EventAction::UpdateTiming
// Created: ABR 2014-01-14
// -----------------------------------------------------------------------------
void EventAction::UpdateTiming()
{
    if( action_ && event_ )
        if( actions::ActionTiming* timing = action_.ConstCast()->Retrieve< actions::ActionTiming >() )
            timing->SetTime( QDateTime::fromString( QString::fromStdString( event_->begin ), EVENT_DATE_FORMAT ) );
}
