// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EventAction.h"
#include "ActionController.h"
#include "Controllers.h"
#include "Entity_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "protocol/Protocol.h"
#include <timeline/api.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EventAction constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventAction::EventAction( E_EventTypes type, const timeline::Event& event, actions::ActionsModel& model, Controllers& controllers )
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
    // NOTHING
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
        if( msg.message().has_unit_order() )
        {
            action_ = model_.CreateAction( msg.message().unit_order(), false );
            missionType_ = eMissionType_Pawn;
        }
        else if( msg.message().has_automat_order() )
        {
            action_ = model_.CreateAction( msg.message().automat_order(), false );
            missionType_ = eMissionType_Automat;
        }
        else if( msg.message().has_crowd_order() )
        {
            action_ = model_.CreateAction( msg.message().crowd_order(), false );
            missionType_ = eMissionType_Population;
        }
        else if( msg.message().has_frag_order() )
        {
            action_ = model_.CreateAction( msg.message().frag_order(), false );
            missionType_ = eMissionType_FragOrder;
        }
        else
            type_ = eEventTypes_SupervisorAction; // $$$$ ABR 2013-06-06: TODO
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
void EventAction::Select( ActionController& eventController, ActionController& actionController ) const
{
    eventController.Select( static_cast< const Event& >( *this ) );
    if( action_ && event_ && event_->done )
        actionController.Select( *action_ );
}
