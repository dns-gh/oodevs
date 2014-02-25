// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SpawnedAttribute.h"

BOOST_CLASS_EXPORT_GUID( SpawnedAttribute, "SpawnedAttribute" )

// -----------------------------------------------------------------------------
// Name: SpawnedAttribute constructor
// Created: LGY 2014-02-24
// -----------------------------------------------------------------------------
SpawnedAttribute::SpawnedAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpawnedAttribute constructor
// Created: LGY 2014-02-24
// -----------------------------------------------------------------------------
SpawnedAttribute::SpawnedAttribute( MIL_Object_ABC& )
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
