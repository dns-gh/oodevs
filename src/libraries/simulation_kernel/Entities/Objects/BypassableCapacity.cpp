// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BypassableCapacity.h"
#include "BypassAttribute.h"
#include "Object.h"

#include "HLA/Deserializer.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( BypassableCapacity, "BypassableCapacity" )

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::BypassableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BypassableCapacity::BypassableCapacity( xml::xistream& xis )
    : bypassSpeed_ ( xml::attribute( xis, "bypass-speed", 0 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::BypassableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BypassableCapacity::BypassableCapacity()
    : bypassSpeed_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BypassableCapacity::BypassableCapacity( const BypassableCapacity& from )
{
    // NOTHING
}
	
// -----------------------------------------------------------------------------
// Name: BypassableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BypassableCapacity::~BypassableCapacity()
{

}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void BypassableCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & bypassSpeed_;    
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BypassableCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BypassableCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new BypassableCapacity( *this ) );
    object.GetAttribute< BypassAttribute >();
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::Bypass
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void BypassableCapacity::Bypass( Object& object, MT_Float rDeltaPercentage )
{
    object.GetAttribute< BypassAttribute >().Update( rDeltaPercentage );
}
