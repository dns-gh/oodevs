// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanBlockDeserializer.h"
#include <urban/Block.h>
#include <urban/ColorRGBA.h>
#include <urban/TerrainObject_ABC.h>

using namespace urban;

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
UrbanBlockDeserializer::UrbanBlockDeserializer( const ASN1T_MsgUrbanCreation& asn )
    : message_                  ( asn )
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
  //  if ( message_.attributes.architecture.m.heightPresent )
        return message_.attributes.architecture.height;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureFloorNumber
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
unsigned int UrbanBlockDeserializer::GetArchitectureFloorNumber() const
{
   // if ( message_.attributes.architecture.m.floorNumberPresent )
        return message_.attributes.architecture.floorNumber;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureBasementLevelNumber
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
unsigned int UrbanBlockDeserializer::GetArchitectureBasementLevelNumber() const
{
    return message_.attributes.architecture.basementLevelNumber;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureRoofShape
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetArchitectureRoofShape() const
{
    return message_.attributes.architecture.roofShape;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureMaterial
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetArchitectureMaterial() const
{
    return message_.attributes.architecture.material;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureInnerCluttering
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureInnerCluttering() const
{
    return message_.attributes.architecture.innerCluttering;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureFacadeOpacity
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureFacadeOpacity() const
{
    return message_.attributes.architecture.facadeOpacity;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetSoilOccupation
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetSoilOccupation() const
{
    //if ( message_.attributes.soil.m.occupationPresent )
        return message_.attributes.soil.occupation;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetSoilTrafficability
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetSoilTrafficability() const
{
    //if ( message_.attributes.soil.m.trafficabilityPresent )
        return message_.attributes.soil.trafficability;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetSoilMulitplicity
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
bool UrbanBlockDeserializer::GetSoilMultiplicity() const
{
    //if ( message_.attributes.soil.m.multiplePresent )
        return message_.attributes.soil.multiple;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetSoilCompoundClearing
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetSoilCompoundClearing() const
{
    return message_.attributes.soil.compoundClearing;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetVegetationType
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetVegetationType() const
{
    return message_.attributes.vegetation.type;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetVegetationHeight
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
unsigned int UrbanBlockDeserializer::GetVegetationHeight() const
{
    return message_.attributes.vegetation.height;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetVegetationDensity
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetVegetationDensity() const
{
        return message_.attributes.vegetation.density;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetColor
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::auto_ptr< ColorRGBA > UrbanBlockDeserializer::GetColor() const
{
    std::auto_ptr< ColorRGBA > color ( new ColorRGBA(   message_.attributes.color.red,
                                        message_.attributes.color.green,
                                        message_.attributes.color.blue,
                                        message_.attributes.color.alpha ) );
     return color;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::HasColor
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
bool UrbanBlockDeserializer::HasColor() const
{
    return ( bool ) message_.attributes.m.colorPresent;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::HasArchitecture
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
bool UrbanBlockDeserializer::HasArchitecture() const
{
    if ( message_.attributes.m.architecturePresent )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::HasSoil
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
bool UrbanBlockDeserializer::HasSoil() const
{
    if ( message_.attributes.m.soilPresent )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::HasVegetation
// Created: SLG 2009-12-08
// -----------------------------------------------------------------------------
bool UrbanBlockDeserializer::HasVegetation() const
{
    if ( message_.attributes.m.vegetationPresent )
        return true;
    return false;
}

