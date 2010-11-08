// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyRouteAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include "protocol/protocol.h"
#include <hla/HLA_UpdateFunctor.h>
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( SupplyRouteAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< SupplyRouteAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< SupplyRouteAttribute > )

using namespace hla;

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute( xml::xistream& xis )
    : bEquipped_       ( false )
    , rWeightSupported_( 0. )
    , rWidth_          ( 0. )
    , rLength_         ( 0. )
    , rFlow_           ( 0. )
{
    xis >> xml::content( "max-weight", rWeightSupported_ )
        >> xml::content( "width", rWidth_ )
        >> xml::content( "length", rLength_ )
        >> xml::content( "flow", rFlow_ )
        >> xml::content( "equipped", bEquipped_ );

    if( rWeightSupported_ <= 0. )
        xis.error( "max-weight is not greater than 0" );
    if( rWidth_ <= 0. )
        xis.error( "width is not greater than 0" );
    if( rLength_ <= 0. )
        xis.error( "length is not greater than 0" );
    if( rFlow_ <= 0. )
        xis.error( "flow is not greater than 0" );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SupplyRouteAttribute
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute()
    : bEquipped_       ( false )
    , rWeightSupported_( 0. )
    , rWidth_          ( 0. )
    , rLength_         ( 0. )
    , rFlow_           ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute constructor
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
SupplyRouteAttribute::SupplyRouteAttribute( const Common::MsgMissionParameter_Value& attributes )
    : bEquipped_       ( attributes.list( 1 ).abool() )
    , rWeightSupported_( attributes.list( 2 ).quantity() )
    , rWidth_          ( attributes.list( 3 ).quantity() )
    , rLength_         ( attributes.list( 4 ).quantity() )
    , rFlow_           ( attributes.list( 5 ).quantity() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
SupplyRouteAttribute::~SupplyRouteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
SupplyRouteAttribute& SupplyRouteAttribute::operator=( const SupplyRouteAttribute& rhs )
{
    bEquipped_        = rhs.bEquipped_;
    rWeightSupported_ = rhs.rWeightSupported_;
    rWidth_           = rhs.rWidth_;
    rLength_          = rhs.rLength_;
    rFlow_            = rhs.rFlow_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void SupplyRouteAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & bEquipped_
         & rWeightSupported_
         & rWidth_
         & rLength_
         & rFlow_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "supply-route" )
            << xml::content( "max-weight", rWeightSupported_ )
            << xml::content( "width"     , rWidth_ )
            << xml::content( "length"    , rLength_ )
            << xml::content( "flow"      , rFlow_ )
            << xml::content( "equipped"  , bEquipped_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeProxyPassThrough< SupplyRouteAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< SupplyRouteAttribute, SupplyRouteAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::SendFullState
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SendFullState( Common::ObjectAttributes& asn ) const
{
    asn.mutable_supply_route()->set_max_weight( (int)rWeightSupported_ );
    asn.mutable_supply_route()->set_width( (int)rWidth_ );
    asn.mutable_supply_route()->set_length( (int)rLength_ );
    asn.mutable_supply_route()->set_flow_rate( (int)rFlow_ );
    asn.mutable_supply_route()->set_equipped( bEquipped_ );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::SendUpdate( Common::ObjectAttributes& asn ) const
{
    if( NeedUpdate() )
    {
        asn.mutable_supply_route()->set_equipped( bEquipped_ );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Deserialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "option" )
    {
        std::string strEquipped;
        deserializer >> strEquipped;
        if( strEquipped == "equipped" )
            bEquipped_ = true;
        else
            bEquipped_ = false;
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Serialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "option", NeedUpdate( eOnHLAUpdate ), bEquipped_ ? "equipped" : "not equipped" );
    Reset( eOnHLAUpdate );
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Equip
// Created: LDC 2009-03-26
// -----------------------------------------------------------------------------
void SupplyRouteAttribute::Equip()
{
    bEquipped_ = true;
    NotifyAttributeUpdated( eOnUpdate );
}
//
// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::IsEquipped
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
bool SupplyRouteAttribute::IsEquipped() const
{
    return bEquipped_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::GetMaxWeight
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
double SupplyRouteAttribute::GetMaxWeight() const
{
    return rWeightSupported_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::GetWidth
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
double SupplyRouteAttribute::GetWidth() const
{
    return rWidth_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::GetLength
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
double SupplyRouteAttribute::GetLength() const
{
    return rLength_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::GetFlow
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
double SupplyRouteAttribute::GetFlow() const
{
    return rFlow_;
}

// -----------------------------------------------------------------------------
// Name: SupplyRouteAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool SupplyRouteAttribute::Update( const SupplyRouteAttribute& rhs )
{
    if( bEquipped_ != rhs.bEquipped_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        bEquipped_ = rhs.bEquipped_;
    }
    if( rWeightSupported_ != rhs.rWeightSupported_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        rWeightSupported_ = rhs.rWeightSupported_;
    }
    if( rWidth_ != rhs.rWidth_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        rWidth_ = rhs.rWidth_;
    }
    if( rLength_ != rhs.rLength_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        rLength_ = rhs.rLength_;
    }
    if( rFlow_ != rhs.rFlow_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        rFlow_ = rhs.rFlow_;
    }
    return NeedUpdate();
}