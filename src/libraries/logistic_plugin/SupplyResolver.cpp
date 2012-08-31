// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SupplyResolver.h"
#include <boost/lexical_cast.hpp>
#include "clients_kernel/Tools.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: SupplyConsignData::Write
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void SupplyConsignData::operator>>( std::stringstream& output ) const
{
    static const std::string separator = ConsignData_ABC::GetSeparator();
    if( recipientAutomats_.empty() )
    {
        output << requestId_         << separator
            << tick_                 << separator
            << simTime_              << separator   // << creationTick_         << separator    << recipientId_       << separator
            << recipientAutomat_     << separator   // << providerId_           << separator
            << provider_             << separator   // << transportProviderId_  << separator
            << transportProvider_    << separator   // << conveyorId_           << separator
            << conveyor_             << separator   // << stateId_              << separator
            << state_                << separator
            << stateEndTick_;
        output  << std::endl;
    }
    else if( !resources_.empty() )
        for( std::map< int, std::string >::const_iterator it = recipientAutomats_.begin(); it != recipientAutomats_.end(); ++it )
        {
            int recipientAutomatId = it->first;
            output << requestId_         << separator
                << tick_                 << separator
                << simTime_              << separator   // << creationTick_         <<  separator   << recipientId_    <<  separator
                << it->second            << separator   // << providerId_           <<  separator
                << provider_             << separator   // << transportProviderId_  <<  separator
                << transportProvider_    << separator   // << conveyorId_           <<  separator
                << conveyor_             << separator   // << stateId_              <<  separator
                << state_                << separator
                << stateEndTick_;

            for( std::map< int, Resource >::const_iterator itRes = resources_.begin(); itRes != resources_.end(); ++itRes )
            {
                const Resource& resource = itRes->second;
                if( resource.recipientAutomatId_ == recipientAutomatId )
                {
                    output << separator      // << resource.id_ << separator
                        << resource.type_       << separator
                        << resource.requested_  << separator
                        << resource.granted_    << separator
                        << resource.conveyed_;
                }
            }
            output  << std::endl;
        }
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& SupplyConsignData::ManageMessage( const ::sword::LogSupplyHandlingCreation& msg, ConsignResolver_ABC& resolver )
{
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_supplier() )
    {
        if( msg.supplier().has_automat() )
        {
            int supplierId = static_cast< int >( msg.supplier().automat().id() );
            providerId_ = boost::lexical_cast< std::string >( supplierId );
            resolver.GetAutomatName( supplierId, provider_ );
        }
        else if( msg.supplier().has_formation() )
        {
            int supplierId = static_cast< int >( msg.supplier().formation().id() );
            providerId_ = boost::lexical_cast< std::string >( supplierId );
            resolver.GetFormationName( msg.supplier().formation().id(), provider_ );
        }
    }
    if( msg.has_transporters_provider() )
    {
        if( msg.transporters_provider().has_automat() )
        {
            int transportId = static_cast< int >( msg.transporters_provider().automat().id() );
            transportProviderId_ = boost::lexical_cast< std::string >( transportId );
            resolver.GetAutomatName( transportId, transportProvider_ );
        }
        else if( msg.transporters_provider().has_formation() )
        {
            int transportId = static_cast< int >( msg.transporters_provider().formation().id() );
            transportProviderId_ = boost::lexical_cast< std::string >( transportId );
            resolver.GetFormationName( transportId, transportProvider_ );
        }
    }
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& SupplyConsignData::ManageMessage( const ::sword::LogSupplyHandlingUpdate& msg, ConsignResolver_ABC& resolver )
{
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_current_state_end_tick() )
    {
        int entTick = msg.current_state_end_tick();
        if( entTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( entTick );
    }
    if( msg.has_convoyer() )
    {
        int conveyorId = msg.convoyer().id();
        if( conveyorId > 0 )
        {
            conveyorId_ = boost::lexical_cast< std::string >( msg.convoyer().id() );
            resolver.GetAgentName( conveyorId, conveyor_ );
        }
    }
    if( msg.has_state() )
    {
        sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus eSupply = msg.state();
        resolver.GetSupplykName( eSupply, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eSupply ) );
    }
    if( msg.has_requests() )
    {
        const ::sword::SupplyRecipientResourceRequests& requests = msg.requests();
        for( int req = 0; req < requests.requests().size(); ++req )
        {
            const sword::SupplyRecipientResourcesRequest& request = requests.requests( req );
            int recipientId = request.recipient().id();
            resolver.GetAutomatName( recipientId, recipientAutomats_[ recipientId ] );
            for ( int res = 0; res < request.resources().size(); ++res )
            {
                const sword::SupplyResourceRequest& resourceMsg = request.resources( res );
                SupplyConsignData::Resource resource;
                if( resourceMsg.has_resource() )
                {
                    resolver.GetResourceName( resourceMsg.resource(), resource.type_ );
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
    resolver.AddToLineIndex( recipientAutomats_.empty()? 1 : static_cast< int >( recipientAutomats_.size() ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SupplyResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
SupplyResolver::SupplyResolver( const std::string& name, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel )
    : ConsignResolver_ABC( name, model, staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyResolver destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
SupplyResolver::~SupplyResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyResolver::IsManageable
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool SupplyResolver::IsManageable( const sword::SimToClient& message )
{
    return      message.message().has_log_supply_handling_creation()
            ||  message.message().has_log_supply_handling_update();
}

// -----------------------------------------------------------------------------
// Name: SupplyResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void SupplyResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_supply_handling_creation() )
        TraceConsign< ::sword::LogSupplyHandlingCreation, SupplyConsignData >( message.message().log_supply_handling_creation(), output_ );
    if( message.message().has_log_supply_handling_update() )
        TraceConsign< ::sword::LogSupplyHandlingUpdate, SupplyConsignData >( message.message().log_supply_handling_update(), output_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyResolver::InitHeader
// Created: MMC 2012-08-24
// -----------------------------------------------------------------------------
void SupplyResolver::InitHeader()
{
    SupplyConsignData consign( tools::translate( "logistic", "request id" ).toAscii().constData() );
    consign.tick_               = tools::translate( "logistic", "tick" ).toAscii().constData();
    consign.creationTick_       = tools::translate( "logistic", "creation tick" ).toAscii().constData();
    consign.stateEndTick_       = tools::translate( "logistic", "state end tick" ).toAscii().constData();
    consign.simTime_            = tools::translate( "logistic", "GDH" ).toAscii().constData();
    consign.recipientAutomatId_ = tools::translate( "logistic", "recipient id" ).toAscii().constData();
    consign.providerId_         = tools::translate( "logistic", "provider id" ).toAscii().constData();
    consign.transportProviderId_= tools::translate( "logistic", "transport provider id" ).toAscii().constData();
    consign.conveyorId_         = tools::translate( "logistic", "conveyor id" ).toAscii().constData();
    consign.stateId_            = tools::translate( "logistic", "state id" ).toAscii().constData();
    consign.recipientAutomat_   = tools::translate( "logistic", "recipient" ).toAscii().constData();
    consign.provider_           = tools::translate( "logistic", "provider" ).toAscii().constData();
    consign.transportProvider_  = tools::translate( "logistic", "transport provider" ).toAscii().constData();
    consign.conveyor_           = tools::translate( "logistic", "conveyor" ).toAscii().constData();
    consign.state_              = tools::translate( "logistic", "state" ).toAscii().constData();
    consign.recipientAutomats_[ 0 ] = consign.recipientAutomat_;
    for( int i = 0; i < 15; ++i )
    {
        SupplyConsignData::Resource resource;
        resource.recipientAutomatId_ = 0;
        resource.recipientId_ = consign.recipientAutomatId_;
        resource.id_        = tools::translate( "logistic", "resource type id" ).toAscii().constData();
        resource.type_      = tools::translate( "logistic", "resource type" ).toAscii().constData();
        resource.requested_ = tools::translate( "logistic", "requested" ).toAscii().constData();
        resource.granted_   = tools::translate( "logistic", "granted" ).toAscii().constData();
        resource.conveyed_  = tools::translate( "logistic", "conveyed" ).toAscii().constData();
        consign.resources_[ i ] = resource;
    }
    SetHeader( consign );
}
