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
#include "Knowledge/DEC_Knowledge_ObjectAttributeBypass.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Hla/HLA_UpdateFunctor.h"

#include <hla/AttributeIdentifier.h>

BOOST_CLASS_EXPORT_GUID( BypassAttribute, "BypassAttribute" )

using namespace hla;

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute()
    : rBypass_ ( 0. )
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
// Name: BypassAttribute::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void BypassAttribute::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & rBypass_;         
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void BypassAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeBypass( *this ) );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::SendFullState
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void BypassAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{    
    asn.m.bypassPresent = 1;
    asn.bypass.m.percentagePresent = 1;
    asn.bypass.percentage = uint( rBypass_ * 100. );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void BypassAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void BypassAttribute::OnUpdate( const ASN1T_ObjectAttributes& asn )
{
    if( asn.m.bypassPresent ) 
        rBypass_ = asn.bypass.percentage / 100.;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Update
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void BypassAttribute::Update( MT_Float progress )
{
    rBypass_ += progress;
    NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
BypassAttribute& BypassAttribute::operator=( const BypassAttribute& rhs )
{    
    rBypass_ = rhs.rBypass_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::GetState
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
MT_Float BypassAttribute::GetState() const
{
    return rBypass_;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::GetState
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
bool BypassAttribute::IsBypassed() const
{
    return rBypass_ >= 1.;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Serialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void BypassAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "contournement", NeedUpdate( eOnHLAUpdate ), rBypass_ );
    Reset( eOnHLAUpdate );
}
    
// -----------------------------------------------------------------------------
// Name: BypassAttribute::Deserialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void BypassAttribute::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if ( attributeID == "contournement" )
    {
        deserializer >> rBypass_;
        NotifyAttributeUpdated( eOnUpdate );
    }
}