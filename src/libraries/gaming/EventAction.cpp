// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "EventAction.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory_ABC.h"
#include "protocol/Protocol.h"
#include "timeline/api.h"
#include "tools/ProtobufSerialization.h"

// -----------------------------------------------------------------------------
// Name: EventAction constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventAction::EventAction( E_EventTypes type, const timeline::Event& event, const actions::ActionFactory_ABC& actionFactory )
    : Event( type, event )
    , actionFactory_( actionFactory )
    , action_( 0 )
    , missionType_( eNbrMissionTypes )
{
    Update( event );
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
    return new EventAction( type_, *event_, actionFactory_ );
}

// -----------------------------------------------------------------------------
// Name: EventAction::Update
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventAction::Update( const timeline::Event& event )
{
    Event::Update( event );
    sword::ClientToSim msg = tools::BinaryToProto< sword::ClientToSim >( event.action.payload );
    if( msg.has_message() )
    {
        if( msg.message().has_unit_order() )
        {
            action_.reset( actionFactory_.CreateAction( msg.message().unit_order(), false ) );
            missionType_ = eMissionType_Pawn;
        }
        else if( msg.message().has_automat_order() )
        {
            action_.reset( actionFactory_.CreateAction( msg.message().automat_order(), false ) );
            missionType_ = eMissionType_Automat;
        }
        else if( msg.message().has_crowd_order() )
        {
            action_.reset( actionFactory_.CreateAction( msg.message().crowd_order(), false ) );
            missionType_ = eMissionType_Population;
        }
        else if( msg.message().has_frag_order() )
        {
            action_.reset( actionFactory_.CreateAction( msg.message().frag_order(), false ) );
            missionType_ = eMissionType_FragOrder;
        }
        else
            type_ = eEventTypes_SupervisorAction; // $$$$ ABR 2013-06-06: TODO
    }
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
// Name: EventAction::GetMissionType
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
E_MissionType EventAction::GetMissionType() const
{
    return missionType_;
}
