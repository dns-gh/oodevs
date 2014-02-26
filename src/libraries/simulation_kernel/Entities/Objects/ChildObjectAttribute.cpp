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

BOOST_CLASS_EXPORT_IMPLEMENT( ChildObjectAttribute )

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
ChildObjectAttribute::ChildObjectAttribute( MIL_Object_ABC& object )
    : childObject_( &object )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute destructor
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
ChildObjectAttribute::~ChildObjectAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute::serialize
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
template< typename Archive >
void ChildObjectAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & childObject_;
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void ChildObjectAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< ChildObjectAttribute, ChildObjectAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute::AddAnimator
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void ChildObjectAttribute::AddChildObject( MIL_Object_ABC& object )
{
    if( childObject_ )
        throw MASA_EXCEPTION( "Detector already registered." );
    childObject_ = &object;
}

// -----------------------------------------------------------------------------
// Name: ChildObjectAttribute::GetChildObject
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
MIL_Object_ABC* ChildObjectAttribute::GetChildObject() const
{
    return childObject_;
}
