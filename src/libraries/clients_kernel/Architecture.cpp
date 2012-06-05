// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Architecture.h"
#include "PropertiesDictionary.h"
#include "Tools.h"
#include "ObjectTypes.h"
#include "MaterialCompositionType.h"
#include "RoofShapeType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::Architecture( PropertiesDictionary& dictionary )
    : dictionary_    ( dictionary )
    , height_        ( 0 )
    , floorNumber_   ( 0 )
    , parkingFloors_ ( 0 )
    , occupation_    ( 0 )
    , trafficability_( 0 )
    , material_      ( 0 )
    , roofShape_     ( 0 )
{
    // NOTHING
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
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void Architecture::Initialize( const ObjectTypes& objectTypes, float height, unsigned int floorNumber, unsigned int parkingFloors, float occupation,
                               float trafficability, const std::string& material /* = "" */, const std::string& roofShape /* = "" */ )
{
    height_ = height;
    floorNumber_ = floorNumber;
    parkingFloors_ = parkingFloors;
    occupation_ = static_cast< unsigned int >( occupation * 100u );
    trafficability_ = trafficability;

    if( material.empty() || material == "default" )
    {
        tools::Iterator< const MaterialCompositionType& > it = objectTypes.StringResolver< MaterialCompositionType >::CreateIterator();
        assert( it.HasMoreElements() );
        material_ = const_cast< MaterialCompositionType* >( &it.NextElement() );
    }
    else
        material_ = &objectTypes.StringResolver< MaterialCompositionType >::Get( material );

    if( roofShape.empty() || roofShape == "default" ) 
    {
        tools::Iterator< const RoofShapeType& > it = objectTypes.StringResolver< RoofShapeType >::CreateIterator();
        assert( it.HasMoreElements() );
        roofShape_ = const_cast< RoofShapeType* >( &it.NextElement() );
    }
    else
        roofShape_ = &objectTypes.StringResolver< RoofShapeType >::Get( roofShape );
}

// -----------------------------------------------------------------------------
// Name: Architecture::CreateDictionnary
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void Architecture::CreateDictionnary( bool readOnly )
{
    if( readOnly )
    {
        const Architecture& architecture = *this;
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), architecture.height_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), architecture.floorNumber_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/parkingFloors" ), architecture.parkingFloors_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), architecture.roofShape_->GetName() );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), architecture.material_->GetName() );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), architecture.occupation_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), architecture.trafficability_ );
    }
    else
    {
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), height_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), floorNumber_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/parkingFloors" ), parkingFloors_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), roofShape_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), material_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), occupation_ );
        dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), trafficability_ );
    }
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetMaterial
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
const MaterialCompositionType& Architecture::GetMaterial() const
{
    assert( material_ != 0 );
    return *material_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetRoofShape
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
const RoofShapeType& Architecture::GetRoofShape() const
{
    assert( roofShape_ != 0 );
    return *roofShape_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetFloorNumber
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetFloorNumber() const
{
    return floorNumber_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetParkingFloors
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
unsigned int Architecture::GetParkingFloors() const
{
    return parkingFloors_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetOccupation
// Created: LGY 2011-04-19
// -----------------------------------------------------------------------------
unsigned int Architecture::GetOccupation() const
{
    return occupation_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetHeight
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
unsigned int Architecture::GetHeight() const
{
    return static_cast< unsigned int >( height_ );
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetTrafficability
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
float Architecture::GetTrafficability() const
{
    return trafficability_;
}
