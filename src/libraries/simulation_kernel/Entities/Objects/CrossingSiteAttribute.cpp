// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CrossingSiteAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( CrossingSiteAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyRecon< CrossingSiteAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyRecon< CrossingSiteAttribute > )

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( xml::xistream& xis )
    : rWidth_        ( 0. )
    , rDepth_        ( 0. )
    , rCurrentSpeed_ ( 0. )
    , bBanksToFitOut_( true )
{
    xis >> xml::content( "width", rWidth_ )
        >> xml::content( "depth", rDepth_ )
        >> xml::content( "speed", rCurrentSpeed_ )
        >> xml::content( "construction-needed", bBanksToFitOut_ );

    if( rWidth_ <= 0. )
        xis.error( "rWidth_ is not greater than 0 " );

    if( rDepth_ <= 0 )
        xis.error( "rDepth_ is not greater than 0 " );

    if( rCurrentSpeed_ < 0 )
        xis.error( "rCurrentSpeed_ is not greater or equal to 0 " );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::CrossingSiteAttribute
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute()
    : rWidth_        ( 0. )
    , rDepth_        ( 0. )
    , rCurrentSpeed_ ( 0. )
    , bBanksToFitOut_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( const Common::MsgMissionParameter_Value& attributes )
{
    rWidth_         = attributes.list( 1 ).quantity();
    rDepth_         = attributes.list( 2 ).quantity();
    rCurrentSpeed_  = attributes.list( 3 ).quantity();
    bBanksToFitOut_ = attributes.list( 4 ).abool();
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
CrossingSiteAttribute::~CrossingSiteAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::serialize
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
template < typename Archive >
void CrossingSiteAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & rWidth_
         & rDepth_
         & rCurrentSpeed_
         & bBanksToFitOut_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::WriteSpecificAttributes
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "crossing-site" )
            << xml::content( "width", rWidth_ )
            << xml::content( "depth", rDepth_ )
            << xml::content( "speed", rCurrentSpeed_ )
            << xml::content( "construction-needed", bBanksToFitOut_ )
        << xml::end; // specific-attributes
}


// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< CrossingSiteAttribute, CrossingSiteAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SendFullState( Common::ObjectAttributes& asn ) const
{
    asn.mutable_crossing_site()->set_width( int( rWidth_ ) );
    asn.mutable_crossing_site()->set_depth( int( rDepth_ ) );
    asn.mutable_crossing_site()->set_flow_rate( int(  rCurrentSpeed_ ) );
    asn.mutable_crossing_site()->set_banks_require_fitting( bBanksToFitOut_ );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SendUpdate( Common::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::OnUpdate
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::OnUpdate( const Common::MsgMissionParameter_Value& attribute )
{
    if( attribute.list_size() > 4 )
    {
        rWidth_         = attribute.list( 1 ).quantity();
        rDepth_         = attribute.list( 2 ).quantity();
        rCurrentSpeed_  = attribute.list( 3 ).quantity();
        bBanksToFitOut_ = attribute.list( 4 ).abool();
    }
}

// -----------------------------------------------------------------------------
// Name: DotationAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
CrossingSiteAttribute& CrossingSiteAttribute::operator=( const CrossingSiteAttribute& rhs )
{
    rWidth_         = rhs.rWidth_;
    rDepth_         = rhs.rDepth_;
    rCurrentSpeed_  = rhs.rCurrentSpeed_;
    bBanksToFitOut_ = rhs.bBanksToFitOut_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::GetWidth
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
double CrossingSiteAttribute::GetWidth() const
{
    return rWidth_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::GetDepth
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
double CrossingSiteAttribute::GetDepth() const
{
    return rDepth_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::GetSpeed
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
double CrossingSiteAttribute::GetSpeed() const
{
    return rCurrentSpeed_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::IsBanksToFitOut
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
bool CrossingSiteAttribute::IsBanksToFitOut() const
{
    return bBanksToFitOut_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool CrossingSiteAttribute::Update( const CrossingSiteAttribute& rhs )
{
    if( rWidth_ != rhs.rWidth_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        rWidth_ = rhs.rWidth_;
    }
    if( rDepth_ != rhs.rDepth_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        rDepth_ = rhs.rDepth_;
    }
    if( rCurrentSpeed_ != rhs.rCurrentSpeed_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        rCurrentSpeed_ = rhs.rCurrentSpeed_;
    }
    if( bBanksToFitOut_ != rhs.bBanksToFitOut_ )
    {
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
        bBanksToFitOut_ = rhs.bBanksToFitOut_;
    }
    return NeedUpdate( eOnUpdate );
}