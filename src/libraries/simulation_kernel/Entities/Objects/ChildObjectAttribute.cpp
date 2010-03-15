// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ChildObjectAttribute.h"
#include "Object.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( ChildObjectAttribute, "ChildObjectAttribute" )

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute constructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
ChildObjectAttribute::ChildObjectAttribute()
    : childObject_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute constructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
ChildObjectAttribute::ChildObjectAttribute( Object& object )
    : childObject_( &object )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectorAttribute destructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
ChildObjectAttribute::~ChildObjectAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void ChildObjectAttribute::serialize
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
template< typename Archive > 
void ChildObjectAttribute::serialize( Archive& /*file*/, const uint )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void ChildObjectAttribute::Register( Object& object ) const
{
    object.SetAttribute< ChildObjectAttribute, ChildObjectAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute::AddAnimator
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void ChildObjectAttribute::AddChildObject( Object& object )
{
    if ( childObject_ )
        throw( std::exception( " Detector already registered" ) );
    childObject_ = &object;
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute::GetChildObject
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
Object* ChildObjectAttribute::GetChildObject() const
{
    return childObject_;
}
