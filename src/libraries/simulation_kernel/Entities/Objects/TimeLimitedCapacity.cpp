// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TimeLimitedCapacity.h"
#include "Object.h"

#include "TimeLimitedAttribute.h"


BOOST_CLASS_EXPORT_GUID( TimeLimitedCapacity, "TimeLimitedCapacity" )

// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacity constructor
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
TimeLimitedCapacity::TimeLimitedCapacity( xml::xistream& /*xis*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedCapacity::TimeLimitedCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacityconstructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedCapacity::TimeLimitedCapacity( const TimeLimitedCapacity& from )
{
    // NOTHING
}
	
// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacitydestructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
TimeLimitedCapacity::~TimeLimitedCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacity::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
template< typename Archive >
void TimeLimitedCapacity::serialize( Archive& file, const uint )
{    
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void TimeLimitedCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void TimeLimitedCapacity::Instanciate( Object& object ) const
{    
    object.AddCapacity( new TimeLimitedCapacity( *this ) );    
}

// -----------------------------------------------------------------------------
// Name: TimeLimitedCapacity::Update
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void TimeLimitedCapacity::Update( Object& object, uint time )
{    
    if ( object.GetAttribute< TimeLimitedAttribute >().IsTimeOver( time ) )
        object.MarkForDestruction();    
}
