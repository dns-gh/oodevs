// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SupplyResolver.h"
#include "protocol/Protocol.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: SupplyConsignData constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
SupplyConsignData::SupplyConsignData() 
    : tick_( noValue_ ), automatId_( noValue_ ), providerId_( noValue_ ), transportProviderId_( noValue_ ), conveyorId_( noValue_ ), stateId_( noValue_ )
    , simTime_( noField_ ), automat_( noField_ ), provider_( noField_ ), transportProvider_( noField_ ), conveyor_( noField_ ), state_( noField_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyConsignData::Write
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void SupplyConsignData::Write( std::ofstream& output )
{
    //format: tick , GDH, automat , provider , transport_provider , convoyeur , state , (resourcetype , requested, granted, conveyed)*
    //example: 1 , 20120312T240000 , 12, 1.1.4.BRB23 , 28, 1.2.BLT , 18, 1.2.TR323, 22, 1.2.TR323.TR1, 3, en attente transporteurs , 4, F63, 12, 12, 0 , 5, F65, 3, 3, 0
    output  << tick_                 <<  separator_
            << simTime_              <<  separator_
            << automatId_            <<  subSeparator_   << automat_            << separator_
            << providerId_           <<  subSeparator_   << provider_           << separator_
            << transportProviderId_  <<  subSeparator_   << transportProvider_  << separator_
            << conveyorId_           <<  subSeparator_   << conveyor_           << separator_
            << stateId_              <<  subSeparator_   << state_;

    for( int i=0; i < resources_.size(); ++i )
    {
        Resource& resource = resources_[ i ];
        output  << separator_
                << resource.id_         << subSeparator_
                << resource.type_       << subSeparator_
                << resource.requested_  << subSeparator_
                << resource.granted_    << subSeparator_
                << resource.conveyed_   << subSeparator_;
    }
    output  << std::endl;
}

// -----------------------------------------------------------------------------
// Name: SupplyResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
SupplyResolver::SupplyResolver( const std::string& name, const dispatcher::Model_ABC& model )
    : ConsignResolver_ABC( name, "tick , GDH, automat , provider , transport_provider , convoyeur , state , (resourcetype , requested, granted, conveyed)*", model )
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

namespace 
{
    void GetSupplykName( sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus eSupply, std::string& name )
    {
        if( sword::LogSupplyHandlingUpdate::convoy_waiting_for_transporters == eSupply )
            name = "convoy_waiting_for_transporters";
        else if( sword::LogSupplyHandlingUpdate::convoy_setup == eSupply )
            name = "convoy_setup";
        else if( sword::LogSupplyHandlingUpdate::convoy_moving_to_loading_point == eSupply )
            name = "convoy_moving_to_loading_point";
        else if( sword::LogSupplyHandlingUpdate::convoy_loading == eSupply )
            name = "convoy_loading";
        else if( sword::LogSupplyHandlingUpdate::convoy_moving_to_unloading_point == eSupply )
            name = "convoy_moving_to_unloading_point";
        else if( sword::LogSupplyHandlingUpdate::convoy_unloading == eSupply )
            name = "convoy_unloading";
        else if( sword::LogSupplyHandlingUpdate::convoy_moving_back_to_loading_point == eSupply )
            name = "convoy_moving_back_to_loading_point";
        else if( sword::LogSupplyHandlingUpdate::convoy_finished == eSupply )
            name = "convoy_finished";
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void SupplyResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_supply_handling_creation() )
    {
        const ::sword::LogSupplyHandlingCreation& msg = message.message().log_supply_handling_creation();
        if( msg.has_request() )
        {
            SupplyConsignData* pConsignData = GetConsign< SupplyConsignData >( static_cast< int >( msg.request().id() ) );
            pConsignData->simTime_ = simTime_;
            if( msg.has_tick() )
                pConsignData->tick_ = msg.tick();
            if( msg.has_supplier() )
            {
                if( msg.supplier().has_automat() )
                {
                    pConsignData->providerId_ = static_cast< int >( msg.supplier().automat().id() );
                    GetAgentName( pConsignData->providerId_, pConsignData->provider_ );
                }
                else if( msg.supplier().has_formation() )
                {
                    pConsignData->providerId_ = static_cast< int >( msg.supplier().formation().id() );
                    GetFormationName( pConsignData->providerId_, pConsignData->provider_ );
                }
            }
            if( msg.has_transporters_provider() )
            {
                if( msg.transporters_provider().has_automat() )
                {
                    pConsignData->transportProviderId_ = static_cast< int >( msg.transporters_provider().automat().id() );
                    GetAutomatName( pConsignData->transportProviderId_, pConsignData->transportProvider_ );
                }
                else if( msg.transporters_provider().has_formation() )
                {
                    pConsignData->transportProviderId_ = static_cast< int >( msg.transporters_provider().formation().id() );
                    GetFormationName( pConsignData->transportProviderId_, pConsignData->transportProvider_ );
                }
            }
            pConsignData->Write( output_ );
        }
    }
    if( message.message().has_log_supply_handling_update() )
    {
        const ::sword::LogSupplyHandlingUpdate& msg = message.message().log_supply_handling_update();
        if( msg.has_request() )
        {
            SupplyConsignData* pConsignData = GetConsign< SupplyConsignData >( static_cast< int >( msg.request().id() ) );
            pConsignData->simTime_ = simTime_;
            if( msg.has_convoyer() )
            {
                pConsignData->conveyorId_ = msg.convoyer().id();
                GetAgentName( pConsignData->conveyorId_, pConsignData->conveyor_ );
            }
            if( msg.has_state() )
            {
                sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus eSupply = msg.state();
                GetSupplykName( eSupply, pConsignData->state_ );
                pConsignData->stateId_ = eSupply;
            }
            if( msg.has_current_state_end_tick() )
                pConsignData->tick_ = msg.current_state_end_tick();
            if( msg.has_requests() )
            {
                pConsignData->resources_.clear();
                const ::sword::SupplyRecipientResourceRequests& requests = msg.requests();
                for( int req = 0; req < requests.requests().size(); ++req )
                {
                    const sword::SupplyRecipientResourcesRequest& request = requests.requests( req );
                    for ( int res = 0; res < request.resources().size(); ++res )
                    {
                        const sword::SupplyResourceRequest& resourceMsg = request.resources( res );
                        SupplyConsignData::Resource resource;
                        if( resourceMsg.has_resource() )
                        {
                            resource.id_ = resourceMsg.resource().id();
                            resource.requested_ = resourceMsg.requested();
                            resource.granted_ = resourceMsg.granted();
                            resource.conveyed_ = resourceMsg.convoyed();
                            pConsignData->resources_.push_back( resource );
                        }
                    }
                }
            }
            pConsignData->Write( output_ );
        }
    }
}
