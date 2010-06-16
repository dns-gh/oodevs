// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "protocol/Simulation.h"
#include "UrbanBlockDeserializer.h"
#include <urban/ColorRGBA.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/ColorRGBA.h>

using namespace urban;

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
UrbanBlockDeserializer::UrbanBlockDeserializer( const MsgsSimToClient::MsgUrbanCreation& message )
    : message_                  ( message )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
UrbanBlockDeserializer::~UrbanBlockDeserializer()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureHeight
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureHeight() const
{
    return message_.attributes().architecture().height();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureFloorNumber
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
unsigned int UrbanBlockDeserializer::GetArchitectureFloorNumber() const
{
    return message_.attributes().architecture().floor_number();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureRoofShape
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetArchitectureRoofShape() const
{
    return message_.attributes().architecture().roof_shape();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureMaterial
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetArchitectureMaterial() const
{
    return message_.attributes().architecture().material();
}


// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureOccupation
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureOccupation() const
{
    return message_.attributes().architecture().occupation();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureTrafficability
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureTrafficability() const
{
    return message_.attributes().architecture().trafficability();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetColor
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::auto_ptr< ColorRGBA > UrbanBlockDeserializer::GetColor() const
{
    std::auto_ptr< ColorRGBA > color ( new ColorRGBA( message_.attributes().color().red(), 
                                message_.attributes().color().green(), 
                                message_.attributes().color().blue(), 
                                message_.attributes().color().alpha() ) );
    return color;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::HasColor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
bool UrbanBlockDeserializer::HasColor() const
{
    return message_.attributes().has_color();
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::HasArchitecture
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
bool UrbanBlockDeserializer::HasArchitecture() const
{
    return message_.attributes().has_architecture();
}