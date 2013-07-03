// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NullTransferSender.h"
#include "ContextFactory_ABC.h"
#include "Interactions.h"
#include "OwnershipStrategy_ABC.h"
#include "OwnershipController_ABC.h"
#include <cassert>
#include "InteractionSender.h"
#include <hla/Interaction.h>
#include <hla/AttributeIdentifier.h>
#include "dispatcher/Logger_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NullTransferSender constructor
// Created: AHC 2012-03-22
// -----------------------------------------------------------------------------
NullTransferSender::NullTransferSender( const rpr::EntityIdentifier federateID, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder,
        OwnershipStrategy_ABC& ownershipStrategy, OwnershipController_ABC& controller , dispatcher::Logger_ABC& logger  )
    : ctxtFactory_( ctxtFactory )
    , federateID_( federateID )
    , pAcknowledgeSender_( new InteractionSender< interactions::Acknowledge >( *this, builder ) )
    , pTransferSender_( new InteractionSender< interactions::TransferControl >( *this, builder ) )
    , ownershipStrategy_ ( ownershipStrategy )
    , ownershipController_( controller )
{
    if (!pAcknowledgeSender_->IsSupported())
        logger.LogWarning("Acknowledge interaction not supported.  OwnershipTransfer will be disabled.");
    if (!pTransferSender_->IsSupported())
        logger.LogWarning("TransferContral interaction not supported.  OwnershipTransfer will be disabled.");
}

// -----------------------------------------------------------------------------
// Name: NullTransferSender destructor
// Created: AHC 2012-03-22
// -----------------------------------------------------------------------------
NullTransferSender::~NullTransferSender()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullTransferSender::RequestTransfer
// Created: AHC 2012-03-22
// -----------------------------------------------------------------------------
void NullTransferSender::RequestTransfer(const std::string& /*agentID*/, const TransferRequestCallback& callback, TransferType /*type*/, const std::vector< ::hla::AttributeIdentifier >& /*attributes*/ )
{
    callback( true );
    return;
}

// -----------------------------------------------------------------------------
// Name: NullTransferSender::Receive
// Created: AHC 2012-03-22
// -----------------------------------------------------------------------------
void NullTransferSender::Receive( interactions::TransferControl& interaction )
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
        reply.responseFlag =  static_cast< uint16_t >( resp ?
                        interactions::Acknowledge::E_AbleToComply :
                        interactions::Acknowledge::E_UnableToComply );
        pAcknowledgeSender_->Send( reply );
        std::vector< ::hla::AttributeIdentifier > attributes;
        if( resp && transferType == interactions::TransferControl::E_EntityPull )
            ownershipController_.PerformDivestiture( interaction.transferEntity.str(), attributes );
        else if( resp && transferType == interactions::TransferControl::E_EntityPush )
            ownershipController_.PerformAcquisition( interaction.transferEntity.str(), attributes );
    }
}

// -----------------------------------------------------------------------------
// Name: NullTransferSender::Receive
// Created: AHC 2012-03-22
// -----------------------------------------------------------------------------
void NullTransferSender::Receive( interactions::Acknowledge& /*interaction*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NullTransferSender::RequestTransfer
// Created: AHC 2013-07-03
// -----------------------------------------------------------------------------
void NullTransferSender::RequestTransfer( const std::vector< std::string >& , const TransferRequestCallback& callback, TransferType , const std::vector< ::hla::AttributeIdentifier >&  )
{
    callback( true );
    return;
}
