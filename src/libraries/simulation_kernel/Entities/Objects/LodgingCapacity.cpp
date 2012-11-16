// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "LodgingCapacity.h"
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( LodgingCapacity )

// -----------------------------------------------------------------------------
// Name: LodgingCapacity constructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LodgingCapacity::LodgingCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingCapacity constructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LodgingCapacity::LodgingCapacity( const LodgingCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingCapacity constructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LodgingCapacity::LodgingCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingCapacity destructor
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
LodgingCapacity::~LodgingCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodgingCapacity::serialize
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
template< typename Archive >
void LodgingCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: LodgingCapacity::Instanciate
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
void LodgingCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    LodgingCapacity* capacity = new LodgingCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: LodgingCapacity::Register
// Created: MMC 2012-11-16
// -----------------------------------------------------------------------------
void LodgingCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}
