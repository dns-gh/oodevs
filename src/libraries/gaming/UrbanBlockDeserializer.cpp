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
#include <urban/TerrainObject_ABC.h>
#include <urban/Block.h>

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
/** @return 
*/
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureHeight() const
{
  //  if ( message_.attributes.architecture.m.heightPresent )
        return message_.attributes.architecture.height;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureFloorNumber
/** @return 
*/
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
unsigned int UrbanBlockDeserializer::GetArchitectureFloorNumber() const
{
   // if ( message_.attributes.architecture.m.floorNumberPresent )
        return message_.attributes.architecture.floorNumber;
}
// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureBasementLevelNumber
/** @return 
*/
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
unsigned int UrbanBlockDeserializer::GetArchitectureBasementLevelNumber() const
{
   // if ( message_.attributes.architecture.m.basementLevelNumberPresent )
        return message_.attributes.architecture.basementLevelNumber;
}
// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureRoofShape
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetArchitectureRoofShape() const
{
    //if ( message_.attributes.architecture.m.roofShapePresent )
        return message_.attributes.architecture.roofShape;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureMaterial
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetArchitectureMaterial() const
{
    //if ( message_.attributes.architecture.m.materialPresent )
        return message_.attributes.architecture.material;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureInnerCluttering
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureInnerCluttering() const
{
    //if ( message_.attributes.architecture.m.innerClutteringPresent )
        return message_.attributes.architecture.innerCluttering;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetArchitectureFacadeOpacity
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetArchitectureFacadeOpacity() const
{
    //if ( message_.attributes.architecture.m.facadeOpacityPresent )
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
    //if ( asn().attributes.soil.occupationPresent )
        return message_.attributes.soil.compoundClearing;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetVegetationType
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
std::string UrbanBlockDeserializer::GetVegetationType() const
{
   // if ( message_.attributes.vegetation.m.typePresent )
        return message_.attributes.vegetation.type;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetVegetationHeight
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
unsigned int UrbanBlockDeserializer::GetVegetationHeight() const
{
    //if ( message_.attributes.vegetation.m.heightPresent )
        return message_.attributes.vegetation.height;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockDeserializer::GetVegetationDensity
// Created: SLG 2009-12-07
// -----------------------------------------------------------------------------
float UrbanBlockDeserializer::GetVegetationDensity() const
{
    //if ( message_.attributes.vegetation.m.densityPresent )
        return message_.attributes.vegetation.density;
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

