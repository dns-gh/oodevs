// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SupplyResolver.h"
#include "ConsignWriter.h"
#include "NameResolver_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/MessageParameters.h"
#include "tools/FileWrapper.h"
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: SupplyConsignData::WriteConsign
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void SupplyConsignData::WriteConsign( ConsignWriter& output ) const
{
    if( recipientAutomats_.empty() )
    {
        output << requestId_
            << tick_
            << simTime_
            << recipientAutomat_
            << provider_
            << transportProvider_
            << conveyor_
            << state_
            << stateEndTick_;
        output.End();
    }
    else if( !resources_.empty() )
        for( std::map< int, std::string >::const_iterator it = recipientAutomats_.begin(); it != recipientAutomats_.end(); ++it )
        {
            int recipientAutomatId = it->first;
            output << requestId_
                << tick_
                << simTime_
                << it->second
                << provider_
                << transportProvider_
                << conveyor_
                << state_
                << stateEndTick_;

            for( std::map< int, Resource >::const_iterator itRes = resources_.begin(); itRes != resources_.end(); ++itRes )
            {
                const Resource& resource = itRes->second;
                if( resource.recipientAutomatId_ == recipientAutomatId )
                {
                    output << resource.type_
                        << resource.requested_
                        << resource.granted_
                        << resource.conveyed_;
                }
            }
            output.End();
        }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool SupplyConsignData::ManageMessage( const ::sword::LogSupplyHandlingCreation& msg,
        const NameResolver_ABC& nameResolver )
{
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_supplier() )
    {
        if( msg.supplier().has_automat() )
        {
            const uint32_t supplierId = msg.supplier().automat().id();
            providerId_ = boost::lexical_cast< std::string >( supplierId );
            nameResolver.GetAutomatName( supplierId, provider_ );
        }
        else if( msg.supplier().has_formation() )
        {
            const uint32_t supplierId = msg.supplier().formation().id();
            providerId_ = boost::lexical_cast< std::string >( supplierId );
            nameResolver.GetFormationName( msg.supplier().formation().id(), provider_ );
        }
    }
    if( msg.has_transporters_provider() )
    {
        if( msg.transporters_provider().has_automat() )
        {
            int transportId = static_cast< int >( msg.transporters_provider().automat().id() );
            transportProviderId_ = boost::lexical_cast< std::string >( transportId );
            nameResolver.GetAutomatName( transportId, transportProvider_ );
        }
        else if( msg.transporters_provider().has_formation() )
        {
            int transportId = static_cast< int >( msg.transporters_provider().formation().id() );
            transportProviderId_ = boost::lexical_cast< std::string >( transportId );
            nameResolver.GetFormationName( transportId, transportProvider_ );
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool SupplyConsignData::ManageMessage( const ::sword::LogSupplyHandlingUpdate& msg,
        const NameResolver_ABC& nameResolver )
{
    if( msg.has_current_state_end_tick() )
    {
        int endTick = msg.current_state_end_tick();
        if( endTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( endTick );
        if( endTick <= GetTick() )
            stateEndTick_.clear();
    }
    if( msg.has_convoyer() )
    {
        int conveyorId = msg.convoyer().id();
        if( conveyorId > 0 )
        {
            conveyorId_ = boost::lexical_cast< std::string >( msg.convoyer().id() );
            nameResolver.GetAgentName( conveyorId, conveyor_ );
        }
    }
    if( msg.has_state() )
    {
        sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus eSupply = msg.state();
        nameResolver.GetSupplykName( eSupply, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eSupply ) );
    }
    if( msg.has_requests() )
    {
        const ::sword::SupplyRecipientResourceRequests& requests = msg.requests();
        for( int req = 0; req < requests.requests().size(); ++req )
        {
            const sword::SupplyRecipientResourcesRequest& request = requests.requests( req );
            int recipientId = request.recipient().id();
            nameResolver.GetAutomatName( recipientId, recipientAutomats_[ recipientId ] );
            for ( int res = 0; res < request.resources().size(); ++res )
            {
                const sword::SupplyResourceRequest& resourceMsg = request.resources( res );
                SupplyConsignData::Resource resource;
                if( resourceMsg.has_resource() )
                {
                    nameResolver.GetResourceName( resourceMsg.resource(), resource.type_ );
                    int resourceId = resourceMsg.resource().id();
                    resource.recipientAutomatId_ = recipientId;
                    resource.recipientId_   = boost::lexical_cast< std::string >( recipientId );
                    resource.id_            = boost::lexical_cast< std::string >( resourceId );
                    resource.requested_     = boost::lexical_cast< std::string >( resourceMsg.requested() );
                    resource.granted_       = boost::lexical_cast< std::string >( resourceMsg.granted() );
                    resource.conveyed_      = boost::lexical_cast< std::string >( resourceMsg.convoyed() );
                    resources_[ resourceId ] = resource;
                }
            }
        }
    }
    return true;
}

bool SupplyConsignData::DoUpdateConsign( const sword::SimToClient& message,
        const NameResolver_ABC& resolver )
{
    const auto& msg = message.message();
    if( msg.has_log_supply_handling_creation() )
    {
        const auto& sub = msg.log_supply_handling_creation();
        return ManageMessage( sub, resolver );
    }
    if( msg.has_log_supply_handling_update() )
    {
        const auto& sub = msg.log_supply_handling_update();
        return ManageMessage( sub, resolver );
    }
    return false;
}

std::string plugins::logistic::GetSupplyHeader()
{
    SupplyConsignData consign( tools::translate( "logistic", "request id" ).toStdString() );
    consign.tick_               = tools::translate( "logistic", "tick" ).toStdString();
    consign.creationTick_       = tools::translate( "logistic", "creation tick" ).toStdString();
    consign.stateEndTick_       = tools::translate( "logistic", "state end tick" ).toStdString();
    consign.simTime_            = tools::translate( "logistic", "GDH" ).toStdString();
    consign.recipientAutomatId_ = tools::translate( "logistic", "recipient id" ).toStdString();
    consign.providerId_         = tools::translate( "logistic", "provider id" ).toStdString();
    consign.transportProviderId_= tools::translate( "logistic", "transport provider id" ).toStdString();
    consign.conveyorId_         = tools::translate( "logistic", "conveyor id" ).toStdString();
    consign.stateId_            = tools::translate( "logistic", "state id" ).toStdString();
    consign.recipientAutomat_   = tools::translate( "logistic", "recipient" ).toStdString();
    consign.provider_           = tools::translate( "logistic", "provider" ).toStdString();
    consign.transportProvider_  = tools::translate( "logistic", "transport provider" ).toStdString();
    consign.conveyor_           = tools::translate( "logistic", "conveyor" ).toStdString();
    consign.state_              = tools::translate( "logistic", "state" ).toStdString();
    consign.recipientAutomats_[ 0 ] = consign.recipientAutomat_;

    for( int i = 0; i < 15; ++i )
    {
        SupplyConsignData::Resource resource;
        resource.recipientAutomatId_ = 0;
        resource.recipientId_ = consign.recipientAutomatId_;
        resource.id_        = tools::translate( "logistic", "resource type id" ).toStdString();
        resource.type_      = tools::translate( "logistic", "resource type" ).toStdString();
        resource.requested_ = tools::translate( "logistic", "requested" ).toStdString();
        resource.granted_   = tools::translate( "logistic", "granted" ).toStdString();
        resource.conveyed_  = tools::translate( "logistic", "conveyed" ).toStdString();
        consign.resources_[ i ] = resource;
    }
    return consign.ToString();
}
