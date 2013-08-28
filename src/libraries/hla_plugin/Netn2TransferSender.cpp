// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Netn2TransferSender.h"
#include "OwnershipStrategy_ABC.h"
#include "ContextFactory_ABC.h"
#include "OwnershipController_ABC.h"
#include "Interactions.h"
#include "LocalAgentResolver_ABC.h"
#include "CallsignResolver_ABC.h"
#include "InteractionSender.h"
#include <hla/Interaction.h>
#include <hla/AttributeIdentifier.h>
#include <hla/VariableLengthData.h>
#include <hla/Serializer.h>
#include <hla/Deserializer.h>

#include <boost/scoped_array.hpp>
#include <cassert>
#include <algorithm>

using namespace plugins::hla;

namespace
{
    std::vector< char > GetUniqueId( const std::string& agentID, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    {
        unsigned long simId = agentResolver.Resolve( agentID );
        return callsignResolver.ResolveUniqueId( simId );
    }
    std::string GetAgentId( const std::vector< char >& uniqueID, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    {
        unsigned long simId = callsignResolver.ResolveSimulationIdentifier( uniqueID );
        return agentResolver.Resolve( simId );
    }
}

// -----------------------------------------------------------------------------
// Name: Netn2TransferSender constructor
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
Netn2TransferSender::Netn2TransferSender( const std::string& federateName, const ::hla::FederateIdentifier& federateHandle, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder,
        OwnershipStrategy_ABC& ownershipStrategy, OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger,
        const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    : ctxtFactory_( ctxtFactory )
    , federateName_( federateName )
    , federateHandle_( federateHandle )
    , pOfferSender_( new InteractionSender< interactions::TMR_OfferTransferModellingResponsibility >( *this, builder ) )
    , pRequestSender_( new InteractionSender< interactions::TMR_RequestTransferModellingResponsibility >( *this, builder ) )
    , ownershipStrategy_ ( ownershipStrategy )
    , ownershipController_( controller )
    , logger_( logger )
    , agentResolver_( agentResolver )
    , callsignResolver_( callsignResolver )
{
    if ( !pOfferSender_->IsSupported() )
    {
        logger.LogWarning("TMR_OfferTransferModellingResponsibility interaction not supported.  OwnershipTransfer will be disabled.");
    }   
    if ( !pRequestSender_->IsSupported() )
    {
        logger.LogWarning("TMR_RequestTransferModellingResponsibility interaction not supported.  OwnershipTransfer will be disabled.");
    }   
}

// -----------------------------------------------------------------------------
// Name: Netn2TransferSender destructor
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
Netn2TransferSender::~Netn2TransferSender()
{
    // NOTHING
}

namespace
{
    unsigned int CreateTransactionID( ::hla::VariableLengthData& tag, interactions::TransactionId& transactionID, const ::hla::FederateIdentifier federateHandle,
           const ContextFactory_ABC& ctxtFactory )
    {
        unsigned int retval = 0;
        if( tag.Size() == 0 )
        {
            retval = ctxtFactory.Create();
            transactionID.federateHandle = federateHandle;
            transactionID.transactionCounter = retval;
        }
        else
        {
            ::hla::Deserializer deser( tag.Buffer(), tag.Size() );
            transactionID.Deserialize( deser );
            retval = transactionID.transactionCounter;
        }
        return retval;
    }
}
// -----------------------------------------------------------------------------
// Name: Netn2TransferSender::RequestTransfer
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
void Netn2TransferSender::RequestTransfer(const std::string& agentID, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag, uint32_t capability )
{
    interactions::TMR_RequestTransferModellingResponsibility transfer;
    transfer.requestFederate = UnicodeString( federateName_ );
    const unsigned int reqId = CreateTransactionID( tag, transfer.transactionID, federateHandle_, ctxtFactory_ );
    std::vector< char > uniqueId( GetUniqueId( agentID, agentResolver_, callsignResolver_ ) );
    if( uniqueId.size() == 0 )
    {
        logger_.LogError( std::string( "Trying to transfer unknown entity " ) + agentID );
        return;
    }
    transfer.instances.list.push_back( NETN_UUID( uniqueId ) );
    for( auto it=attributes.begin(); attributes.end()!=it; ++it )
    {
        const ::hla::AttributeIdentifier& attr = *it;
        transfer.attributes.list.push_back( UnicodeString( attr.ToString() ) );
    }
    transfer.capabilityType = capability;
    transfer.transferType = type == E_EntityPush ? static_cast< uint32_t >( interactions::TMR::Divest )
                                                 : static_cast< uint32_t >( interactions::TMR::Acquire );
    pRequestSender_->Send( transfer );
    callbacks_.insert( std::make_pair( reqId, callback ) );
}

// -----------------------------------------------------------------------------
// Name: Netn2TransferSender::Receive
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
void Netn2TransferSender::Receive( interactions::TMR_RequestTransferModellingResponsibility& request )
{
    if( request.responseFederate.str().size() != 0 && request.responseFederate.str() != federateName_ )
        return;

    interactions::TMR_OfferTransferModellingResponsibility reply;
    reply.transactionID = request.transactionID;
    reply.requestFederate = request.requestFederate;
    reply.responseFederate = UnicodeString( federateName_ );

    if( request.instances.list.size() == 0 )
    {
        reply.isOffering = false;
        reply.reason = static_cast< uint32_t >( interactions::TMR::CapabilityDoesNotMatch );
        pOfferSender_->Send( reply );
        return;
    }
    std::vector< ::hla::AttributeIdentifier > attributes( request.attributes.list.size(), ::hla::AttributeIdentifier("") );
    std::transform( request.attributes.list.begin(), request.attributes.list.end(), attributes.begin(), std::mem_fun_ref( &UnicodeString::str ) );
    bool resp( true );
    interactions::TMR::TransferTypeEnum32 transferType( static_cast< interactions::TMR::TransferTypeEnum32 >( request.transferType ) );
    for( auto it = request.instances.list.begin(); it!=request.instances.list.end(); ++it )
    {
        const NETN_UUID& uniqueId = *it;
        std::string agentId( GetAgentId( uniqueId.data(), agentResolver_, callsignResolver_ ) );
        if( agentId.size() == 0 )
        {
            logger_.LogError( std::string( "Trying to transfer unknown entity " ) + uniqueId.str() );
            resp = false;
            break;
        }
        else
        {
            if( transferType == interactions::TMR::Acquire )
                resp &= ownershipStrategy_.AcceptDivestiture( agentId, true );
            else if( transferType == interactions::TMR::Divest )
                resp &= ownershipStrategy_.AcceptAcquisition( agentId, true );
        }
        if( !resp )
            break;
    }
    reply.isOffering =  resp;
    if( !resp )
        reply.reason = static_cast< uint32_t >( interactions::TMR::Reason_Other );
    pOfferSender_->Send( reply );

    if( resp )
    {
        for( auto it = request.instances.list.begin(); it!=request.instances.list.end(); ++it )
        {
            const NETN_UUID& uniqueId = *it;
            std::string agentId( GetAgentId( uniqueId.data(), agentResolver_, callsignResolver_ ) );
            ::hla::Serializer ser;
            request.transactionID.Serialize( ser );
            boost::scoped_array< uint8_t > buff( new uint8_t[ ser.GetSize() ] );
            ser.CopyTo( buff.get() );
            ::hla::VariableLengthData tag( buff.get(), ser.GetSize() );
            if( transferType == interactions::TMR::Acquire )
                ownershipController_.PerformDivestiture( agentId, attributes, tag );
            else if( transferType == interactions::TMR::Divest )
                ownershipController_.PerformAcquisition( agentId, attributes, tag );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Netn2TransferSender::Receive
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
void Netn2TransferSender::Receive( interactions::TMR_OfferTransferModellingResponsibility& offer )
{
    T_Callbacks::iterator it;
    if( offer.requestFederate.str() == federateName_ &&
        ( it=callbacks_.find( offer.transactionID.transactionCounter ) ) != callbacks_.end() )
    {
        (it->second)( offer.isOffering );
        callbacks_.erase( offer.transactionID.transactionCounter );
    }
}

// -----------------------------------------------------------------------------
// Name: Netn2TransferSender::RequestTransfer
// Created: AHC 2013-07-03
// -----------------------------------------------------------------------------
void Netn2TransferSender::RequestTransfer( const std::vector< std::string >& agentIDs, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag, uint32_t capability )
{
    interactions::TMR_RequestTransferModellingResponsibility transfer;
    transfer.requestFederate = UnicodeString( federateName_ );
    const unsigned int reqId = CreateTransactionID( tag, transfer.transactionID, federateHandle_, ctxtFactory_ );
    for( auto it = agentIDs.begin(); agentIDs.end() != it ; ++it )
    {
        const std::string& agentID = *it;
        std::vector< char > uniqueId( GetUniqueId( agentID, agentResolver_, callsignResolver_ ) );
        if( uniqueId.size() == 0 )
        {
            logger_.LogError( std::string( "Trying to transfer unknown entity " ) + agentID );
            return;
        }
        transfer.instances.list.push_back( NETN_UUID( uniqueId ) );
    }
    for( auto it=attributes.begin(); attributes.end()!=it; ++it )
    {
        const ::hla::AttributeIdentifier& attr = *it;
        transfer.attributes.list.push_back( UnicodeString( attr.ToString() ) );
    }
    transfer.capabilityType = capability;
    transfer.transferType = type == E_EntityPush ? static_cast< uint32_t >( interactions::TMR::Divest )
                                                 : static_cast< uint32_t >( interactions::TMR::Acquire );
    pRequestSender_->Send( transfer );
    callbacks_.insert( std::make_pair( reqId, callback ) );
}
