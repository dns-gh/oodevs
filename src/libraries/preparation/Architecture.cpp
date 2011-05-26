// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Architecture.h"
#include <urban/TerrainObject_ABC.h>
#include <urban/PhysicalAttribute.h>

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::Architecture( const urban::TerrainObject_ABC& object, std::auto_ptr< kernel::Architecture_ABC > pArchitecture )
    : pArchitecture_( pArchitecture )
{
    const urban::PhysicalAttribute* pPhysical = object.Retrieve< urban::PhysicalAttribute >();;
    if( pPhysical && pPhysical->GetArchitecture() )
    {
        const urban::Architecture& architecture = *pPhysical->GetArchitecture();
        Initialize( architecture.GetHeight(), architecture.GetFloorNumber(), architecture.GetParkingFloors(), architecture.GetRoofShape(),
                    architecture.GetMaterial(), architecture.GetOccupation(), architecture.GetTrafficability() );
    }
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Architecture::Initialize
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Architecture::Initialize( float height, unsigned int floorNumber, unsigned int parkingFloors, const std::string& roofShape,
                               const std::string& material, float occupation, float trafficability )
{
    pArchitecture_->Initialize( height, floorNumber, parkingFloors, roofShape, material, occupation, trafficability );
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetMaterial
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
const std::string& Architecture::GetMaterial() const
{
    return pArchitecture_->GetMaterial();
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetFloorNumber
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetFloorNumber() const
{
    return pArchitecture_->GetFloorNumber();
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetOccupation
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetOccupation() const
{
    return pArchitecture_->GetOccupation();
}
