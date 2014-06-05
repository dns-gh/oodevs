// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ClientConditions.h"
#include "EventCondition.h"
#include "Events.h"
#include "CommandString.h"
#include "ModelResolver.h"
#include "AgentManipulator.h"
#include "AutomatManipulator.h"
#include "clients_kernel/Entity_ABC.h"
#include "protocol/Protocol.h"
#include <tools/ElementObserver_ABC.h>
#include <directia/brain/Brain.h>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ClientConditions constructor
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
ClientConditions::ClientConditions( kernel::Controller& controller, const dispatcher::Model_ABC& model )
    : controller_( controller )
    , model_     ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientConditions destructor
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
ClientConditions::~ClientConditions()
{
    // NOTHING
}

namespace directia
{
    void UsedByDIA( ClientConditions* ) {}
    void ReleasedByDIA( ClientConditions* ) {}
}

// -----------------------------------------------------------------------------
// Name: ClientConditions::RegisterIn
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
void ClientConditions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "events.client" ] = this;
    brain.Register( "UserChose", &ClientConditions::UserChose );
    brain.Register( "MissionChosen", &ClientConditions::MissionChosen );
    brain.Register( "EntitySelected", &ClientConditions::EntitySelected );
}

// -----------------------------------------------------------------------------
// Name: ClientConditions::UserChose
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ClientConditions::UserChose()
{
    struct UserChoice : public SimpleEventCondition< sword::TextMessage >
    {
        UserChoice( kernel::Controller& controller )
            : SimpleEventCondition( controller )
        {
            // NOTHING
        }
        virtual void NotifyUpdated( const sword::TextMessage& message )
        {
            CommandString command( message.message() );
            if( command.Command() == "choose" )
                Trigger( command.Arg( 1 ), command.Arg( 2 ) );
        };
    };
    return boost::shared_ptr< Condition_ABC >( new UserChoice( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > ClientConditions::MissionChosen
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ClientConditions::MissionChosen()
{
    struct MissionChoice : public SimpleEventCondition< sword::TextMessage >
    {
        MissionChoice( kernel::Controller& controller, const dispatcher::Model_ABC& model )
            : SimpleEventCondition( controller )
            , resolver_( new ModelResolver( model ) )
        {
            // NOTHING
        }
        virtual void NotifyUpdated( const sword::TextMessage& message )
        {
            CommandString command( message.message() );
            if( command.Command() == "mission" )
            {
                const std::string mission = command.Arg( 3 );
                const kernel::Entity_ABC* entity = resolver_->Resolve( command );
                if( entity )
                    if( const AutomatManipulator* automat = entity->Retrieve< AutomatManipulator >() )
                    {
                        Trigger( mission, automat );
                        return;
                    }
                Trigger( mission, entity ? entity->Retrieve< AgentManipulator >() : 0 );
            }
        };
        std::unique_ptr< ModelResolver > resolver_;
    };
    return boost::shared_ptr< Condition_ABC >( new MissionChoice( controller_, model_ ) );
}

// -----------------------------------------------------------------------------
// Name: ClientConditions::EntitySelected
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ClientConditions::EntitySelected()
{
    struct EntitySelected : public SimpleEventCondition< sword::TextMessage >
    {
        EntitySelected( kernel::Controller& controller, const dispatcher::Model_ABC& model )
            : SimpleEventCondition( controller ), resolver_( new ModelResolver( model ) )
        {
            // NOTHING
        }
        virtual void NotifyUpdated( const sword::TextMessage& message )
        {
            CommandString command( message.message() );
            if( command.Command() == "select" )
            {
                const kernel::Entity_ABC* entity = resolver_->Resolve( command );
                if( entity )
                    if( const AutomatManipulator* automat = entity->Retrieve< AutomatManipulator >() )
                    {
                        Trigger( automat );
                        return;
                    }
                Trigger( entity ? entity->Retrieve< AgentManipulator >() : 0 );
            }
        };
        std::unique_ptr< ModelResolver > resolver_;
    };
    return boost::shared_ptr< Condition_ABC >( new EntitySelected( controller_, model_ ) );
}
