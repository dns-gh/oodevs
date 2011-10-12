// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnOfferConvoySender.h"
#include "InteractionSender_ABC.h"
#include "Transporters_ABC.h"
#include "Interactions.h"
#include "TransportedUnitsVisitor_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoySender constructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnOfferConvoySender::NetnOfferConvoySender( InteractionSender_ABC< interactions::NetnOfferConvoy >& offerInteractionSender,
                                              InteractionSender_ABC< interactions::NetnServiceStarted >& serviceStartedInteractionSender,
                                              const Transporters_ABC& transporters )
    : offerInteractionSender_         ( offerInteractionSender )
    , serviceStartedInteractionSender_( serviceStartedInteractionSender )
    , transporters_                   ( transporters )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoySender destructor
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
NetnOfferConvoySender::~NetnOfferConvoySender()
{
    // NOTHING
}

namespace
{
    class TransportersVisitor : public TransportedUnitsVisitor_ABC
    {
    public:
        explicit TransportersVisitor( interactions::ListOfTransporters& listOfTransporters )
            : listOfTransporters_( listOfTransporters )
        {}
        virtual void Notify( const std::string& callsign, const std::string& uniqueId )
        {
            listOfTransporters_.listOfTransporters.push_back( NetnObjectDefinitionStruct( callsign, uniqueId, NetnObjectFeatureStruct() ) );
        }
    private:
        interactions::ListOfTransporters& listOfTransporters_;
    };
    void FillTransporters( const Transporters_ABC& transporters, interactions::ListOfTransporters& listOfTransporters, const NetnDataTStruct& dataTransport )
    {
        TransportersVisitor visitor( listOfTransporters );
        const std::string transportedUnitUniqueId = dataTransport.objectToManage[ 0 ].uniqueId.str();
        const geometry::Point2d embarkmentPoint( dataTransport.appointment.location.Latitude(), dataTransport.appointment.location.Longitude() );
        transporters.Apply( transportedUnitUniqueId, embarkmentPoint, visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoySender::Receive
// Created: SLI 2011-10-11
// -----------------------------------------------------------------------------
void NetnOfferConvoySender::Receive( interactions::NetnRequestConvoy& request )
{
    if( request.serviceType != 4 ) // Convoy service type
        return;
    if( request.transportData.convoyType != 0 ) // Transport convoy type
        return;
    if( request.transportData.dataTransport.objectToManage.empty() )
        return;
    interactions::NetnOfferConvoy offer;
    FillTransporters( transporters_, offer.listOfTransporters, request.transportData.dataTransport );
    if( offer.listOfTransporters.listOfTransporters.size() == 0u )
        return;
    offer.isOffering = 1; // True
    offer.requestTimeOut = 0u; // No timeout
    offer.serviceId.eventCount = request.serviceId.eventCount;
    offer.serviceId.issuingObjectIdentifier = request.serviceId.issuingObjectIdentifier;
    offer.consumer = request.consumer;
    offer.provider = request.provider;
    offer.serviceType = request.serviceType;
    offer.offerType = 1; // RequestAcknowledgePositive
    offer.transportData = request.transportData;
    pendingOffers_.insert( offer.serviceId.eventCount );
    offerInteractionSender_.Send( offer );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoySender::Receive
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferConvoySender::Receive( interactions::NetnAcceptOffer& accept )
{
    unsigned int eventCount = accept.serviceId.eventCount;
    if( pendingOffers_.find( eventCount ) == pendingOffers_.end() )
        return;
    if( accept.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    Transfer( pendingOffers_, acceptedOffers_, eventCount );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoySender::Receive
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferConvoySender::Receive( interactions::NetnReadyToReceiveService& readyToReceive )
{
    unsigned int eventCount = readyToReceive.serviceId.eventCount;
    if( acceptedOffers_.find( eventCount ) == acceptedOffers_.end() )
        return;
    if( readyToReceive.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    Transfer( acceptedOffers_, startedOffers_, eventCount );
    interactions::NetnServiceStarted serviceStarted;
    serviceStarted.serviceId = readyToReceive.serviceId;
    serviceStarted.consumer = readyToReceive.consumer;
    serviceStarted.provider = readyToReceive.provider;
    serviceStarted.serviceType = readyToReceive.serviceType;
    serviceStartedInteractionSender_.Send( serviceStarted );
}

// -----------------------------------------------------------------------------
// Name: NetnOfferConvoySender::Transfer
// Created: VPR 2011-10-12
// -----------------------------------------------------------------------------
void NetnOfferConvoySender::Transfer( T_Offers& from, T_Offers& to, unsigned int eventCount ) const
{
    from.erase( eventCount );
    to.insert( eventCount );
}
