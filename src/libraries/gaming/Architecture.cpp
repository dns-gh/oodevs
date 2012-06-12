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
Architecture::Architecture( const sword::UrbanAttributes& message, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes )
    : kernel::Architecture( dictionary )
{
    if( message.has_architecture() )
    {
        const sword::UrbanAttributes::Architecture& architecture = message.architecture();
        Initialize( objectTypes, static_cast< unsigned int >( architecture.height() ), architecture.floor_number(), architecture.parking_floors(), architecture.occupation(),
            architecture.trafficability(), architecture.material(), architecture.roof_shape() );
    }
    else
        Initialize( objectTypes, 20, 6, 0, 0.5f, 0.5f );
    CreateDictionnary( true );
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
// Name: Architecture::SetMaterial
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetMaterial( kernel::MaterialCompositionType& /*material*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetRoofShape
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetRoofShape( kernel::RoofShapeType& /*roofShape*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetFloorNumber
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetFloorNumber( unsigned int /*floorNumber*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetParkingFloors
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetParkingFloors( unsigned int /*parkingFloors*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetOccupation
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetOccupation( unsigned int /*occupation*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetHeight
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetHeight( unsigned int /*height*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}

// -----------------------------------------------------------------------------
// Name: Architecture::SetTrafficability
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Architecture::SetTrafficability( float /*trafficability*/ )
{
    throw std::runtime_error( __FUNCTION__ ": not to be called." );
}
