// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UndergroundCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( UndergroundCapacity )

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::UndergroundCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::UndergroundCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::UndergroundCapacity( const UndergroundCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity destructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::~UndergroundCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::serialize
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void UndergroundCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::Instanciate
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
void UndergroundCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    UndergroundCapacity* capacity = new UndergroundCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::Register
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
void UndergroundCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
