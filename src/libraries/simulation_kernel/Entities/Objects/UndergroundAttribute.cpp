// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UndergroundAttribute.h"
#include "MIL_Object_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( UndergroundAttribute )

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( xml::xistream& xis )
    : network_( xis.attribute< std::string >( "network" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( const sword::MissionParameter_Value& attributes )
    : network_( attributes.list( 1 ).acharstr() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute constructor
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute::UndergroundAttribute( const UndergroundAttribute& from )
    : network_( from.network_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::operator=
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute& UndergroundAttribute::operator=( const UndergroundAttribute& from )
{
    network_ = from.network_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute destructor
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
UndergroundAttribute::~UndergroundAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::serialize
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
template< typename Archive >
void UndergroundAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & network_;
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::Register
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< UndergroundAttribute, UndergroundAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::WriteODB
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "underground" )
            << xml::attribute( "network", network_ )
        << xml::end;

}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::SendFullState
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_underground()->set_network( network_ );
}

// -----------------------------------------------------------------------------
// Name: UndergroundAttribute::SendUpdate
// Created: JSR 2011-07-11
// -----------------------------------------------------------------------------
void UndergroundAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}
