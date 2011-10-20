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
#include "TransportationController.h"
#include "NetnRequestConvoy.h"
#include "NetnRequestConvoySender.h"
#include "NetnOfferConvoy.h"
#include "NetnCancelConvoy.h"
#include "NetnOfferConvoySender.h"
#include "NetnOfferConvoyReceiver.h"
#include "NetnAcceptOffer.h"
#include "NetnRejectOfferConvoy.h"
#include "NetnReadyToReceiveService.h"
#include "NetnOfferResponseSender.h"
#include "NetnServiceStarted.h"
#include "NetnServiceComplete.h"
#include "NetnServiceReceived.h"
#include "NetnConvoyDestroyedEntities.h"
#include "NetnServiceStartedReceiver.h"
#include "NetnServiceCompleteReceiver.h"
#include "NetnConvoyEmbarkmentStatusReceiver.h"
#include "NetnConvoyEmbarkmentStatus.h"
#include "NetnConvoyDisembarkmentStatusReceiver.h"
#include "NetnConvoyDisembarkmentStatus.h"
#include <xeumeuleu/xml.hpp>

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
    : pTransportationController_             ( new TransportationController( xis, missionResolver, controller, callsignResolver, subordinates, contextFactory, simulationPublisher ) )
    , pNetnRequestConvoy_                    ( new NetnRequestConvoy( federate, *this ) )
    , pNetnRequestConvoySender_              ( new NetnRequestConvoySender( *pTransportationController_, *pNetnRequestConvoy_ ) )
    , pNetnOfferConvoyReceiver_              ( new NetnOfferConvoyReceiver( *pTransportationController_ ) )
    , pNetnOfferConvoy_                      ( new NetnOfferConvoy( federate, *pNetnOfferConvoyReceiver_ ) )
    , pNetnServiceStartedReceiver_           ( new NetnServiceStartedReceiver( *pTransportationController_ ) )
    , pNetnServiceStarted_                   ( new NetnServiceStarted( federate, *pNetnServiceStartedReceiver_ ) )
    , pNetnConvoyEmbarkmentStatusReceiver_   ( new NetnConvoyEmbarkmentStatusReceiver( *pTransportationController_ ) )
    , pNetnConvoyEmbarkmentStatus_           ( new NetnConvoyEmbarkmentStatus( federate, *pNetnConvoyEmbarkmentStatusReceiver_ ) )
    , pNetnConvoyDisembarkmentStatusReceiver_( new NetnConvoyDisembarkmentStatusReceiver( *pTransportationController_ ) )
    , pNetnConvoyDisembarkmentStatus_        ( new NetnConvoyDisembarkmentStatus( federate, *pNetnConvoyDisembarkmentStatusReceiver_ ) )
    , pNetnOfferConvoySender_                ( new NetnOfferConvoySender( *pNetnOfferConvoy_, *pNetnServiceStarted_, *pNetnConvoyEmbarkmentStatus_, transporters, controller, callsignResolver ) )
    , pNetnRejectOfferConvoy_                ( new NetnRejectOfferConvoy( federate ) )
    , pNetnAcceptOffer_                      ( new NetnAcceptOffer( federate, *pNetnOfferConvoySender_ ) )
    , pNetnReadyToReceiveService_            ( new NetnReadyToReceiveService( federate, *pNetnOfferConvoySender_ ) )
    , pNetnServiceReceived_                  ( new NetnServiceReceived( federate ) )
    , pNetnOfferResponseSender_              ( new NetnOfferResponseSender( *pTransportationController_, *pNetnAcceptOffer_, *pNetnRejectOfferConvoy_, *pNetnReadyToReceiveService_, *pNetnServiceReceived_ ) )
    , pNetnServiceCompleteReceiver_          ( new NetnServiceCompleteReceiver( *pTransportationController_ ) )
    , pNetnServiceComplete_                  ( new NetnServiceComplete( federate, *pNetnServiceCompleteReceiver_ ) )
    , pNetnCancelConvoy_                     ( new NetnCancelConvoy( federate ) )
    , pNetnConvoyDestroyedEntities_          ( new NetnConvoyDestroyedEntities( federate ) )
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
    pNetnOfferConvoySender_->Receive( request );
}
