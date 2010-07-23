// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DelayAttribute.h"
#include "MIL_Object_ABC.h"
#include "Tools/MIL_Tools.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DelayAttribute )

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute()
    : nDelay_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( xml::xistream& xis )
    : nDelay_( 0 )
{
    xis >> xml::attribute( "value", nDelay_ );
    if( nDelay_ < 0 )
        xis.error( "nDelay_ is not greater than or equal to 0" );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( unsigned int nDelay )
    : nDelay_( nDelay )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( const Common::MsgMissionParameter_Value& attributes )
    : nDelay_( attributes.list( 1 ).quantity() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::DelayAttribute( const DelayAttribute& from )
    : nDelay_( from.nDelay_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::operator=
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute& DelayAttribute::operator=( const DelayAttribute& from )
{
    nDelay_ = from.nDelay_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayAttribute::~DelayAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::serialize
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
template< typename Archive >
void DelayAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & nDelay_;
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::Register
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< DelayAttribute, DelayAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::WriteODB
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "delay" )
            << xml::attribute( "value", nDelay_ );
    xos << xml::end;}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::SendFullState
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    asn.mutable_effect_delay()->set_value( nDelay_ );
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::SendUpdate
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
{
    if( NeedUpdate() )
    {
        SendFullState( asn );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: DelayAttribute::IsOverDelay
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
bool DelayAttribute::IsOverDelay( unsigned int nTicks ) const
{
    return nTicks >= MIL_Tools::ConvertSecondsToSim( nDelay_ );
}
