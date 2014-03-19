// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PropagationCapacity.h"
#include "MIL_Object_ABC.h"

using namespace sword::capacity;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::capacity::PropagationCapacity )

// -----------------------------------------------------------------------------
// Name: PropagationCapacity constructor
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
PropagationCapacity::PropagationCapacity()
{
    // NOTHING
}

PropagationCapacity::PropagationCapacity( const PropagationCapacity& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity constructor
// Created: LGY 2012-09-19
// -----------------------------------------------------------------------------
PropagationCapacity::PropagationCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity destructor
// Created: LGY 2012-09-19
// -----------------------------------------------------------------------------
PropagationCapacity::~PropagationCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity::Register
// Created: LGY 2012-09-20
// -----------------------------------------------------------------------------
void PropagationCapacity::Register( MIL_Object_ABC& /*object*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity::Instanciate
// Created: LGY 2012-09-20
// -----------------------------------------------------------------------------
void PropagationCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new PropagationCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: PropagationCapacity::serialize
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
template< typename Archive >
void PropagationCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}
