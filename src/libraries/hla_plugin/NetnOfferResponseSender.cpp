// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnOfferResponseSender.h"
#include "TransportationRequester_ABC.h"
#include "InteractionSender_ABC.h"
#include "Interactions.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnOfferResponseSender constructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnOfferResponseSender::NetnOfferResponseSender( TransportationRequester_ABC& controller,
                                                  InteractionSender_ABC< interactions::NetnAcceptOffer >& acceptOfferSender,
                                                  InteractionSender_ABC< interactions::NetnRejectOfferConvoy >& rejectOfferSender,
                                                  InteractionSender_ABC< interactions::NetnReadyToReceiveService >& readyToReceiveServiceSender,
                                                  InteractionSender_ABC< interactions::NetnServiceReceived >& serviceReceivedSender )
    : controller_                 ( controller )
    , acceptOfferSender_          ( acceptOfferSender )
    , rejectOfferSender_          ( rejectOfferSender )
    , readyToReceiveServiceSender_( readyToReceiveServiceSender )
    , serviceReceivedSender_      ( serviceReceivedSender )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferResponseSender destructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnOfferResponseSender::~NetnOfferResponseSender()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferResponseSender::ConvoyRequested
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferResponseSender::ConvoyRequested( long long /*embarkmentTime*/, const geometry::Point2d& /*embarkmentPoint*/,
                                               long long /*disembarkmentTime*/, const geometry::Point2d& /*disembarkmentPoint*/,
                                               const TransportedUnits_ABC& /*transportedUnits*/, unsigned int /*context*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnOfferResponseSender::OfferAccepted
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferResponseSender::OfferAccepted( unsigned int context, const std::string& provider )
{
    interactions::NetnAcceptOffer acceptation;
    acceptation.serviceId = NetnEventIdentifier( context, "SWORD" );
    acceptation.consumer = UnicodeString( "SWORD" );
    acceptation.provider = UnicodeString( provider );
    acceptation.serviceType = 4; // Convoy
    acceptOfferSender_.Send( acceptation );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferResponseSender::OfferRejected
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferResponseSender::OfferRejected( unsigned int context, const std::string& provider, const std::string& reason )
{
    interactions::NetnRejectOfferConvoy rejection;
    rejection.serviceId = NetnEventIdentifier( context, "SWORD" );
    rejection.consumer = UnicodeString( "SWORD" );
    rejection.provider = UnicodeString( provider );
    rejection.serviceType = 4; // Convoy
    rejection.reason = reason;
    rejectOfferSender_.Send( rejection );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferResponseSender::ReadyToReceiveService
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferResponseSender::ReadyToReceiveService( unsigned int context, const std::string& provider )
{
    interactions::NetnReadyToReceiveService ready;
    ready.serviceId = NetnEventIdentifier( context, "SWORD" );
    ready.consumer = UnicodeString( "SWORD" );
    ready.provider = UnicodeString( provider );
    ready.serviceType = 4; // Convoy
    readyToReceiveServiceSender_.Send( ready );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferResponseSender::ServiceReceived
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferResponseSender::ServiceReceived( unsigned int context, const std::string& provider )
{
    interactions::NetnServiceReceived received;
    received.serviceId = NetnEventIdentifier( context, "SWORD" );
    received.consumer = UnicodeString( "SWORD" );
    received.provider = UnicodeString( provider );
    received.serviceType = 4; // Convoy
    serviceReceivedSender_.Send( received );
}
