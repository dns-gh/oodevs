// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SpawnedAttribute.h"
#include "Object.h"

BOOST_CLASS_EXPORT_IMPLEMENT( SpawnedAttribute )

// -----------------------------------------------------------------------------
// Name: SpawnedAttribute constructor
// Created: LGY 2014-02-24
// -----------------------------------------------------------------------------
SpawnedAttribute::SpawnedAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnedAttribute destructor
// Created: LGY 2014-02-24
// -----------------------------------------------------------------------------
SpawnedAttribute::~SpawnedAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnedAttribute::serialize
// Created: LGY 2014-02-24
// -----------------------------------------------------------------------------
template< typename Archive >
void SpawnedAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: SpawnedAttribute::Register
// Created: LGY 2014-02-24
// -----------------------------------------------------------------------------
void SpawnedAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< SpawnedAttribute, SpawnedAttribute >( *this );
}
