// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CrossingSiteAttribute.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeCrossingSite.h"
#include "Knowledge/DEC_Knowledge_Object.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( CrossingSiteAttribute, "CrossingSiteAttribute" )

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( xml::xistream& xis )
    : rWidth_           ( 0. )
    , rDepth_           ( 0. )
    , rCurrentSpeed_    ( 0. )
    , bBanksToFitOut_   ( true )   
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
    : rWidth_           ( 0. )
    , rDepth_           ( 0. )
    , rCurrentSpeed_    ( 0. )
    , bBanksToFitOut_   ( true )   
{

}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute constructor
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
CrossingSiteAttribute::CrossingSiteAttribute( const ASN1T_ObjectAttributes& asn )
{
    OnUpdate( asn );
}


// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
CrossingSiteAttribute::~CrossingSiteAttribute()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::serialize
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
template < typename Archive >
void CrossingSiteAttribute::serialize( Archive& file, const uint )
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
        << xml::end(); // specific-attributes
}


// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeCrossingSite( *this ) );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    asn.m.crossing_sitePresent = 1;
    asn.crossing_site.width                 = (int)rWidth_;
    asn.crossing_site.depth                 = (int)rDepth_;
    asn.crossing_site.flow_rate             = (int)rCurrentSpeed_;
    asn.crossing_site.banks_require_fitting = bBanksToFitOut_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate( eOnCreation ) )
    {
        SendFullState( asn );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::OnUpdate
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void CrossingSiteAttribute::OnUpdate( const ASN1T_ObjectAttributes& asn )
{
    rWidth_         = asn.crossing_site.width;
    rDepth_         = asn.crossing_site.depth;
    rCurrentSpeed_  = asn.crossing_site.flow_rate;
    bBanksToFitOut_ = asn.crossing_site.banks_require_fitting != false;
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
MT_Float CrossingSiteAttribute::GetWidth() const
{
    return rWidth_;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::GetDepth
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
MT_Float CrossingSiteAttribute::GetDepth() const
{
    return rDepth_;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttribute::GetSpeed
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
MT_Float CrossingSiteAttribute::GetSpeed() const
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