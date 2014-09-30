// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "VisionPlugin.h"
#include "Clients.h"
#include "Units.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/AuthenticatedLinkResolver_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include "tools/MessageDispatcher_ABC.h"
#include "MT_Tools/MT_Logger.h"

using namespace plugins::vision;

VisionPlugin::VisionPlugin( const dispatcher::Model_ABC& model, tools::MessageDispatcher_ABC& dispatcher,
                            dispatcher::SimulationPublisher_ABC& simulation, dispatcher::AuthenticatedLinkResolver_ABC& resolver )
    : model_     ( model )
    , simulation_( simulation )
    , resolver_  ( resolver )
    , clients_   ( new Clients() )
    , units_     ( new Units() )
    , enabled_   ( false )
{
    dispatcher.RegisterMessage( *this, &VisionPlugin::OnReceive );
}

VisionPlugin::~VisionPlugin()
{
    // NOTHING
}

void VisionPlugin::OnReceive( const std::string& link, const sword::ClientToSim& message )
{
    if( message.message().has_control_toggle_vision_cones() )
        Handle( link, message.message().control_toggle_vision_cones(), message.context() );
    else if( message.message().has_list_enabled_vision_cones() )
        Handle( link, message.message().list_enabled_vision_cones(), message.context() );
}

void VisionPlugin::Handle( const std::string& link, const sword::ControlEnableVisionCones& message, int context )
{
    dispatcher::ClientPublisher_ABC& publisher = resolver_.GetAuthenticatedPublisher( link );
    const unsigned int client = resolver_.GetClientID( link );
    if( ! Validate( publisher, message, context, client ) )
        return;
    client::ControlEnableVisionConesAck ack;
    ack().set_error_code( sword::ControlEnableVisionConesAck::no_error );
    ack.Send( publisher, context, client );
    if( message.units().size() == 0 )
        Register( publisher, message.vision_cones() );
    for( auto it = message.units().begin(); it != message.units().end(); ++it )
        Register( publisher, *it, message.vision_cones() );
}

void VisionPlugin::Handle( const std::string& link, const sword::ListEnabledVisionCones& message, int context ) const
{
    dispatcher::ClientPublisher_ABC& publisher = resolver_.GetAuthenticatedPublisher( link );
    const auto start = message.has_start() ? message.start().id() : 0;
    std::size_t count = message.has_count() ? message.count() : std::numeric_limits< std::size_t >::max();
    client::ListEnabledVisionConesAck ack;
    ack().set_all( clients_->IsRegistered( publisher ) );
    units_->List( publisher, start, count, ack );
    ack.Send( publisher, context, resolver_.GetClientID( link ) );
}

bool VisionPlugin::Validate( dispatcher::ClientPublisher_ABC& publisher, const sword::ControlEnableVisionCones& message, int context, unsigned int client ) const
{
    if( message.units().size() != 0 && ! message.vision_cones() && clients_->IsRegistered( publisher ) )
    {
        client::ControlEnableVisionConesAck ack;
        ack().set_error_code( sword::ControlEnableVisionConesAck::error_invalid_unit );
        ack().set_error_msg( "cannot unregister a single unit with vision cones globally enabled" );
        ack.Send( publisher, context, client );
        return false;
    }
    for( auto it = message.units().begin(); it != message.units().end(); ++it )
    {
        if( ! model_.Agents().Find( it->id() ) )
        {
            client::ControlEnableVisionConesAck ack;
            ack().set_error_code( sword::ControlEnableVisionConesAck::error_invalid_unit );
            ack().set_error_msg( "invalid unit identifier " + boost::lexical_cast< std::string >( it->id() ) );
            ack.Send( publisher, context, client );
            return false;
        }
    }
    return true;
}

void VisionPlugin::Register( dispatcher::ClientPublisher_ABC& publisher, const sword::Id& unitId, bool activate )
{
    const unsigned int id = unitId.id();
    if( activate )
    {
        if( clients_->IsRegistered( publisher ) )
            return;
        units_->Register( publisher, unitId );
        auto it = cones_.find( id );
        if( it != cones_.end() )
            publisher.Send( it->second );
    }
    else
        units_->Unregister( publisher, unitId );
    NotifyFilterChanged();
}

void VisionPlugin::Register( dispatcher::ClientPublisher_ABC& publisher, bool activate )
{
    units_->Unregister( publisher );
    if( activate )
        clients_->Register( publisher );
    else
        clients_->Unregister( publisher );
    NotifyFilterChanged();
}

void VisionPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& publisher, const std::string& link, bool uncounted )
{
    clients_->Unregister( publisher );
    units_->Unregister( publisher );
    NotifyFilterChanged();
    dispatcher::PluginContainer::NotifyClientLeft( publisher, link, uncounted );
}

void VisionPlugin::Update()
{
    NotifyFilterChanged();
    dispatcher::PluginContainer::Update();
}

void VisionPlugin::NotifyFilterChanged()
{
    const bool enabled = clients_->IsRegistered() || units_->IsRegistered();
    if( enabled_ == enabled )
        return;
    simulation::ControlEnableVisionCones message;
    message().set_vision_cones( enabled );
    message.Send( simulation_ );
    enabled_ = enabled;
    if( ! enabled_ )
        cones_.clear();
}

bool VisionPlugin::ForwardSimToClient( const sword::SimToClient& message )
{
    if( !message.message().has_unit_vision_cones() )
        return true;
    const unsigned int id = message.message().unit_vision_cones().unit().id();
    cones_[ id ] = message;
    units_->Send( id, message );
    clients_->Send( message );
    return false;
}
