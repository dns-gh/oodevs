// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BorderCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BorderCapacity )

// -----------------------------------------------------------------------------
// Name: BorderCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::BorderCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::BorderCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity destructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::~BorderCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
BorderCapacity::BorderCapacity( const BorderCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::serialize
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
template< typename Archive >
void BorderCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::Instanciate
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
void BorderCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    BorderCapacity* capacity = new BorderCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: BorderCapacity::Register
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
void BorderCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
