// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "WorkableCapacity.h"
#include "Object.h"
#include "AnimatorAttribute.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( WorkableCapacity )

// -----------------------------------------------------------------------------
// Name: WorkableCapacity constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
WorkableCapacity::WorkableCapacity( xml::xistream& xis )    
    : maxAnimators_ ( xml::attribute< int >( xis, "max-animator" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
WorkableCapacity::WorkableCapacity()    
    : maxAnimators_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
WorkableCapacity::~WorkableCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity constructor
// Created: JCR 2008-08-21
// -----------------------------------------------------------------------------
WorkableCapacity::WorkableCapacity( const WorkableCapacity& from )
    : maxAnimators_ ( from.maxAnimators_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void WorkableCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & maxAnimators_;
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void WorkableCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.GetAttribute< AnimatorAttribute >() = AnimatorAttribute( maxAnimators_ );
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void WorkableCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new WorkableCapacity( *this ) );
    object.GetAttribute< AnimatorAttribute >() = AnimatorAttribute( maxAnimators_ );
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity::AddAnimator
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool WorkableCapacity::AddAnimator( Object& object, const MIL_Agent_ABC& agent )
{
    return object.GetAttribute< AnimatorAttribute >().AddAnimator( agent );    
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity::ReleaseAnimator
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void WorkableCapacity::ReleaseAnimator( Object& object, const MIL_Agent_ABC& agent )
{
    return object.GetAttribute< AnimatorAttribute >().ReleaseAnimator( agent );
}
