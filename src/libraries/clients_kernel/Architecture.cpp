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
#include "UrbanObject_ABC.h"
#include "MaterialCompositionType.h"
#include "RoofShapeType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::Architecture( kernel::UrbanObject_ABC& object, PropertiesDictionary& dictionary, const ObjectTypes& objectTypes )
    : object_        ( object )
    , dictionary_    ( dictionary )
    , objectTypes_   ( objectTypes )
    , height_        ( 0, Units::meters )
    , floorNumber_   ( 0 )
    , parkingFloors_ ( 0 )
    , occupation_    ( 0, Units::percentage )
    , trafficability_( 0, Units::tons )
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
void Architecture::Initialize( unsigned int height, unsigned int floorNumber, unsigned int parkingFloors, float occupation,
                               float trafficability, const std::string& material /* = "" */, const std::string& roofShape /* = "" */ )
{
    height_.value_ = height;
    floorNumber_ = floorNumber;
    parkingFloors_ = parkingFloors;
    occupation_.value_ = static_cast< unsigned int >( occupation * 100u );
    trafficability_.value_ = trafficability;

    material_ = objectTypes_.StringResolver< MaterialCompositionType >::Find( material );
    if( !material_ )
    {
        tools::Iterator< const MaterialCompositionType& > it = objectTypes_.StringResolver< MaterialCompositionType >::CreateIterator();
        assert( it.HasMoreElements() );
        material_ = const_cast< MaterialCompositionType* >( &it.NextElement() );
    }

    roofShape_ = objectTypes_.StringResolver< RoofShapeType >::Find( roofShape );
    if( !roofShape_ )
    {
        tools::Iterator< const RoofShapeType& > it = objectTypes_.StringResolver< RoofShapeType >::CreateIterator();
        assert( it.HasMoreElements() );
        roofShape_ = const_cast< RoofShapeType* >( &it.NextElement() );
    }
}

template< typename T >
struct Setter
{
    Setter( UrbanObject_ABC& object, const ObjectTypes& objectTypes ) : object_( object ), objectTypes_( objectTypes ) {}
    void operator()( T* data, const T& value )
    {
        *data = value;
        object_.UpdateTemplate( objectTypes_ );
    }
    Setter& operator=( const Setter& rhs ) { object_ = rhs.object_; objectTypes_ = rhs.objectTypes_; return *this; }

    UrbanObject_ABC& object_;
    const ObjectTypes& objectTypes_;
};

// -----------------------------------------------------------------------------
// Name: Architecture::CreateDictionnary
// Created: ABR 2012-05-30
// -----------------------------------------------------------------------------
void Architecture::CreateDictionnary( bool readOnly )
{
    if( readOnly )
    {
        const Architecture& architecture = *this;
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), architecture.trafficability_ );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), architecture.height_ );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), architecture.floorNumber_ );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/parkingFloors" ), architecture.parkingFloors_ );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), architecture.roofShape_->GetName() );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), architecture.material_->GetName() );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), architecture.occupation_ );
    }
    else
    {
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/trafficability" ), trafficability_, Setter< UnitedValue< float > >( object_, objectTypes_ ) );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/Height" ), height_, Setter< UnitedValue< unsigned int > >( object_, objectTypes_ ) );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/floorNumber" ), floorNumber_, Setter< unsigned int >( object_, objectTypes_ ) );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/parkingFloors" ), parkingFloors_, Setter< unsigned int >( object_, objectTypes_ ) );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/roofShape" ), roofShape_, Setter< RoofShapeType* >( object_, objectTypes_ ) );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/material" ), material_, Setter< MaterialCompositionType* >( object_, objectTypes_ ) );
        dictionary_.Register( object_, tools::translate( "Block", "PhysicalFeatures/Architecture/occupation" ), occupation_, Setter< UnitedValue< unsigned int > >( object_, objectTypes_ ) );
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
    return occupation_.value_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetHeight
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
unsigned int Architecture::GetHeight() const
{
    return height_.value_;
}

// -----------------------------------------------------------------------------
// Name: Architecture::GetTrafficability
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
float Architecture::GetTrafficability() const
{
    return trafficability_.value_;
}
