// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "HierarchyPlugin.h"
#include "Hierarchies.h"

#include <tools/SessionConfig.h>
#include <tools/Helpers.h>
#include <protocol/Protocol.h>

using namespace plugins;

namespace
{
    const tools::Path dbname = "hierarchies.db";

    std::unique_ptr< Hierarchies > InitializeHierarchies( const tools::SessionConfig& config,
                                                          const tools::Path& name,
                                                          bool replay )
    {
        const auto filename = config.BuildSessionChildFile( name );
        if( !replay )
        {
            filename.Remove();
            if( config.HasCheckpoint() )
                ( config.GetCheckpointDirectory() / name ).Copy( filename, tools::Path::OverwriteIfExists );
        }
        return std::unique_ptr< Hierarchies >( new Hierarchies( config.GetCheckpointDirectory(), filename ) );
    }
}

HierarchyPlugin::HierarchyPlugin( const tools::SessionConfig& config, bool replay )
    : config_( config )
    , replay_( replay )
    , data_  ( InitializeHierarchies( config, ::dbname, replay ) )
{
    // NOTHING
}

HierarchyPlugin::~HierarchyPlugin()
{
    // NOTHING
}

namespace
{
    void OnControlBeginTick( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        dst.SetTick( msg.control_begin_tick().current_tick() );
    }

    void OnControlInformation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.control_information();
        if( m.has_current_tick() )
            dst.SetTick( m.current_tick() );
    }

    void OnAutomatCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.automat_creation();
        const auto& p = m.parent();
        const auto parent = p.has_formation() ? p.formation().id() : m.party().id();
        dst.SetParent( m.automat().id(), parent );
    }

    void OnAutomatChangeSuperior( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.automat_change_superior();
        dst.SetParent( m.automat().id(), m.superior().formation().id() );
    }

    void OnCrowdCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.crowd_creation();
        dst.SetParent( m.crowd().id(), m.party().id() );
    }

    void OnFormationCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.formation_creation();
        const auto parent = m.has_parent() ? m.parent().id() : m.party().id();
        dst.SetParent( m.formation().id(), parent );
    }

    void OnFormationChangeSuperior( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.formation_change_superior();
        const auto& s = m.superior();
        const auto parent = s.has_formation() ? s.formation().id() : s.party().id();
        dst.SetParent( m.formation().id(), parent );
    }

    void OnKnowledgeGroupCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.knowledge_group_creation();
        const auto parent = m.has_parent() ? m.parent().id() : m.party().id();
        dst.SetParent( m.knowledge_group().id(), parent );
    }

    void OnObjectCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.object_creation();
        dst.SetParent( m.object().id(), m.party().id() );
    }

    void OnPartyCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.party_creation();
        dst.SetParent( m.party().id(), 0 );
    }

    void OnPopulationCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.population_creation();
        dst.SetParent( m.id().id(), m.party().id() );
    }

    void OnUnitCreation( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.unit_creation();
        dst.SetParent( m.unit().id(), m.automat().id() );
    }

    void OnUnitChangeSuperior( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.unit_change_superior();
        dst.SetParent( m.unit().id(), m.parent().id() );
    }

    void OnControlCheckpointSaveEnd( Hierarchies& dst, const sword::SimToClient_Content& msg )
    {
        const auto& m = msg.control_checkpoint_save_end();
        dst.Save( tools::Path::FromUTF8( m.name() ) / ::dbname );
    }

    #define ADD_CALLBACK( X, Y ) { &sword::SimToClient_Content::has_## X, ( Y ) },
    const struct
    {
        bool( sword::SimToClient_Content::*Has )() const;
        void(* Handle )( Hierarchies&, const sword::SimToClient_Content& );
    } callbacks[] = {
        ADD_CALLBACK( automat_change_superior,          &OnAutomatChangeSuperior )
        ADD_CALLBACK( automat_creation,                 &OnAutomatCreation )
        ADD_CALLBACK( control_begin_tick,               &OnControlBeginTick )
        ADD_CALLBACK( control_checkpoint_save_end,      &OnControlCheckpointSaveEnd )
        ADD_CALLBACK( control_information,              &OnControlInformation )
        ADD_CALLBACK( crowd_creation,                   &OnCrowdCreation )
        ADD_CALLBACK( formation_change_superior,        &OnFormationChangeSuperior )
        ADD_CALLBACK( formation_creation,               &OnFormationCreation )
        ADD_CALLBACK( knowledge_group_creation,         &OnKnowledgeGroupCreation )
        ADD_CALLBACK( object_creation,                  &OnObjectCreation )
        ADD_CALLBACK( party_creation,                   &OnPartyCreation )
        ADD_CALLBACK( population_creation,              &OnPopulationCreation )
        ADD_CALLBACK( unit_change_superior,             &OnUnitChangeSuperior )
        ADD_CALLBACK( unit_creation,                    &OnUnitCreation )
    };
    #undef ADD_CALLBACK
}

void HierarchyPlugin::Receive( const sword::SimToClient& msg )
{
    if( replay_ )
        return;
    const auto& content = msg.message();
    for( size_t i = 0; i < COUNT_OF( callbacks ); ++i )
        if( ( ( content ).*( callbacks[i].Has ) )() )
            return callbacks[i].Handle( *data_, content );
}

void HierarchyPlugin::SendState( dispatcher::ClientPublisher_ABC& publisher )
{
    data_->SendState( publisher );
}

void HierarchyPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client,
    const std::string&, dispatcher::Profile_ABC&, unsigned int, bool )
{
    data_->RegisterClient( client );
}

void HierarchyPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client,
    const std::string&, bool )
{
    data_->UnregisterClient( client );
}
