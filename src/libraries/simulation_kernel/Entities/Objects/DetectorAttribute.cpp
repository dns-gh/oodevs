// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DetectorAttribute.h"
#include "Object.h"

BOOST_CLASS_EXPORT_GUID( DetectorAttribute, "AnimatorAttribute" )

// -----------------------------------------------------------------------------
// Name: DetectorAttribute constructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
DetectorAttribute::DetectorAttribute()
: detector_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute constructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
DetectorAttribute::DetectorAttribute( const MIL_Agent_ABC& agent )
: detector_ ( &agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute destructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
DetectorAttribute::~DetectorAttribute()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DetectorAttribute::serialize
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
template< typename Archive >
void DetectorAttribute::serialize( Archive& /*file*/, const unsigned int )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void DetectorAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< DetectorAttribute, DetectorAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute::AddAnimator
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void DetectorAttribute::AddDetector( const MIL_Agent_ABC& agent )
{
    if( detector_ )
        throw( std::exception( " Detector already registered" ) );
    detector_ = &agent;
}


// -----------------------------------------------------------------------------
// Name: DetectorAttribute::GetAnimators
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
const MIL_Agent_ABC* DetectorAttribute::GetDetector() const
{
    return detector_;
}
