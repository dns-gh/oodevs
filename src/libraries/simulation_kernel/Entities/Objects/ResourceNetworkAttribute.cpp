// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ResourceNetworkAttribute.h"
#include "MIL_Object_ABC.h"
#include "ResourceNetworkCapacity.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ResourceNetworkAttribute )

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute()
    : capacity_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( MIL_Object_ABC& object )
{
    capacity_ = object.Retrieve< ResourceNetworkCapacity >();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( xml::xistream& xis, MIL_Object_ABC& object )
{
    capacity_ = object.Retrieve< ResourceNetworkCapacity >();
    if( capacity_ )
        capacity_->Update( xis, object );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute destructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::~ResourceNetworkAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::operator=
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute& ResourceNetworkAttribute::operator=( const ResourceNetworkAttribute& from )
{
    capacity_ = from.capacity_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SendFullState
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    if( capacity_ )
        capacity_->SendFullState( asn );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SendUpdate
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( capacity_ )
        capacity_->SendState( asn );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::serialize
// Created: JSR 2011-06-15
// -----------------------------------------------------------------------------
template< typename Archive >
void ResourceNetworkAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & capacity_;
}
