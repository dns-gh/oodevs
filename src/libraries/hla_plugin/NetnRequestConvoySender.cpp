// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnRequestConvoySender.h"
#include "TransportationController_ABC.h"
#include "InteractionSender_ABC.h"
#include "TransportedUnitsVisitor_ABC.h"
#include "TransportedUnits_ABC.h"
#include "ContextFactory_ABC.h"
#include "Interactions.h"
#include "Transportation.h"
#include "rpr/Coordinates.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoySender constructor
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnRequestConvoySender::NetnRequestConvoySender( TransportationController_ABC& controller,
                                                  InteractionSender_ABC< interactions::NetnRequestConvoy >& interactionSender,
                                                  const ContextFactory_ABC& contextFactory )
    : controller_       ( controller )
    , interactionSender_( interactionSender )
    , contextFactory_   ( contextFactory )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoySender destructor
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnRequestConvoySender::~NetnRequestConvoySender()
{
    controller_.Unregister( *this );
}

namespace
{
    class TransportedUnitsVisitor : public TransportedUnitsVisitor_ABC
    {
    public:
        explicit TransportedUnitsVisitor( std::vector< NetnObjectDefinitionStruct >& units )
            : units_( units )
        {}
        virtual void Notify( const std::string& callsign, const std::string& uniqueId )
        {
            units_.push_back( NetnObjectDefinitionStruct( callsign, uniqueId, NetnObjectFeatureStruct() ) );
        }
    private:
        std::vector< NetnObjectDefinitionStruct >& units_;
    };
}

// -----------------------------------------------------------------------------
// Name: NetnRequestConvoySender::ConvoyRequested
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
void NetnRequestConvoySender::ConvoyRequested( const std::string& carrier, long long embarkmentTime, const geometry::Point2d& embarkmentPoint,
                                               long long disembarkmentTime, const geometry::Point2d& disembarkmentPoint,
                                               const TransportedUnits_ABC& transportedUnits )
{
    interactions::NetnRequestConvoy convoy;
    convoy.serviceId = NetnEventIdentifier( contextFactory_.Create(), "SWORD" );
    convoy.consumer = UnicodeString( "SWORD" );
    convoy.provider = UnicodeString( carrier );
    convoy.serviceType = 4; // convoy
    convoy.requestTimeOut = 0; // no timeout
    NetnDataTStruct transport;
    transport.appointment = NetnAppointmentStruct( embarkmentTime, rpr::WorldLocation( embarkmentPoint.X(), embarkmentPoint.Y(), 0. ) );
    transport.finalAppointment = NetnAppointmentStruct( disembarkmentTime, rpr::WorldLocation( disembarkmentPoint.X(), disembarkmentPoint.Y(), 0. ) );
    TransportedUnitsVisitor visitor( transport.objectToManage );
    transportedUnits.Apply( visitor );
    convoy.transportData = NetnTransportStruct( transport );
    interactionSender_.Send( convoy );
}
