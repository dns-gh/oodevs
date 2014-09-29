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
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( SupplyRouteAttribute )

BOOST_CLASS_EXPORT( DEC_Knowledge_ObjectAttributeProxyPassThrough< SupplyRouteAttribute > )

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
        throw MASA_EXCEPTION( xis.context() + "max-weight is not greater than 0" );
    if( rWidth_ <= 0. )
        throw MASA_EXCEPTION( xis.context() + "width is not greater than 0" );
    if( rLength_ <= 0. )
        throw MASA_EXCEPTION( xis.context() + "length is not greater than 0" );
    if( rFlow_ <= 0. )
        throw MASA_EXCEPTION( xis.context() + "flow is not greater than 0" );
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
SupplyRouteAttribute::SupplyRouteAttribute( const sword::MissionParameter_Value& attributes )
    : bEquipped_       ( attributes.list( 1 ).booleanvalue() )
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
    file & bEquipped_;
    file & rWeightSupported_;
    file & rWidth_;
    file & rLength_;
    file & rFlow_;
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
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< SupplyRouteAttribute > >(
        *new DEC_Knowledge_ObjectAttributeProxyPassThrough< SupplyRouteAttribute >() );
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
void SupplyRouteAttribute::SendFullState( sword::ObjectAttributes& asn ) const
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
bool SupplyRouteAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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
        NotifyAttributeUpdated( eOnUpdate );
        bEquipped_ = rhs.bEquipped_;
    }
    if( rWeightSupported_ != rhs.rWeightSupported_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        rWeightSupported_ = rhs.rWeightSupported_;
    }
    if( rWidth_ != rhs.rWidth_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        rWidth_ = rhs.rWidth_;
    }
    if( rLength_ != rhs.rLength_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        rLength_ = rhs.rLength_;
    }
    if( rFlow_ != rhs.rFlow_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        rFlow_ = rhs.rFlow_;
    }
    return NeedUpdate( eOnUpdate );
}