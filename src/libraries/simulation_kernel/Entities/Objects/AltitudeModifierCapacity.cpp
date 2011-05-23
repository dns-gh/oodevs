// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AltitudeModifierCapacity.h"
#include "AltitudeModifierAttribute.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( AltitudeModifierCapacity )

// -----------------------------------------------------------------------------
// Name: AltitudeModifierCapacity constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierCapacity::AltitudeModifierCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierCapacity constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierCapacity::AltitudeModifierCapacity( const AltitudeModifierCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierCapacity constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierCapacity::AltitudeModifierCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierCapacity destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierCapacity::~AltitudeModifierCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierCapacity::serialize
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
template< typename Archive >
void AltitudeModifierCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierCapacity::Instanciate
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    AltitudeModifierCapacity* capacity = new AltitudeModifierCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierCapacity::Register
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
