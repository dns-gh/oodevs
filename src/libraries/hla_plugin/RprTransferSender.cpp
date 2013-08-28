// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RprTransferSender.h"
#include "OwnershipStrategy_ABC.h"
#include "ContextFactory_ABC.h"
#include "OwnershipController_ABC.h"
#include "Interactions.h"
#include <cassert>
#include "InteractionSender.h"
#include <hla/Interaction.h>
#include <hla/AttributeIdentifier.h>
#include <hla/VariableLengthData.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RprTransferSender::T_RequestInfo constructor
// Created: AHC 2012-10-29
// -----------------------------------------------------------------------------
RprTransferSender::T_RequestInfo::T_RequestInfo( const std::string& n, TransferRequestCallback cb )
    : name_( n )
    , callback_( cb )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RprTransferSender constructor
// Created: AHC 2012-02-23
// -----------------------------------------------------------------------------
RprTransferSender::RprTransferSender( const rpr::EntityIdentifier federateID, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder,
        OwnershipStrategy_ABC& ownershipStrategy, OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger  )
    : ctxtFactory_( ctxtFactory )
    , federateID_( federateID )
    , pAcknowledgeSender_( new InteractionSender< interactions::Acknowledge >( *this, builder ) )
    , pTransferSender_( new InteractionSender< interactions::TransferControl >( *this, builder ) )
    , ownershipStrategy_ ( ownershipStrategy )
    , ownershipController_( controller )
    , logger_( logger )
{
    if (!pAcknowledgeSender_->IsSupported())
    {
        logger.LogWarning("Acknowledge interaction not supported.  OwnershipTransfer will be disabled.");
    }   
    if (!pTransferSender_->IsSupported())
    {
        logger.LogWarning("TransferControl interaction not supported.  OwnershipTransfer will be disabled.");
    }   
}

// -----------------------------------------------------------------------------
// Name: RprTransferSender destructor
// Created: AHC 2012-03-22
// -----------------------------------------------------------------------------
RprTransferSender::~RprTransferSender()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RprTransferSender::RequestTransfer
// Created: AHC 2012-02-23
// -----------------------------------------------------------------------------
void RprTransferSender::RequestTransfer(const std::string& agentID, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& /*attributes*/, ::hla::VariableLengthData& tag, uint32_t )
{
    unsigned int reqId = ctxtFactory_.Create();
    interactions::TransferControl transfer;
    transfer.originatingEntity = federateID_;
    transfer.receivingEntity = rpr::EntityIdentifier( 0xFFFF, 0xFFFF, 0xFFFF );
    transfer.transferEntity = Omt13String(agentID);
    transfer.requestIdentifier = reqId;
    transfer.transferType = type == E_EntityPush ? static_cast<uint8_t>( interactions::TransferControl::E_EntityPush )
                                                 : static_cast<uint8_t>( interactions::TransferControl::E_EntityPull );
    pTransferSender_->Send( transfer );
    callbacks_.insert( std::make_pair( reqId, T_RequestInfo( agentID, callback ) ) );
    tag=::hla::VariableLengthData(reqId);
}

// -----------------------------------------------------------------------------
// Name: RprTransferSender::Receive
// Created: AHC 2012-02-23
// -----------------------------------------------------------------------------
void RprTransferSender::Receive( interactions::TransferControl& interaction )
{
    if( rpr::EntityIdentifier::Match(interaction.receivingEntity, federateID_ ) )
    {
        bool resp( false );
        interactions::TransferControl::TransferTypeEnum8 transferType( static_cast<interactions::TransferControl::TransferTypeEnum8>( interaction.transferType ) );
        if( transferType == interactions::TransferControl::E_EntityPull )
            resp = ownershipStrategy_.AcceptDivestiture( interaction.transferEntity.str(), true );
        else if( transferType == interactions::TransferControl::E_EntityPush )
            resp = ownershipStrategy_.AcceptAcquisition( interaction.transferEntity.str(), true );
        interactions::Acknowledge reply;
        reply.originatingEntity = federateID_;
        reply.receivingEntity = interaction.originatingEntity;
        reply.requestIdentifier = interaction.requestIdentifier;
        reply.acknowledgeFlag = 5;
        reply.responseFlag =  static_cast<uint16_t>( resp ?
                        interactions::Acknowledge::E_AbleToComply :
                        interactions::Acknowledge::E_UnableToComply );
        pAcknowledgeSender_->Send( reply );
        std::vector< ::hla::AttributeIdentifier > attributes;
        ::hla::VariableLengthData tag( interaction.requestIdentifier );
        if( resp && transferType == interactions::TransferControl::E_EntityPull )
            ownershipController_.PerformDivestiture( interaction.transferEntity.str(), attributes, tag );
        else if( resp && transferType == interactions::TransferControl::E_EntityPush )
            ownershipController_.PerformAcquisition( interaction.transferEntity.str(), attributes, tag );
    }
}

// -----------------------------------------------------------------------------
// Name: RprTransferSender::Receive
// Created: AHC 2012-02-23
// -----------------------------------------------------------------------------
void RprTransferSender::Receive( interactions::Acknowledge& interaction )
{
    T_Callbacks::iterator it;
    if( rpr::EntityIdentifier::Match(interaction.receivingEntity, federateID_ ) &&
        ( it=callbacks_.find( interaction.requestIdentifier ) ) != callbacks_.end() )
    {
        (it->second.callback_)( interaction.responseFlag == interactions::Acknowledge::E_AbleToComply );
        callbacks_.erase( interaction.requestIdentifier );
    }
}

// -----------------------------------------------------------------------------
// Name: RprTransferSender::RequestTransfer
// Created: AHC 2013-07-03
// -----------------------------------------------------------------------------
void RprTransferSender::RequestTransfer( const std::vector< std::string >& , const TransferRequestCallback& , TransferType , const std::vector< ::hla::AttributeIdentifier >&, ::hla::VariableLengthData& tag, uint32_t )
{
    tag=::hla::VariableLengthData( ctxtFactory_.Create() );
    logger_.LogWarning("Attempting to perform multiple ownership transfers using RPR.");
}
