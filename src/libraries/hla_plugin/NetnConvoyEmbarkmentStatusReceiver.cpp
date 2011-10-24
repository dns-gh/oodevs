// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "NetnConvoyEmbarkmentStatusReceiver.h"
#include "TransportationRequester_ABC.h"
#include "Interactions.h"
#include "TransportedUnits_ABC.h"
#include "TransportedUnitsVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnConvoyEmbarkmentStatusReceiver constructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnConvoyEmbarkmentStatusReceiver::NetnConvoyEmbarkmentStatusReceiver( TransportationRequester_ABC& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyEmbarkmentStatusReceiver destructor
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
NetnConvoyEmbarkmentStatusReceiver::~NetnConvoyEmbarkmentStatusReceiver()
{
    // NOTHING
}

namespace
{
    class TransportedUnits : public TransportedUnits_ABC
    {
    public:
        explicit TransportedUnits( const interactions::ListOfTransporters& listOfTransportedUnits )
            : listOfTransportedUnits_( listOfTransportedUnits )
        {}
        virtual void Apply( TransportedUnitsVisitor_ABC& visitor ) const
        {
            BOOST_FOREACH( const NetnObjectDefinitionStruct& unit, listOfTransportedUnits_.list )
                visitor.Notify( unit.callsign.str(), unit.uniqueId.str() );
        }
    private:
        const interactions::ListOfTransporters& listOfTransportedUnits_;
    };
}

// -----------------------------------------------------------------------------
// Name: NetnConvoyEmbarkmentStatusReceiver::Receive
// Created: SLI 2011-10-12
// -----------------------------------------------------------------------------
void NetnConvoyEmbarkmentStatusReceiver::Receive( interactions::NetnConvoyEmbarkmentStatus& interaction )
{
    if( interaction.serviceType != 4 )
        return;
    if( interaction.serviceId.issuingObjectIdentifier.str() != "SWORD" )
        return;
    TransportedUnits units( interaction.listOfObjectEmbarked );
    controller_.NotifyEmbarkationStatus( interaction.serviceId.eventCount, interaction.transportUnitIdentifier.str(), units );
}
