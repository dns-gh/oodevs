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
// Name: NetnObjectDescription::NetnObjectDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectDescription::NetnObjectDescription()
    : weight( 0. )
    , volume( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectDescription::NetnObjectDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectDescription::NetnObjectDescription( float weight, float volume, const std::string& type )
    : weight( weight )
    , volume( volume )
    , type  ( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectDescription::~NetnObjectDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectDescription::~NetnObjectDescription()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnHumanDescription::NetnHumanDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnHumanDescription::NetnHumanDescription()
    : quantity( 0 )
    , injury  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnHumanDescription::NetnHumanDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnHumanDescription::NetnHumanDescription( const rpr::EntityType& humanType, short quantity, short injury )
    : humanType( humanType )
    , quantity ( quantity )
    , injury   ( injury )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnHumanDescription::~NetnHumanDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnHumanDescription::~NetnHumanDescription()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnEquipDescription::NetnEquipDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnEquipDescription::NetnEquipDescription()
    : quantity   ( 0 )
    , damageState( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnEquipDescription::NetnEquipDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnEquipDescription::NetnEquipDescription( const rpr::EntityType& equipType, int quantity, int damageState )
    : equipType  ( equipType )
    , quantity   ( quantity )
    , damageState( damageState )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnEquipDescription::~NetnEquipDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnEquipDescription::~NetnEquipDescription()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnPlateformDescription::NetnPlateformDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnPlateformDescription::NetnPlateformDescription()
    : damageState( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnPlateformDescription::NetnPlateformDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnPlateformDescription::NetnPlateformDescription( const rpr::EntityType& plateformType, int damageState )
    : plateformType( plateformType )
    , damageState  ( damageState )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnPlateformDescription::~NetnPlateformDescription
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnPlateformDescription::~NetnPlateformDescription()
{
    // NOTHING
}

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
// Name: NetnObjectFeatureStruct::NetnObjectFeatureStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectFeatureStruct::NetnObjectFeatureStruct( const std::vector< NetnObjectDefinitionStruct >& subObjectList )
    : featureLevel ( 5 )
    , subObjectList( subObjectList.begin(), subObjectList.end() )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::NetnObjectFeatureStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectFeatureStruct::NetnObjectFeatureStruct( const NetnObjectDescription& objectDetail )
    : featureLevel( 1 )
    , objectDetail( objectDetail )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::NetnObjectFeatureStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectFeatureStruct::NetnObjectFeatureStruct( const NetnHumanDescription& humanDetail )
    : featureLevel( 2 )
    , humanDetail ( humanDetail )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::NetnObjectFeatureStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectFeatureStruct::NetnObjectFeatureStruct( const NetnEquipDescription& equipDetail )
    : featureLevel( 3 )
    , equipDetail ( equipDetail )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::NetnObjectFeatureStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnObjectFeatureStruct::NetnObjectFeatureStruct( const NetnPlateformDescription& plateformDetail )
    : featureLevel   ( 4 )
    , plateformDetail( plateformDetail )
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
NetnObjectDefinitionStruct::NetnObjectDefinitionStruct( const std::string& callsign, const std::string& uniqueId, const NetnObjectFeatureStruct& objectFeature )
    : callsign     ( callsign )
    , uniqueId     ( uniqueId )
    , objectFeature( objectFeature )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::~NetnObjectDefinitionStruct
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
// Name: NetnObjectFeatureStruct::NetnAppointmentStruct
// Created: SLI 2011-10-07
// -----------------------------------------------------------------------------
NetnAppointmentStruct::NetnAppointmentStruct( int64 dateTime, const rpr::WorldLocation& location )
    : dateTime( dateTime )
    , location( location )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NetnObjectFeatureStruct::~NetnAppointmentStruct
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
// Name: NetnTransportStruct::NetnTransportStruct
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
