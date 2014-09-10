// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TrafficabilityCapacity.h"
#include "TrafficabilityAttribute.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( TrafficabilityCapacity )

// -----------------------------------------------------------------------------
// Name: TrafficabilityCapacity constructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
TrafficabilityCapacity::TrafficabilityCapacity()
    : default_  ( false )
    , maxWeight_( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityCapacity constructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
TrafficabilityCapacity::TrafficabilityCapacity( xml::xistream& xis )
    : default_  ( xis.attribute< bool >( "default" ) )
    , maxWeight_( xis.attribute< float >( "max-weight", 0.f ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityCapacity constructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
TrafficabilityCapacity::TrafficabilityCapacity( const TrafficabilityCapacity& capacity )
    : default_  ( capacity.default_ )
    , maxWeight_( capacity.maxWeight_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityCapacity destructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
TrafficabilityCapacity::~TrafficabilityCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityCapacity::serialize
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
template< typename Archive >
void TrafficabilityCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & default_;
    file & maxWeight_;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityCapacity::Instanciate
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
void TrafficabilityCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    TrafficabilityCapacity* capacity = new TrafficabilityCapacity( *this );
    object.AddCapacity( capacity );
    if( default_ )
        object.GetAttribute< TrafficabilityAttribute >() = TrafficabilityAttribute( maxWeight_ );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityCapacity::Register
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
void TrafficabilityCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
