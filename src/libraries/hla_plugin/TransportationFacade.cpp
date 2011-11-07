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
#include "InteractionBuilder.h"
#include "TransportationRequester.h"
#include "TransportationOfferer.h"
#include "InteractionSender.h"
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
                                            const InteractionBuilder& builder, const ContextFactory_ABC& contextFactory, const Transporters_ABC& /*transporters*/,
                                            dispatcher::SimulationPublisher_ABC& simulationPublisher, dispatcher::ClientPublisher_ABC& clientsPublisher )
    : pNetnRequestConvoy_            ( new InteractionSender< interactions::NetnRequestConvoy >( *this, builder ) )
    , pNetnOfferConvoy_              ( new InteractionSender< interactions::NetnOfferConvoy >( *this, builder ) )
    , pNetnAcceptOffer_              ( new InteractionSender< interactions::NetnAcceptOffer >( *this, builder ) )
    , pNetnRejectOfferConvoy_        ( new InteractionSender< interactions::NetnRejectOfferConvoy >( *this, builder ) )
    , pNetnCancelConvoy_             ( new InteractionSender< interactions::NetnCancelConvoy >( *this, builder ) )
    , pNetnReadyToReceiveService_    ( new InteractionSender< interactions::NetnReadyToReceiveService >( *this, builder ) )
    , pNetnServiceStarted_           ( new InteractionSender< interactions::NetnServiceStarted >( *this, builder ) )
    , pNetnConvoyEmbarkmentStatus_   ( new InteractionSender< interactions::NetnConvoyEmbarkmentStatus >( *this, builder ) )
    , pNetnConvoyDisembarkmentStatus_( new InteractionSender< interactions::NetnConvoyDisembarkmentStatus >( *this, builder ) )
    , pNetnConvoyDestroyedEntities_  ( new InteractionSender< interactions::NetnConvoyDestroyedEntities >( *this, builder ) )
    , pNetnServiceComplete_          ( new InteractionSender< interactions::NetnServiceComplete >( *this, builder ) )
    , pNetnServiceReceived_          ( new InteractionSender< interactions::NetnServiceReceived >( *this, builder ) )
    , pTransportationRequester_      ( new TransportationRequester( xis, missionResolver, controller, callsignResolver, subordinates,
                                                                    contextFactory, simulationPublisher, *pNetnRequestConvoy_,
                                                                    *pNetnAcceptOffer_, *pNetnRejectOfferConvoy_,
                                                                    *pNetnReadyToReceiveService_, *pNetnServiceReceived_ ) )
    , pTransportationOfferer_        ( new TransportationOfferer( xis, missionResolver, *pNetnOfferConvoy_, *pNetnServiceStarted_, *pNetnConvoyEmbarkmentStatus_, *pNetnConvoyDisembarkmentStatus_,
                                                                  controller, contextFactory, callsignResolver, clientsPublisher ) )
{
    // NOTHING
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
    pTransportationRequester_->Receive( interaction );
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
    pTransportationRequester_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    pTransportationRequester_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnConvoyDisembarkmentStatus& interaction )
{
    pTransportationRequester_->Receive( interaction );
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
    pTransportationRequester_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: TransportationFacade::Receive
// Created: SLI 2011-10-24
// -----------------------------------------------------------------------------
void TransportationFacade::Receive( interactions::NetnServiceReceived& /*interaction*/ )
{
    // NOTHING
}
