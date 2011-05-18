// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AltitudeModifierAttribute.h"
#include "MIL_Object_ABC.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( AltitudeModifierAttribute )

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute()
    : height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( const xml::xistream& xis )
    : height_( xis.attribute< int >( "height" ) )
{
    if( height_ < 0 )
        xis.error( "height_ is not greater than or equal to 0" );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( const sword::MissionParameter_Value& attributes )
    : height_( attributes.list( 1 ).quantity() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::~AltitudeModifierAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::serialize
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
template< typename Archive >
void AltitudeModifierAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & height_;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Register
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< AltitudeModifierAttribute, AltitudeModifierAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::WriteODB
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "altitude-modifier" )
            << xml::attribute( "height", height_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SendFullState
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_altitude_modifier()->set_height( height_ );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SendUpdate
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::operator=
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute& AltitudeModifierAttribute::operator=( const AltitudeModifierAttribute& from )
{
    height_ = from.height_;
    return *this;
}
