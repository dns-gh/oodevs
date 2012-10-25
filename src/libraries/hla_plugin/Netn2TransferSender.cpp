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
#include <boost/foreach.hpp>
#include <cassert>

using namespace plugins::hla;

namespace
{
    std::string GetUniqueId( const std::string& agentID, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    {
        unsigned long simId = agentResolver.Resolve( agentID );
        return callsignResolver.ResolveUniqueId( simId );
    }
    std::string GetAgentId( const std::string& uniqueID, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    {
        unsigned long simId = callsignResolver.ResolveSimulationIdentifier( uniqueID );
        return agentResolver.Resolve( simId );
    }
}

// -----------------------------------------------------------------------------
// Name: Netn2TransferSender constructor
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
Netn2TransferSender::Netn2TransferSender( const std::string& federateName, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder,
        OwnershipStrategy_ABC& ownershipStrategy, OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger,
        const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    : ctxtFactory_( ctxtFactory )
    , federateName_( federateName )
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

// -----------------------------------------------------------------------------
// Name: Netn2TransferSender::RequestTransfer
// Created: AHC 2012-10-25
// -----------------------------------------------------------------------------
void Netn2TransferSender::RequestTransfer(const std::string& agentID, const TransferRequestCallback& callback, TransferType type)
{
    unsigned int reqId = ctxtFactory_.Create();
    interactions::TMR_RequestTransferModellingResponsibility transfer;
    transfer.transactionID.federateName = UnicodeString( federateName_ );
    transfer.transactionID.transactionCounter = reqId;
    transfer.requestFederate = UnicodeString( federateName_ );
    std::string uniqueId( GetUniqueId( agentID, agentResolver_, callsignResolver_ ) );
    if( uniqueId.size() == 0 )
    {
        logger_.LogError( std::string( "Trying to tranfer unknown entity " ) + agentID );
        return;
    }
    transfer.instances.list.push_back( NETN_UUID( uniqueId ) );
    // FIXME transfer.attributes
    transfer.capabilityType = static_cast< uint32 >( interactions::TMR::TotalTransfer );
    transfer.transferType = type == E_EntityPush ? static_cast< uint32 >( interactions::TMR::Divest )
                                                 : static_cast< uint32 >( interactions::TMR::Acquire );
    pRequestSender_->Send( transfer );
    callbacks_.insert( std::make_pair( reqId, std::make_pair( agentID, callback ) ) );
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
        reply.reason = static_cast< uint32 >( interactions::TMR::CapabilityDoesNotMatch );
        pOfferSender_->Send( reply );
        return;
    }

    bool resp( true );
    interactions::TMR::TransferTypeEnum32 transferType( static_cast< interactions::TMR::TransferTypeEnum32 >( request.transferType ) );
    BOOST_FOREACH( const NETN_UUID& uniqueId, request.instances.list )
    {
        std::string agentId( GetAgentId( uniqueId.str(), agentResolver_, callsignResolver_ ) );
        if( agentId.size() == 0 )
        {
            logger_.LogError( std::string( "Trying to tranfer unknown entity " ) + uniqueId.str() );
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
        reply.reason = static_cast< uint32 >( interactions::TMR::Reason_Other );
    pOfferSender_->Send( reply );

    if( resp )
    {
        BOOST_FOREACH( const NETN_UUID& uniqueId, request.instances.list )
        {
            std::string agentId( GetAgentId( uniqueId.str(), agentResolver_, callsignResolver_ ) );
            if( transferType == interactions::TMR::Acquire )
                ownershipController_.PerformDivestiture( agentId );
            else if( transferType == interactions::TMR::Divest )
                ownershipController_.PerformAcquisition( agentId );
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
        (it->second.second)( offer.isOffering );
        callbacks_.erase( offer.transactionID.transactionCounter );
    }
}

