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
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "hla/HLA_UpdateFunctor.h"
#include "protocol/protocol.h"
#include <hla/AttributeIdentifier.h>

BOOST_CLASS_EXPORT_IMPLEMENT( BypassAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< BypassAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< BypassAttribute > )

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
// Created: RPD 2009-10-20
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( const Common::MsgMissionParameter_Value& attributes )
    : bypassPercentage_( attributes.list( 1 ).quantity() / 100., 0.05, 0., 1.)
{
    // NOTHING
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
    ar << (double)bypassPercentage_.Get();
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void BypassAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< BypassAttribute > >( *new DEC_Knowledge_ObjectAttributeProxyPassThrough< BypassAttribute >() );
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
void BypassAttribute::SendFullState( Common::ObjectAttributes& asn ) const
{
    asn.mutable_bypass()->set_percentage( unsigned int( bypassPercentage_.Send() * 100. ) );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void BypassAttribute::SendUpdate( Common::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BypassAttribute::OnUpdate( const Common::MsgMissionParameter_Value& attribute )
{
    if( attribute.list_size() > 1 )
    {
        if( bypassPercentage_.Set( attribute.list( 1 ).quantity() / 100. ) )
            NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Update
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void BypassAttribute::Update( double progress )
{
    if( bypassPercentage_.Set( bypassPercentage_.Get() + progress ) )
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
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
// Name: BypassAttribute::Serialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void BypassAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "contournement", NeedUpdate( eOnHLAUpdate ), bypassPercentage_.Get() );
    Reset( eOnHLAUpdate );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Deserialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void BypassAttribute::Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer )
{
    if( attributeID == "contournement" )
    {
        double bypass;
        deserializer >> bypass;
        bypassPercentage_.Set( bypass );
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool BypassAttribute::Update( const BypassAttribute& rhs )
{
    bypassPercentage_.Set( rhs.bypassPercentage_.Get() );
    if( bypassPercentage_.NeedToBeSent() )
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    return NeedUpdate( eOnUpdate );
}