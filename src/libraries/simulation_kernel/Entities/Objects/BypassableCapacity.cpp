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
#include "MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BypassableCapacity )

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::BypassableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BypassableCapacity::BypassableCapacity( xml::xistream& xis )
    : bypassSpeed_ ( xis.attribute< double >( "bypass-speed", 0. ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::BypassableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BypassableCapacity::BypassableCapacity()
    : bypassSpeed_ ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
BypassableCapacity::~BypassableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void BypassableCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & bypassSpeed_;
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void BypassableCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BypassableCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    object.AddCapacity( new BypassableCapacity( *this ) );
    object.GetAttribute< BypassAttribute >();
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::Bypass
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void BypassableCapacity::Bypass( MIL_Object_ABC& object, double rDeltaPercentage )
{
    object.GetAttribute< BypassAttribute >().Update( rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::IsBypassed
// Created: LDC 2010-11-12
// -----------------------------------------------------------------------------
bool BypassableCapacity::IsBypassed( const MIL_Object_ABC& object ) const
{
    return object.GetAttribute< BypassAttribute >().IsBypassed();
}

// -----------------------------------------------------------------------------
// Name: BypassableCapacity::GetBypassSpeed
// Created: LDC 2010-11-12
// -----------------------------------------------------------------------------
double BypassableCapacity::GetBypassSpeed() const
{
    return bypassSpeed_;
}
