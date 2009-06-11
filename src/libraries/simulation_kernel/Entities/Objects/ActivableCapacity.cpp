// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ActivableCapacity.h"
#include "Object.h"
#include "ObstacleAttribute.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( ActivableCapacity, "ActivableCapacity" )

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::ActivableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity( xml::xistream& xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::ActivableCapacity
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::ActivableCapacity( const ActivableCapacity& from )
{
    // NOTHING
}
	
// -----------------------------------------------------------------------------
// Name: ActivableCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
ActivableCapacity::~ActivableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void ActivableCapacity::serialize
// Created: JCR 2008-07-04
// -----------------------------------------------------------------------------
template< typename Archive > void ActivableCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void ActivableCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ActivableCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new ActivableCapacity( *this ) );
    object.GetAttribute< ObstacleAttribute >() = ObstacleAttribute( true ); // reserved obstacle
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::Activate
// Created: JCR 2008-07-04
// -----------------------------------------------------------------------------
void ActivableCapacity::Activate( Object& object )
{
    return object.GetAttribute< ObstacleAttribute >().Activate();
}

// -----------------------------------------------------------------------------
// Name: ActivableCapacity::IsActivate
// Created: JCR 2008-07-04
// -----------------------------------------------------------------------------
bool ActivableCapacity::IsActivated( Object& object ) const
{
    return object.GetAttribute< ObstacleAttribute >().IsActivated();
}
