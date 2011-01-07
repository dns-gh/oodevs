// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UniversalCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( UniversalCapacity )

// -----------------------------------------------------------------------------
// Name: UniversalCapacity constructor
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
UniversalCapacity::UniversalCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniversalCapacity constructor
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
UniversalCapacity::UniversalCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniversalCapacity constructor
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
UniversalCapacity::UniversalCapacity( const UniversalCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniversalCapacity destructor
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
UniversalCapacity::~UniversalCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniversalCapacity::serialize
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
template< typename Archive >
void UniversalCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: UniversalCapacity::Instanciate
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
void UniversalCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    UniversalCapacity* capacity = new UniversalCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: UniversalCapacity::Register
// Created: JSR 2011-01-06
// -----------------------------------------------------------------------------
void UniversalCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
