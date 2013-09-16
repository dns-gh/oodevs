// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "AutomatCreationListener.h"

#include "Action_ABC.h"
#include "ActionsModel.h"
#include "ActionTasker.h"
#include "ActionTiming.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_gui/EntityType.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: AutomatCreationListener constructor
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
AutomatCreationListener::AutomatCreationListener( const geometry::Point2f& point, const kernel::AutomatType& type, int context,
                 tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, kernel::Controller& controller, actions::ActionsModel& actionsModel, const kernel::Time_ABC& time )
    : point_          ( point)
    , type_           ( type )
    , context_        ( context )
    , automatResolver_( automatResolver )
    , controller_     ( controller )
    , actionsModel_   ( actionsModel )
    , time_           ( time )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatCreationListener destructor
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
AutomatCreationListener::~AutomatCreationListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatCreationListener::OnMessageReceived
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
bool AutomatCreationListener::OnMessageReceived( const sword::SimToClient& message )
{
    if( message.context() != context_ || !message.message().has_automat_creation() )
        return false;
    const sword::AutomatCreation& creation = message.message().automat_creation();
    kernel::Automat_ABC* automat = automatResolver_.Find( creation.automat().id() );
    if( !automat )
        return false;
    unsigned int numberOfAgents = type_.NumberOfAgents();
    static const float pi = std::acos( -1.f );
    float angle = numberOfAgents > 1 ? 2 * pi / ( numberOfAgents - 1 ) : 0;
    bool pcSet = false;
    unsigned int current = 0;
    const auto& compositions = type_.GetCompositions();
    for( auto composition = compositions.begin(); composition != compositions.end(); ++composition )
    {
        const kernel::AgentType& agentType = composition->GetType();
        unsigned int number = composition->GetSensibleNumber();
        geometry::Point2f point = point_;
        for( unsigned int i = 0; i < number; ++i )
        {
            if( pcSet || &composition->GetType() != automat->Get< ::gui::EntityType< kernel::AutomatType > >().GetType().GetTypePC() )
                point = geometry::Point2f( 100.f * std::sin( current++ * angle ), 100.f * std::cos( current * angle ) ) + point_.ToVector();
            actions::Action_ABC* action = actionsModel_.CreateAgentCreationAction( agentType, point, *automat );
            action->Attach( *new actions::ActionTiming( controller_, time_ ) );
            action->Attach( *new actions::ActionTasker( automat, false ) );
            actionsModel_.PublishForce( *action );
        }
    }
    return true;
}
