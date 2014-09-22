// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InteractionHeightAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( InteractionHeightAttribute )

BOOST_CLASS_EXPORT( DEC_Knowledge_ObjectAttributeProxyPassThrough< InteractionHeightAttribute > )

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute constructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
InteractionHeightAttribute::InteractionHeightAttribute()
    : height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute constructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
InteractionHeightAttribute::InteractionHeightAttribute( const sword::MissionParameter_Value& attributes )
    : height_( attributes.list( 1 ).quantity() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute constructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
InteractionHeightAttribute::InteractionHeightAttribute( xml::xistream& xis )
    : height_( xis.attribute< float >( "height", 0.f ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute constructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
InteractionHeightAttribute::InteractionHeightAttribute( double height )
    : height_( height )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute destructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
InteractionHeightAttribute::~InteractionHeightAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::serialize
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
template< typename Archive >
void InteractionHeightAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & height_;
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::Instanciate
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< InteractionHeightAttribute > >(
        *new DEC_Knowledge_ObjectAttributeProxyPassThrough< InteractionHeightAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< InteractionHeightAttribute, InteractionHeightAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::SendFullState
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_interaction_height()->set_height( static_cast< float >( height_ ) );
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::WriteODB
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::WriteODB( xml::xostream& /*xos*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::Get
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
double InteractionHeightAttribute::Get() const
{
    return height_;
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool InteractionHeightAttribute::Update( const InteractionHeightAttribute& rhs )
{
    if( height_ != rhs.height_ )
    {
        height_ = rhs.height_;
        return true;
    }
    return false;
}