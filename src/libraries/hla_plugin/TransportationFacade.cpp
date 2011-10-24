// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TransportationFacade.h"
#include "TransportationInteractionBuilder.h"
#include "TransportationRequester.h"
#include "NetnRequestConvoySender.h"
#include "TransportationOfferer.h"
#include "NetnOfferConvoyReceiver.h"
#include "NetnOfferResponseSender.h"
#include "NetnServiceStartedReceiver.h"
#include "NetnServiceCompleteReceiver.h"
#include "NetnConvoyEmbarkmentStatusReceiver.h"
#include "NetnConvoyDisembarkmentStatusReceiver.h"
#include <xeumeuleu/xml.hpp>
#include <hla/Interaction.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: TransportationFacade constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
TransportationFacade::TransportationFacade( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                            tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                            const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                            Federate_ABC& federate, const ContextFactory_ABC& contextFactory, const Transporters_ABC& transporters,
                                            dispatcher::SimulationPublisher_ABC& simulationPublisher, dispatcher::ClientPublisher_ABC& clientsPublisher )
    : pNetnRequestConvoy_                    ( new ::hla::Interaction< interactions::NetnRequestConvoy >( *this ) )
    , pNetnOfferConvoy_                      ( new ::hla::Interaction< interactions::NetnOfferConvoy >( *this ) )
    , pNetnAcceptOffer_                      ( new ::hla::Interaction< interactions::NetnAcceptOffer >( *this ) )
    , pNetnRejectOfferConvoy_                ( new ::hla::Interaction< interactions::NetnRejectOfferConvoy >( *this ) )
    , pNetnCancelConvoy_                     ( new ::hla::Interaction< interactions::NetnCancelConvoy >( *this ) )
    , pNetnReadyToReceiveService_            ( new ::hla::Interaction< interactions::NetnReadyToReceiveService >( *this ) )
    , pNetnServiceStarted_                   ( new ::hla::Interaction< interactions::NetnServiceStarted >( *this ) )
    , pNetnConvoyEmbarkmentStatus_           ( new ::hla::Interaction< interactions::NetnConvoyEmbarkmentStatus >( *this ) )
    , pNetnConvoyDisembarkmentStatus_        ( new ::hla::Interaction< interactions::NetnConvoyDisembarkmentStatus >( *this ) )
    , pNetnConvoyDestroyedEntities_          ( new ::hla::Interaction< interactions::NetnConvoyDestroyedEntities >( *this ) )
    , pNetnServiceComplete_                  ( new ::hla::Interaction< interactions::NetnServiceComplete >( *this ) )
    , pNetnServiceReceived_                  ( new ::hla::Interaction< interactions::NetnServiceReceived >( *this ) )
    , pTransportationRequester_             ( new TransportationRequester( xis, missionResolver, controller, callsignResolver, subordinates, contextFactory, simulationPublisher ) )
    , pNetnRequestConvoySender_              ( new NetnRequestConvoySender( *pTransportationRequester_, *this ) )
    , pNetnOfferConvoyReceiver_              ( new NetnOfferConvoyReceiver( *pTransportationRequester_ ) )
    , pNetnServiceStartedReceiver_           ( new NetnServiceStartedReceiver( *pTransportationRequester_ ) )
    , pNetnConvoyEmbarkmentStatusReceiver_   ( new NetnConvoyEmbarkmentStatusReceiver( *pTransportationRequester_ ) )
    , pNetnConvoyDisembarkmentStatusReceiver_( new NetnConvoyDisembarkmentStatusReceiver( *pTransportationRequester_ ) )
    , pTransportationOfferer_                ( new TransportationOfferer( *this, *this, *this, transporters, controller, callsignResolver ) )
    , pNetnOfferResponseSender_              ( new NetnOfferResponseSender( *pTransportationRequester_, *this, *this, *this, *this ) )
    , pNetnServiceCompleteReceiver_          ( new NetnServiceCompleteReceiver( *pTransportationRequester_ ) )
{
    TransportationInteractionBuilder builder;
    builder.Build( federate, *pNetnRequestConvoy_ );
    builder.Build( federate, *pNetnOfferConvoy_ );
    builder.Build( federate, *pNetnAcceptOffer_ );
    builder.Build( federate, *pNetnRejectOfferConvoy_ );
    builder.Build( federate, *pNetnCancelConvoy_ );
    builder.Build( federate, *pNetnReadyToReceiveService_ );
    builder.Build( federate, *pNetnServiceStarted_ );
    builder.Build( federate, *pNetnConvoyEmbarkmentStatus_ );
    builder.Build( federate, *pNetnConvoyDisembarkmentStatus_ );
    builder.Build( federate, *pNetnConvoyDestroyedEntities_ );
    builder.Build( federate, *pNetnServiceComplete_ );
    builder.Build( federate, *pNetnServiceReceived_ );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
TransportationFacade::~TransportationFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnRequestConvoy& request )
{
    pTransportationOfferer_->Receive( request );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnOfferConvoy& interaction )
{
    pNetnOfferConvoyReceiver_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnAcceptOffer& interaction )
{
    pTransportationOfferer_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnRejectOfferConvoy& /*interaction*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnCancelConvoy& /*interaction*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnReadyToReceiveService& interaction )
{
    pTransportationOfferer_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnServiceStarted& interaction )
{
    pNetnServiceStartedReceiver_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    pNetnConvoyEmbarkmentStatusReceiver_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnConvoyDisembarkmentStatus& interaction )
{
    pNetnConvoyDisembarkmentStatusReceiver_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnConvoyDestroyedEntities& /*interaction*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnServiceComplete& interaction )
{
    pNetnServiceCompleteReceiver_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnServiceReceived& /*interaction*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnRequestConvoy& interaction )
{
    pNetnRequestConvoy_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnOfferConvoy& interaction )
{
    pNetnOfferConvoy_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnAcceptOffer& interaction )
{
    pNetnAcceptOffer_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnRejectOfferConvoy& interaction )
{
    pNetnRejectOfferConvoy_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnCancelConvoy& interaction )
{
    pNetnCancelConvoy_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnReadyToReceiveService& interaction )
{
    pNetnReadyToReceiveService_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnServiceStarted& interaction )
{
    pNetnServiceStarted_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    pNetnConvoyEmbarkmentStatus_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnConvoyDisembarkmentStatus& interaction )
{
    pNetnConvoyDisembarkmentStatus_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnConvoyDestroyedEntities& interaction )
{
    pNetnConvoyDestroyedEntities_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnServiceComplete& interaction )
{
    pNetnServiceComplete_->Send( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Send
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Send( const interactions::NetnServiceReceived& interaction )
{
    pNetnServiceReceived_->Send( interaction );
}
