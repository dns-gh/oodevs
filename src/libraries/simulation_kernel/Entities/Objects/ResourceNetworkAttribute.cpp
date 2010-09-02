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
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( xml::xistream& xis, MIL_Object_ABC& object )
{
    capacity_ = object.Retrieve< ResourceNetworkCapacity >();
    if( capacity_  )
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
void ResourceNetworkAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    SendUpdate( asn );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SendUpdate
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
{
    if( capacity_ )
        capacity_->SendState( asn );
}
