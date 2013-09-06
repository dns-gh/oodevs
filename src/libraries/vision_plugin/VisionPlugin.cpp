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
#include "dispatcher/AuthenticatedLinkResolver_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "tools/MessageDispatcher_ABC.h"
#include <boost/range/algorithm.hpp>

using namespace plugins::vision;

VisionPlugin::VisionPlugin( const dispatcher::Model_ABC& model, tools::MessageDispatcher_ABC& dispatcher, dispatcher::AuthenticatedLinkResolver_ABC& resolver )
    : model_   ( model )
    , resolver_( resolver )
    , clients_ ( new Clients() )
    , units_   ( new Units() )
{
    dispatcher.RegisterMessage( *this, &VisionPlugin::OnReceive );
}

VisionPlugin::~VisionPlugin()
{
    // NOTHING
}

bool VisionPlugin::Filter( const sword::SimToClient& message ) const
{
    return message.message().has_unit_vision_cones();
}

void VisionPlugin::Receive( const sword::SimToClient& message )
{
    if( Filter( message ) )
    {
        const unsigned int id = message.message().unit_vision_cones().unit().id();
        cones_[ id ] = message;
        units_->Send( id, message );
        clients_->Send( message );
    }
}

void VisionPlugin::OnReceive( const std::string& link, const sword::ClientToSim& message )
{
    if( message.message().has_control_toggle_vision_cones() )
    {
        const auto& toggle = message.message().control_toggle_vision_cones();
        dispatcher::ClientPublisher_ABC& publisher = resolver_.GetPublisher( link );
        const unsigned int client = resolver_.GetClientID( link );
        if( ! Validate( publisher, toggle, message.context(), client ) )
            return;
        client::ControlEnableVisionConesAck ack;
        ack().set_error_code( sword::ControlEnableVisionConesAck::no_error );
        ack.Send( publisher, message.context(), client );
        if( toggle.units().size() == 0 )
            Register( publisher, toggle.vision_cones() );
        for( auto it = toggle.units().begin(); it != toggle.units().end(); ++it )
            Register( publisher, *it, toggle.vision_cones() );
    }
    else if( message.message().has_list_enabled_vision_cones() )
    {
        const auto& list = message.message().list_enabled_vision_cones();
        dispatcher::ClientPublisher_ABC& publisher = resolver_.GetPublisher( link );
        const auto start = list.has_start() ? list.start().id() : 0;
        std::size_t count = list.has_count() ? list.count() : std::numeric_limits< std::size_t >::max();
        client::ListEnabledVisionConesAck ack;
        ack().set_all( clients_->IsRegistered( publisher ) );
        units_->List( publisher, start, count, ack );
        ack.Send( publisher, message.context(), resolver_.GetClientID( link ) );
    }
}

bool VisionPlugin::Validate( dispatcher::ClientPublisher_ABC& publisher, const sword::ControlEnableVisionCones& message, int context, unsigned int client ) const
{
    for( auto it = message.units().begin(); it != message.units().end(); ++it )
        if( ! model_.Agents().Find( it->id() ) )
        {
            client::ControlEnableVisionConesAck ack;
            ack().set_error_code( sword::ControlEnableVisionConesAck::error_invalid_unit );
            ack().set_error_msg( "invalid unit identifier " + boost::lexical_cast< std::string >( it->id() ) );
            ack.Send( publisher, context, client );
            return false;
        }
    return true;
}

void VisionPlugin::Register( dispatcher::ClientPublisher_ABC& publisher, const sword::UnitId& unitId, bool activate )
{
    clients_->Unregister( publisher );
    const unsigned int id = unitId.id();
    if( activate )
    {
        units_->Register( publisher, unitId );
        auto it = cones_.find( id );
        if( it != cones_.end() )
            publisher.Send( it->second );
    }
    else
        units_->Unregister( publisher, unitId );
}

void VisionPlugin::Register( dispatcher::ClientPublisher_ABC& publisher, bool activate )
{
    units_->Unregister( publisher );
    if( activate )
        clients_->Register( publisher );
    else
        clients_->Unregister( publisher );
}

void VisionPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& publisher, const std::string& /*link*/ )
{
    clients_->Unregister( publisher );
    units_->Unregister( publisher );
}
