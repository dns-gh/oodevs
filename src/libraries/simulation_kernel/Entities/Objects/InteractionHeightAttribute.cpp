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

#include "Knowledge/DEC_Knowledge_ObjectAttributeInteractionHeight.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( InteractionHeightAttribute, "InteractionHeightAttribute" )

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
InteractionHeightAttribute::InteractionHeightAttribute( const ASN1T_ObjectAttributes& asn )
    : height_( asn.interaction_height.height )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute constructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
InteractionHeightAttribute::InteractionHeightAttribute( xml::xistream& xis )
: height_( xml::attribute( xis, "height", 0.f ) )
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
// Name: template< typename Archive > void InteractionHeightAttribute::serialize
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
template< typename Archive > void InteractionHeightAttribute::serialize( Archive& file, const uint )
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
    object.Attach( *new DEC_Knowledge_ObjectAttributeInteractionHeight( *this ) );
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::SendFullState
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    asn.m.interaction_heightPresent = 1;
    asn.interaction_height.height = height_;
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::WriteODB
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void InteractionHeightAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "max-interaction-height" ) 
            << xml::attribute( "height", height_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: InteractionHeightAttribute::Get
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
MT_Float InteractionHeightAttribute::Get() const
{
    return height_;
}
