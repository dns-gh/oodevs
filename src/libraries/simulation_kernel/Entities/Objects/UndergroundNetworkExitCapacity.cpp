// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UndergroundNetworkExitCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( UndergroundNetworkExitCapacity )

// -----------------------------------------------------------------------------
// Name: UndergroundNetworkExitCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundNetworkExitCapacity::UndergroundNetworkExitCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundNetworkExitCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundNetworkExitCapacity::UndergroundNetworkExitCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundNetworkExitCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundNetworkExitCapacity::UndergroundNetworkExitCapacity( const UndergroundNetworkExitCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundNetworkExitCapacity destructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundNetworkExitCapacity::~UndergroundNetworkExitCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundNetworkExitCapacity::serialize
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void UndergroundNetworkExitCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: UndergroundNetworkExitCapacity::Instanciate
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
void UndergroundNetworkExitCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    UndergroundNetworkExitCapacity* capacity = new UndergroundNetworkExitCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: UndergroundNetworkExitCapacity::Register
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
void UndergroundNetworkExitCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
