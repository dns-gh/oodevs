// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ExternalOwnershipPolicy.h"

#include "OwnershipStrategy_ABC.h"
#include "OwnershipController_ABC.h"
#include "Interactions.h"
#include "InteractionSender.h"
#include "LocalAgentResolver_ABC.h"
#include "CallsignResolver_ABC.h"
#include "TransferSender_ABC.h"
#include "RemoteAgentSubject_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/Interaction.h>
#include <hla/AttributeIdentifier.h>
#include <hla/VariableLengthData.h>
#include <hla/Serializer.h>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_array.hpp>
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
        try
        {
            unsigned long simId = callsignResolver.ResolveSimulationIdentifier( uniqueID );
            return agentResolver.Resolve( simId );
        }
        catch( const std::exception& )
        {
            // NOTHING
        }
        return "";
    }
    template< typename C >
    bool GetAgents( const std::vector< NETN_UUID >& instances, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver,
            C& agents, dispatcher::Logger_ABC& logger )
    {
        for( auto it = instances.begin(); it != instances.end(); ++it )
        {
            const NETN_UUID& uniqueId = *it;
            const std::string agentId( GetAgentId( uniqueId.data(), agentResolver, callsignResolver ) );
            if( agentId.empty() )
            {
                logger.LogError( std::string( "Trying to transfer unknown entity " ) + uniqueId.str() );
                return false;
            }
            agents.insert( agents.end(), agentId );
        }
        return true;
    }
}

struct ExternalOwnershipPolicy::PendingRequest : boost::noncopyable
{
    PendingRequest( ExternalOwnershipPolicy& policy, OwnershipController_ABC& controller,
                std::vector< std::string >& instances, std::vector< ::hla::AttributeIdentifier >& attributes,
                TransferSender_ABC::TransferType transfer, const interactions::TMR& tmr, const ::hla::VariableLengthData& tag )
            : policy_( policy )
            , ownershipController_( controller )
            , instances_( instances )
            , attributes_( attributes )
            , transfer_( transfer )
            , transactionID_( tmr.transactionID )
            , requestFederate_( tmr.requestFederate )
            , responseFederate_( tmr.responseFederate )
            , pendings_( instances.begin(), instances.end() )
            , completed_( false )
            , tag_( tag )
            {}
    void Callback( bool reply )
    {
        if( reply )
        {
            if( transfer_ == TransferSender_ABC::E_EntityPull )
                std::for_each( instances_.begin(), instances_.end(), [&](const std::string& id)
                    {
                        ownershipController_.PerformAcquisition( id, attributes_, tag_ );
                    });
            else
                std::for_each( instances_.begin(), instances_.end(), [&](const std::string& id)
                    {
                        ownershipController_.PerformDivestiture( id, attributes_, tag_ );
                    });
        }
        else
        {
            completed_ = true;
            policy_.TransferCompleted( false, transactionID_, requestFederate_, responseFederate_);
        }
    }

    void Divested( const std::string& identifier )
    {
        if( transfer_ == TransferSender_ABC::E_EntityPush )
        {
            pendings_.erase( identifier );
            if( pendings_.empty() )
            {
                completed_ = true;
                policy_.TransferCompleted( true, transactionID_, requestFederate_, responseFederate_);
            }
        }
    }

    void Acquired( const std::string& identifier )
    {
        if( transfer_ == TransferSender_ABC::E_EntityPull )
        {
            pendings_.erase( identifier );
            if( pendings_.empty() )
            {
                completed_ = true;
                policy_.TransferCompleted( true, transactionID_, requestFederate_, responseFederate_);
            }
        }
    }

    bool Completed() const { return completed_; }

private:
    ExternalOwnershipPolicy& policy_;
    OwnershipController_ABC& ownershipController_;
    std::vector< std::string > instances_;
    std::vector< ::hla::AttributeIdentifier > attributes_;
    TransferSender_ABC::TransferType transfer_;
    interactions::TransactionId transactionID_;
    UnicodeString requestFederate_;
    UnicodeString responseFederate_;
    std::set< std::string > pendings_;
    bool completed_;
    ::hla::VariableLengthData tag_;
};

ExternalOwnershipPolicy::ExternalOwnershipPolicy(const std::string& federateName, const InteractionBuilder& builder,
        OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger, RemoteAgentSubject_ABC& subject,
        const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver, TransferSender_ABC& transferSender)
    : federateName_( federateName )
    , pOfferSender_( new InteractionSender< interactions::TMR_OfferTransferModellingResponsibility >( *this, builder ) )
    , pInitiateSender_( new InteractionSender< interactions::TMR_InitiateTransferModellingResponsibility >( *this, builder ) )
    , pResultSender_( new InteractionSender< interactions::TMR_TransferResult >( *this, builder ) )
    , ownershipController_( controller )
    , logger_( logger )
    , subject_( subject )
    , agentResolver_( agentResolver )
    , callsignResolver_( callsignResolver )
    , transferSender_( transferSender )
{
    subject_.Register( *this );
}

ExternalOwnershipPolicy::~ExternalOwnershipPolicy()
{
    subject_.Unregister( *this );
}

void ExternalOwnershipPolicy::Receive( interactions::TMR_InitiateTransferModellingResponsibility& interaction )
{
    logger_.LogInfo("Received external ownership request from " + interaction.requestFederate.str() );

    if( interaction.requestFederate.str() != federateName_ )
        return;

    interactions::TMR_OfferTransferModellingResponsibility reply;
    reply.requestFederate = interaction.requestFederate;
    reply.responseFederate = interaction.responseFederate;
    reply.transactionID = interaction.transactionID;
    reply.respondent = UnicodeString( federateName_ );
    const interactions::TMR::TransferTypeEnum32 type = static_cast< interactions::TMR::TransferTypeEnum32 >( interaction.transferType );
    if( interaction.instances.list.empty() )
    {
        logger_.LogWarning("External ownership request with no instances ");
        reply.isOffering = false;
        reply.reason = static_cast< uint32_t >( interactions::TMR::CapabilityDoesNotMatch );
    }
    else
    {
        switch(type)
        {
        case interactions::TMR::Transfer_Other:
            reply.isOffering = false;
            reply.reason = static_cast< uint32_t >( interactions::TMR::Reason_Other );
            break;
        case interactions::TMR::Acquire:
            {
                std::set< std::string > candidates;
                const bool resp = GetAgents( interaction.instances.list, agentResolver_, callsignResolver_, candidates, logger_);
                if( resp )
                    reply.isOffering = std::includes( remoteObjects_.begin(), remoteObjects_.end(), candidates.begin(), candidates.end() );
                else
                    reply.isOffering = false;
                if( !reply.isOffering )
                {
                    logger_.LogWarning("External ownership acquire with invalid units");
                    reply.reason = static_cast< uint32_t >( interactions::TMR::Reason_Other );
                }
            }
            break;
        case interactions::TMR::Divest:
            {
                std::set< std::string > candidates;
                const bool resp = GetAgents( interaction.instances.list, agentResolver_, callsignResolver_, candidates, logger_);
                if( resp )
                    reply.isOffering = std::includes( localObjects_.begin(), localObjects_.end(), candidates.begin(), candidates.end() );
                else
                    reply.isOffering = false;
                if( !reply.isOffering )
                {
                    logger_.LogWarning("External ownership divest with invalid units");
                    reply.reason = static_cast< uint32_t >( interactions::TMR::Reason_Other );
                }
            }
            break;
        case interactions::TMR::AcquireWithoutNegotiating:
            {
                std::set< std::string > candidates;
                const bool resp = GetAgents( interaction.instances.list, agentResolver_, callsignResolver_, candidates, logger_);
                if( resp )
                    reply.isOffering = std::includes( remoteObjects_.begin(), remoteObjects_.end(), candidates.begin(), candidates.end() );
                else
                    reply.isOffering = false;
                if( !reply.isOffering )
                    reply.reason = static_cast< uint32_t >( interactions::TMR::Reason_Other );
            }
            break;
        default:
            reply.isOffering = false;
            reply.reason = static_cast< uint32_t >( interactions::TMR::Reason_Other );
            break;
        }
    }
    pOfferSender_->Send( reply );
    if( reply.isOffering )
    {
        ::hla::Serializer ser;
        interaction.transactionID.Serialize( ser );
        boost::scoped_array< uint8_t > buff( new uint8_t[ ser.GetSize() ] );
        ser.CopyTo( buff.get() );
        ::hla::VariableLengthData tag( buff.get(), ser.GetSize() );

        std::vector< std::string > candidates;
        GetAgents( interaction.instances.list, agentResolver_, callsignResolver_, candidates, logger_);
        std::vector< ::hla::AttributeIdentifier > attributes( interaction.attributes.list.size(), ::hla::AttributeIdentifier("") );
        std::transform( interaction.attributes.list.begin(), interaction.attributes.list.end(), attributes.begin(), std::mem_fun_ref( &UnicodeString::str ) );
        switch(type)
        {
        case interactions::TMR::Acquire:
        case interactions::TMR::Divest:
            {
                logger_.LogInfo("Negotiating external request with other federate");
                const TransferSender_ABC::TransferType transferType = (type == interactions::TMR::Acquire ? TransferSender_ABC::E_EntityPull: TransferSender_ABC::E_EntityPush );
                T_PendingRequestPtr request( new PendingRequest( *this, ownershipController_, candidates, attributes, transferType, interaction, tag ) );
                requests_[ interaction.transactionID.transactionCounter ] = request;
                transferSender_.RequestTransfer( candidates, boost::bind( &ExternalOwnershipPolicy::PendingRequest::Callback, request.get(), _1 ),
                        transferType, attributes, tag, interaction.capabilityType );
            }
            break;
        case interactions::TMR::AcquireWithoutNegotiating:
            {
                std::for_each( candidates.begin(), candidates.end(), [&](const std::string& id)
                    {
                        ownershipController_.PerformAcquisition( id, attributes, tag );
                    });
            }
            break;
        }
    }
}

void ExternalOwnershipPolicy::Receive( interactions::TMR_OfferTransferModellingResponsibility& )
{
    // NOTHING
}

void ExternalOwnershipPolicy::Receive( interactions::TMR_TransferResult& )
{
    //NOTHING
}

void ExternalOwnershipPolicy::RemoteCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    remoteObjects_.insert( identifier );
}

void ExternalOwnershipPolicy::RemoteDestroyed( const std::string& identifier )
{
    remoteObjects_.erase( identifier );
}

void ExternalOwnershipPolicy::LocalCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    localObjects_.insert( identifier );
}

void ExternalOwnershipPolicy::LocalDestroyed( const std::string& identifier )
{
    localObjects_.erase( identifier );
}

void ExternalOwnershipPolicy::CleanupRequests()
{
    T_Requests::iterator it = requests_.begin();
    while( it != requests_.end() )
    {
        if( it->second->Completed() )
            it = requests_.erase( it );
        else
            ++it;
    }
}
void ExternalOwnershipPolicy::Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    localObjects_.erase( identifier );
    remoteObjects_.insert( identifier );
    std::for_each( requests_.begin(), requests_.end(), [&](T_Requests::reference request)
        {
            request.second->Divested( identifier );
        });
    CleanupRequests();
}

void ExternalOwnershipPolicy::Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    remoteObjects_.erase( identifier );
    localObjects_.insert( identifier );
    std::for_each( requests_.begin(), requests_.end(), [&](T_Requests::reference request)
        {
            request.second->Acquired( identifier );
        });
    CleanupRequests();
}

void ExternalOwnershipPolicy::TransferCompleted( bool isOk, const interactions::TransactionId& id, const UnicodeString& requestFederate, const UnicodeString& responseFederate )
{
    interactions::TMR_TransferResult result;
    result.transactionID = id;
    result.requestFederate = requestFederate;
    result.responseFederate = responseFederate;
    result.transferOk = isOk;
    pResultSender_->Send( result );
    logger_.LogInfo("External ownership request completed");
}
