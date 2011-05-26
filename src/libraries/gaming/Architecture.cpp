// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Architecture.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::Architecture( const sword::UrbanAttributes& message, std::auto_ptr< kernel::Architecture_ABC > pArchitecture )
    : pArchitecture_( pArchitecture )
{
    const sword::UrbanAttributes::Architecture& architecture = message.architecture();
    Initialize( architecture.height(), architecture.floor_number(), architecture.parking_floors(), architecture.roof_shape(),
                architecture.material(), architecture.occupation(), architecture.trafficability() );
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-15
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
