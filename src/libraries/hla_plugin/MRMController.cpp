// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "MRMController.h"
#include "InteractionSender.h"
#include "InteractionBuilder.h"
#include "InteractionsNetn.h"
#include "RemoteAgentSubject_ABC.h"
#include "LocalAgentResolver_ABC.h"
#include "CallsignResolver_ABC.h"
#include <hla/Interaction.h>
#include <dispatcher/Logger_ABC.h>
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

namespace
{
    std::string GetAgentId( const NETN_UUID& uniqueID, const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    {
        try
        {
            unsigned long simId = callsignResolver.ResolveSimulationIdentifier( uniqueID.data() );
            return agentResolver.Resolve( simId );
        }
        catch( const std::exception& )
        {
            // NOTHING
        }
        return std::string("");
    }
}

MRMController::Info::Info()
{
    // NOTHING
}

MRMController::Info::Info( const std::string& entity_val, bool is_aggregate )
    : entity( entity_val )
    , aggregate( is_aggregate )
{
    // NOTHING
}

MRMController::MRMController( const std::string& federateName, const InteractionBuilder& builder, RemoteAgentSubject_ABC& subject, dispatcher::Logger_ABC& logger,
        const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    : federateName_( federateName )
    , subject_( subject )
    , logger_( logger )
    , agentResolver_( agentResolver )
    , callsignResolver_( callsignResolver )
    , disaggregationRequestSender_( new InteractionSender< interactions::MRM_DisaggregationRequest >( *this, builder ) )
    , disaggregationResponseSender_( new InteractionSender< interactions::MRM_DisaggregationResponse >( *this, builder ) )
    , actionCompleteSender_( new InteractionSender< interactions::MRM_ActionComplete >( *this, builder ) )
    , aggregationRequestSender_( new InteractionSender< interactions::MRM_AggregationRequest >( *this, builder ) )
    , aggregationResponseSender_( new InteractionSender< interactions::MRM_AggregationResponse >( *this, builder ) )
{
    subject.Register( *this );
}

MRMController::~MRMController()
{
    subject_.Unregister( *this );
}

void MRMController::Receive( interactions::MRM_DisaggregationRequest& interaction )
{
    if( interaction.aggregateFederate.str() != federateName_ )
        return;

    logger_.LogInfo("Received MRM disaggregation " + boost::lexical_cast< std::string >( interaction.transactionID.transactionCounter ) +  " request for " + interaction.aggregateUUID.str() );

    interactions::MRM_DisaggregationResponse reply;
    reply.aggregateFederate = interaction.aggregateFederate;
    reply.higherResolutionFederate = interaction.higherResolutionFederate;
    reply.transactionID = interaction.transactionID;

    const std::string agentID = GetAgentId( interaction.aggregateUUID, agentResolver_, callsignResolver_);
    if( agentID.empty() )
    {
        logger_.LogError( std::string( "MRM Trying to transfer unknown entity " ) + interaction.aggregateUUID.str() );
        reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_UnableToComply ) ;
        reply.nonComplianceReason = interactions::MRM_Object::noncompliancereasonenum_UnitUnknown;
        disaggregationResponseSender_->Send( reply );
        return;
    }
    if( localObjects_.find( agentID ) == localObjects_.end() )
    {
        logger_.LogError( std::string( "MRM Trying to transfer not owned entity " ) + interaction.aggregateUUID.str() );
        reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_UnableToComply ) ;
        reply.nonComplianceReason = interactions::MRM_Object::noncompliancereasonenum_UnitNotOwned;
        disaggregationResponseSender_->Send( reply );
        return;
    }
    if( disaggregatedObjects_.find( agentID ) != disaggregatedObjects_.end() )
    {
        logger_.LogError( std::string( "MRM Trying to transfer already disaggregated entity " ) + interaction.aggregateUUID.str() );
        reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_UnableToComply ) ;
        reply.nonComplianceReason = interactions::MRM_Object::noncompliancereasonenum_UnitAlreadyDisaggregated;
        disaggregationResponseSender_->Send( reply );
        return;
    }
    reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_AbleToComply );
    reply.nonComplianceReason = 0;
    disaggregationResponseSender_->Send( reply );
    pendingRequests_[ interaction.transactionID ] = Info( agentID, false );
}

void MRMController::Receive( interactions::MRM_ActionComplete& interaction )
{
    if( interaction.aggregateFederate.str() != federateName_ )
        return;
    logger_.LogInfo("Completed MRM action " + boost::lexical_cast< std::string >( interaction.transactionID.transactionCounter ) + " : " +
            boost::lexical_cast< std::string >( interaction.result ) );
    auto it = pendingRequests_.find( interaction.transactionID );
    if( interaction.result )
    {
        if( pendingRequests_.end() != it )
        {
            const Info& inf = it->second;
            if( !inf.aggregate )
                disaggregatedObjects_.insert( inf.entity );
            else
                disaggregatedObjects_.erase( inf.entity );
        }
    }
    pendingRequests_.erase( interaction.transactionID );
}

void MRMController::Receive( interactions::MRM_DisaggregationResponse& )
{
    // NOTHING
}

void MRMController::RemoteCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    remoteObjects_.insert( identifier );
}

void MRMController::RemoteDestroyed( const std::string& identifier )
{
    remoteObjects_.erase( identifier );
}

void MRMController::LocalCreated( const std::string& identifier, HlaClass_ABC& /*hlaClass*/, HlaObject_ABC& /*object*/ )
{
    localObjects_.insert( identifier );
}

void MRMController::LocalDestroyed( const std::string& identifier )
{
    localObjects_.erase( identifier );
}

void MRMController::Divested( const std::string& identifier, const T_AttributeIdentifiers& /*attributes*/ )
{
    localObjects_.erase( identifier );
    remoteObjects_.insert( identifier );
}

void MRMController::Acquired( const std::string& identifier, const T_AttributeIdentifiers& /*attributes*/ )
{
    localObjects_.insert( identifier );
    remoteObjects_.erase( identifier );
}

void MRMController::Receive( interactions::MRM_AggregationRequest& interaction )
{
    if( interaction.aggregateFederate.str() != federateName_ )
        return;

    logger_.LogInfo("Received MRM aggregation request" + boost::lexical_cast< std::string >( interaction.transactionID.transactionCounter ) +  " request for " + interaction.aggregateUUID.str() );

    interactions::MRM_AggregationResponse reply;
    reply.aggregateFederate = interaction.aggregateFederate;
    reply.higherResolutionFederate = interaction.higherResolutionFederate;
    reply.transactionID = interaction.transactionID;

    std::string agentID = GetAgentId( interaction.aggregateUUID, agentResolver_, callsignResolver_);
    if( agentID.empty() )
    {
        logger_.LogError( std::string( "MRM Trying to transfer unknown entity " ) + interaction.aggregateUUID.str() );
        reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_UnableToComply ) ;
        reply.nonComplianceReason = interactions::MRM_Object::noncompliancereasonenum_UnitUnknown;
        aggregationResponseSender_->Send( reply );
        return;
    }
    if( remoteObjects_.find( agentID ) == remoteObjects_.end() )
    {
        logger_.LogError( std::string( "MRM Trying to transfer owned entity " ) + interaction.aggregateUUID.str() );
        reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_UnableToComply ) ;
        reply.nonComplianceReason = interactions::MRM_Object::noncompliancereasonenum_UnitNotOwned;
        aggregationResponseSender_->Send( reply );
        return;
    }
    if( disaggregatedObjects_.find( agentID ) == disaggregatedObjects_.end() )
    {
        logger_.LogError( std::string( "MRM Trying to transfer not disaggregated entity " ) + interaction.aggregateUUID.str() );
        reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_UnableToComply ) ;
        reply.nonComplianceReason = interactions::MRM_Object::noncompliancereasonenum_UnitAlreadyAggregated;
        aggregationResponseSender_->Send( reply );
        return;
    }
    reply.acknowledge = uint16_t( interactions::MRM_Object::responseflagenum16_AbleToComply ) ;
    reply.nonComplianceReason = 0;
    aggregationResponseSender_->Send( reply );
    pendingRequests_[ interaction.transactionID ] = Info( agentID, true );
}

void MRMController::Receive( interactions::MRM_AggregationResponse& )
{
    // NOTHING
}
