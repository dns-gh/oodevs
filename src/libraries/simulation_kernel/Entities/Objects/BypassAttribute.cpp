// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BypassAttribute.h"
#include "ObjectAttributes.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BypassAttribute )

BOOST_CLASS_EXPORT( DEC_Knowledge_ObjectAttributeProxyPassThrough< BypassAttribute > )

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute()
    : bypassPercentage_( 0., 0.05, 0., 1.)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( const xml::xistream& xis )
    : bypassPercentage_( xis.attribute< double >( "value" ), 0.05, 0., 1.)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: RPD 2009-10-20
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( const sword::MissionParameter_Value& attributes )
    : bypassPercentage_( 0., 0.05, 0., 1.)
{
    CheckCount( "bypass", attributes, 2 );
    bypassPercentage_.Set( GetQuantity( "bypass", attributes, 1 ) / 100. );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
BypassAttribute::~BypassAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void BypassAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    double tmp;
    ar >> tmp;
    bypassPercentage_.Set( tmp );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void BypassAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    std::string emptyString;
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << static_cast< double >( bypassPercentage_.Get() );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::WriteODB
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
void BypassAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "bypass")
            << xml::attribute("value", bypassPercentage_.Get() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void BypassAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< BypassAttribute > >(
        *new DEC_Knowledge_ObjectAttributeProxyPassThrough< BypassAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void BypassAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< BypassAttribute, BypassAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::SendFullState
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void BypassAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_bypass()->set_percentage( static_cast< unsigned int >(
                bypassPercentage_.Send() * 100 + 0.5f ) );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
bool BypassAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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
// Name: BypassAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BypassAttribute::OnUpdate( const sword::MissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        if( bypassPercentage_.Set( attribute.list( 1 ).quantity() / 100. ) )
            NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Update
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void BypassAttribute::Update( double progress )
{
    if( bypassPercentage_.Set( bypassPercentage_.Get() + progress ) )
        NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
BypassAttribute& BypassAttribute::operator=( const BypassAttribute& rhs )
{
    bypassPercentage_ = rhs.bypassPercentage_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::IsBypassed
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool BypassAttribute::IsBypassed() const
{
    return bypassPercentage_.Get() >= 1.;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool BypassAttribute::Update( const BypassAttribute& rhs )
{
    bypassPercentage_.Set( rhs.bypassPercentage_.Get() );
    if( bypassPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate );
    return NeedUpdate( eOnUpdate );
}
