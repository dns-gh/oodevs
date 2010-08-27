// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SealOffAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( SealOffAttribute )

// -----------------------------------------------------------------------------
// Name: SealOffAttribute constructor
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffAttribute::SealOffAttribute()
: rSealOffLevel_( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute destructor
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffAttribute::~SealOffAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::serialize
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
template< typename Archive >
void SealOffAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & rSealOffLevel_;
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::Register
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< SealOffAttribute, SealOffAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::operator=
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
SealOffAttribute& SealOffAttribute::operator=( const SealOffAttribute& rhs )
{
    rSealOffLevel_ = rhs.rSealOffLevel_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::SendFullState
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    asn.mutable_sealoff()->set_level( unsigned int( rSealOffLevel_ * 100 ) );
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::Send
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnCreation ) | NeedUpdate( eOnUpdate ) )
    {
        asn.mutable_sealoff()->set_level( unsigned int( rSealOffLevel_ * 100 ) );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: SealOffAttribute::SetSealOffLevel
// Created: MGD 2010-08-24
// -----------------------------------------------------------------------------
void SealOffAttribute::SetSealOffLevel( float level )
{
    rSealOffLevel_ = std::max( 0.f, std::min( 1.f, level ) );
    NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
}
