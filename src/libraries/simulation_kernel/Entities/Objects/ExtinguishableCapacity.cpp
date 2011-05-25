// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ExtinguishableCapacity.h"
#include "FireAttribute.h"
#include "MIL_Object_ABC.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( ExtinguishableCapacity )

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::ExtinguishableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ExtinguishableCapacity::ExtinguishableCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::ExtinguishableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ExtinguishableCapacity::ExtinguishableCapacity()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ExtinguishableCapacity::ExtinguishableCapacity( const ExtinguishableCapacity& /*from*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ExtinguishableCapacity::~ExtinguishableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void ExtinguishableCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ExtinguishableCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ExtinguishableCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new ExtinguishableCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ExtinguishableCapacity::Extinguish
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void ExtinguishableCapacity::Extinguish( MIL_Object_ABC& /*object*/, int /*extinguisherAgent*/, int /*numberOfFireHoses*/ )
{
    // NOTHING
}
