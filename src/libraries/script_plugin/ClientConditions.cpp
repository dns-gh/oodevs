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
#include "directia/Brain.h"
#include "game_asn/Messenger.h"
#include "clients_kernel/Entity_ABC.h"
#include "tools/ElementObserver_ABC.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ClientConditions constructor
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
ClientConditions::ClientConditions( kernel::Controller& controller, const dispatcher::Model& model )
    : controller_( controller )
    , model_( model )
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

// -----------------------------------------------------------------------------
// Name: ClientConditions::RegisterIn
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
void ClientConditions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events.client", this );
    brain.RegisterFunction( "UserChose", &ClientConditions::UserChose );
    brain.RegisterFunction( "MissionChosen", &ClientConditions::MissionChosen );
    brain.RegisterFunction( "EntitySelected", &ClientConditions::EntitySelected );
}

// -----------------------------------------------------------------------------
// Name: ClientConditions::UserChose
// Created: SBO 2008-06-27
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ClientConditions::UserChose()
{
    struct UserChoice : public SimpleEventCondition< ASN1T_MsgTextMessage >
    {
        UserChoice( kernel::Controller& controller )
            : SimpleEventCondition( controller ) {}

        virtual void NotifyUpdated( const ASN1T_MsgTextMessage& message )
        {
            CommandString command( message.message );
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
    struct MissionChoice : public SimpleEventCondition< ASN1T_MsgTextMessage >
    {
        MissionChoice( kernel::Controller& controller, const dispatcher::Model& model )
            : SimpleEventCondition( controller )
            , resolver_( new ModelResolver( model ) ){}

        virtual void NotifyUpdated( const ASN1T_MsgTextMessage& message )
        {
            CommandString command( message.message );
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
        std::auto_ptr< ModelResolver > resolver_;
    };

    return boost::shared_ptr< Condition_ABC >( new MissionChoice( controller_, model_ ) );
}

// -----------------------------------------------------------------------------
// Name: ClientConditions::EntitySelected
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > ClientConditions::EntitySelected()
{
    struct EntitySelected : public SimpleEventCondition< ASN1T_MsgTextMessage >
    {
        EntitySelected( kernel::Controller& controller, const dispatcher::Model& model )
            : SimpleEventCondition( controller ), resolver_( new ModelResolver( model ) ) {}

        virtual void NotifyUpdated( const ASN1T_MsgTextMessage& message )
        {
            CommandString command( message.message );
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
        std::auto_ptr< ModelResolver > resolver_;
    };

    return boost::shared_ptr< Condition_ABC >( new EntitySelected( controller_, model_ ) );
}
