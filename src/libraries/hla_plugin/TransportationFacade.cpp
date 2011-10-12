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
#include "NetnOfferConvoySender.h"
#include "NetnOfferConvoyReceiver.h"
#include "NetnAcceptOffer.h"
#include "NetnRejectOfferConvoy.h"
#include "NetnReadyToReceiveService.h"
#include "NetnOfferResponseSender.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: TransportationFacade constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
TransportationFacade::TransportationFacade( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                            tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                            const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                            Federate_ABC& federate, const ContextFactory_ABC& contextFactory, const Transporters_ABC& transporters )
    : pTransportationController_ ( new TransportationController( xis, missionResolver, controller, callsignResolver, subordinates, contextFactory ) )
    , pNetnRequestConvoy_        ( new NetnRequestConvoy( federate, *this ) )
    , pNetnRequestConvoySender_  ( new NetnRequestConvoySender( *pTransportationController_, *pNetnRequestConvoy_ ) )
    , pNetnOfferConvoyReceiver_  ( new NetnOfferConvoyReceiver( *pTransportationController_ ) )
    , pNetnOfferConvoy_          ( new NetnOfferConvoy( federate, *pNetnOfferConvoyReceiver_ ) )
    , pNetnOfferConvoySender_    ( new NetnOfferConvoySender( *pNetnOfferConvoy_, transporters ) )
    , pNetnAcceptOffer_          ( new NetnAcceptOffer( federate ) )
    , pNetnRejectOfferConvoy_    ( new NetnRejectOfferConvoy( federate ) )
    , pNetnReadyToReceiveService_( new NetnReadyToReceiveService( federate ) )
    , pNetnOfferResponseSender_  ( new NetnOfferResponseSender( *pTransportationController_, *pNetnAcceptOffer_, *pNetnRejectOfferConvoy_, *pNetnReadyToReceiveService_ ) )
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
