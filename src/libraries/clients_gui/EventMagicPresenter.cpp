// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Event.h"
#include "EventAction.h"
#include "EventMagicPresenter.h"
#include "EventMagicViewState.h"
#include "EventView_ABC.h"

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"

#include "clients_kernel/Entity_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter constructor
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
EventMagicPresenter::EventMagicPresenter( EventView_ABC< EventMagicViewState >& view,
                                          kernel::Controllers& controllers,
                                          actions::ActionsModel& actionsModel )
    : EventSubPresenter_ABC< EventMagicViewState >( eEventTypes_Magic, view )
    , actionsModel_( actionsModel )
    , entity_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter destructor
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
EventMagicPresenter::~EventMagicPresenter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter::OnTargetChanged
// Created: ABR 2013-12-20
// -----------------------------------------------------------------------------
void EventMagicPresenter::OnTargetChanged( const kernel::Entity_ABC* entity )
{
    entity_ = entity;
    state_->target_ = entity_ ? entity_->GetId() : 0;
    BuildView();
}

namespace
{
    bool IsValidTarget( const kernel::Entity_ABC* /*entity*/ )
    {
        // check action type with entity type here
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter::ShouldEnableTrigger
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
bool EventMagicPresenter::ShouldEnableTrigger() const
{
    return IsValidTarget( entity_ );
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter::Trigger
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventMagicPresenter::Trigger( const gui::Event& event )
{
    if( const actions::Action_ABC* action = event.GetAction() )
        actionsModel_.Publish( *action );
    else
        throw MASA_EXCEPTION( "Invalid action" );
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter::Clear
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventMagicPresenter::Clear()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter::Purge
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventMagicPresenter::Purge()
{
    EventSubPresenter_ABC< EventMagicViewState >::Purge();
    entity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter::FillFrom
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventMagicPresenter::FillFrom( const Event& event )
{
    if( const actions::Action_ABC* action = event.GetAction() )
    {
        if( const actions::ActionTasker* tasker = action->Retrieve< actions::ActionTasker >() )
        {
            entity_ = tasker->GetTasker();
            state_->target_ = entity_ ? entity_->GetId() : 0;
            state_->hasTarget_ = true;
        }
        state_->name_ = action->GetName();
    }
    else
        throw MASA_EXCEPTION( "Invalid action" );
}

// -----------------------------------------------------------------------------
// Name: EventMagicPresenter::CommitTo
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventMagicPresenter::CommitTo( timeline::Event& /* event */ ) const
{
    // NOTHING cause no edition available for now
}
