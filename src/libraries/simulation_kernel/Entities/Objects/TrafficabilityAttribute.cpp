// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "TrafficabilityAttribute.h"
#include "Object.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( xml::xistream& xis )
    : max_( xis.attribute< double >( "max" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::operator=
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute& TrafficabilityAttribute::operator=( const TrafficabilityAttribute& rhs )
{
    max_ = rhs.max_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute destructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::~TrafficabilityAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::serialize
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
void TrafficabilityAttribute::serialize( Archive& file, const double )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & max_;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::WriteODB
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "obstacle" )
            << xml::attribute( "max", max_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::Register
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< TrafficabilityAttribute, TrafficabilityAttribute >( *this );
}


// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::SendFullState
// Created: LGY 2011-08-24
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
	asn.mutable_trafficability()->set_value( max_ );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::SendUpdate
// Created: LGY 2011-08-24
// -----------------------------------------------------------------------------
bool TrafficabilityAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::OnUpdate
// Created: CMA 2011-09-07
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::OnUpdate( const sword::MissionParameter_Value& attribute )
{
	max_ = attribute.list( 1 ).areal();
	NotifyAttributeUpdated( eOnUpdate );
}
