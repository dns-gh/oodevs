// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Transportation.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::NetnObjectFeatureStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectFeatureStruct::NetnObjectFeatureStruct()
    : featureLevel( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::~NetnObjectFeatureStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectFeatureStruct::~NetnObjectFeatureStruct()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectDefinitionStruct::NetnObjectDefinitionStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectDefinitionStruct::NetnObjectDefinitionStruct()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectDefinitionStruct::NetnObjectDefinitionStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectDefinitionStruct::NetnObjectDefinitionStruct( const std::string& callsign, const std::string& uniqueId )
    : callsign( callsign )
    , uniqueId( uniqueId )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transportation::~NetnObjectDefinitionStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectDefinitionStruct::~NetnObjectDefinitionStruct()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: NetnAppointmentStruct::NetnAppointmentStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnAppointmentStruct::NetnAppointmentStruct()
    : dateTime( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transportation::NetnAppointmentStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnAppointmentStruct::NetnAppointmentStruct( int64 dateTime, const rpr::WorldLocation& location )
    : dateTime( dateTime )
    , location( location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transportation::~NetnAppointmentStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnAppointmentStruct::~NetnAppointmentStruct()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnDataTStruct::NetnDataTStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnDataTStruct::NetnDataTStruct()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnDataTStruct::NetnDataTStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnDataTStruct::NetnDataTStruct( const std::vector< NetnObjectDefinitionStruct >& objectToManage,
                                  const NetnAppointmentStruct& appointment,
                                  const NetnAppointmentStruct& finalAppointment )
    : objectToManage  ( objectToManage.begin(), objectToManage.end() )
    , appointment     ( appointment )
    , finalAppointment( finalAppointment )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnDataTStruct::~NetnDataTStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnDataTStruct::~NetnDataTStruct()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnDataEDStruct::NetnDataEDStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnDataEDStruct::NetnDataEDStruct()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnDataEDStruct::NetnDataEDStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnDataEDStruct::NetnDataEDStruct( const std::vector< NetnObjectDefinitionStruct >& objectToManage,
                                    const NetnAppointmentStruct& appointment )
    : objectToManage( objectToManage.begin(), objectToManage.end() )
    , appointment   ( appointment )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnDataEDStruct::~NetnDataEDStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnDataEDStruct::~NetnDataEDStruct()
{
    // NOITHING
}

// -----------------------------------------------------------------------------
// Name: NetnEventIdentifier::NetnEventIdentifier
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnEventIdentifier::NetnEventIdentifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnEventIdentifier::NetnEventIdentifier
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnEventIdentifier::NetnEventIdentifier( int32 eventCount, const std::string& issuingObjectIdentifier )
    : eventCount             ( eventCount )
    , issuingObjectIdentifier( issuingObjectIdentifier )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnEventIdentifier::~NetnEventIdentifier
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnEventIdentifier::~NetnEventIdentifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnTransportStruct::NetnTransportStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnTransportStruct::NetnTransportStruct()
    : convoyType( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnTransportStruct::NetnTransportStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnTransportStruct::NetnTransportStruct( const NetnDataTStruct& dataTransport )
    : convoyType   ( 0 )
    , dataTransport( dataTransport )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transportation::NetnTransportStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnTransportStruct::NetnTransportStruct( const NetnDataEDStruct& data, int32 convoyType )
    : convoyType       ( convoyType )
{
    if( convoyType == 1 )
        dataEmbarkment = data;
    if( convoyType == 2 )
        dataDisembarkment = data;
}

// -----------------------------------------------------------------------------
// Name: NetnTransportStruct::~NetnTransportStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnTransportStruct::~NetnTransportStruct()
{
    // NOTHING
}
