// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FireForbiddenCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( FireForbiddenCapacity )

// -----------------------------------------------------------------------------
// Name: FireForbiddenCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
FireForbiddenCapacity::FireForbiddenCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireForbiddenCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
FireForbiddenCapacity::FireForbiddenCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireForbiddenCapacity destructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
FireForbiddenCapacity::~FireForbiddenCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireForbiddenCapacity constructor
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
FireForbiddenCapacity::FireForbiddenCapacity( const FireForbiddenCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireForbiddenCapacity::serialize
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
template< typename Archive >
void FireForbiddenCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: FireForbiddenCapacity::Instanciate
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
void FireForbiddenCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    FireForbiddenCapacity* capacity = new FireForbiddenCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: FireForbiddenCapacity::Register
// Created: JSR 2011-08-31
// -----------------------------------------------------------------------------
void FireForbiddenCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
