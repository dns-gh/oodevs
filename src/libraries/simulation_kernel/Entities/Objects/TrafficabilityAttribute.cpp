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
#include "ObjectAttributes.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( TrafficabilityAttribute )

BOOST_CLASS_EXPORT( DEC_Knowledge_ObjectAttributeProxyPassThrough< TrafficabilityAttribute > )

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute()
    : max_( 0. )
{
    // NOTHING
}

TrafficabilityAttribute::TrafficabilityAttribute( const sword::MissionParameter_Value& attributes )
    : max_( 0. )
{
    CheckCount( "trafficability", attributes, 2 );
    max_ = GetReal( "trafficability", attributes, 1 );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( xml::xistream& xis )
    : max_( xis.attribute< float >( "max" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute constructor
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
TrafficabilityAttribute::TrafficabilityAttribute( float max )
    : max_( max )
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
// Name: TrafficabilityAttribute::load
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> max_;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::save
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << max_;
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::GetMaxValue
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
float TrafficabilityAttribute::GetMaxValue() const
{
    return max_;
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

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::Instanciate
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
void TrafficabilityAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< TrafficabilityAttribute > >(
        *new DEC_Knowledge_ObjectAttributeProxyPassThrough< TrafficabilityAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityAttribute::Update
// Created: CMA 2011-09-09
// -----------------------------------------------------------------------------
bool TrafficabilityAttribute::Update( const TrafficabilityAttribute& rhs )
{
    if( max_ != rhs.max_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        max_ = rhs.max_;
    }
    return NeedUpdate( eOnUpdate );
}
