// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MineAttribute.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationType.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeMine.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "HLA/HLA_UpdateFunctor.h"
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( MineAttribute, "MineAttribute" )

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute()
    : dotation_( 0 )
    , nFullNbrDotation_( 0 )
    , nCurrentNbrDotation_( 0 )
    , rMiningPercentage_( 0 )
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( const PHY_DotationCategory& dotation, uint nDefaultMaxNbrDotation )
    : dotation_( &dotation )
    , nFullNbrDotation_( nDefaultMaxNbrDotation )
    , nCurrentNbrDotation_( 0 )
    , rMiningPercentage_( 0 )
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Load
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void MineAttribute::Load( xml::xistream& xis )
{
    assert( dotation_ != 0 );
    const MT_Float completion = xml::attribute< MT_Float >( xis, "completion", 1.f );
    if( completion > 0. && completion <= 1. )
        rMiningPercentage_ = completion;
    nCurrentNbrDotation_ = uint( rMiningPercentage_ * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MineAttribute::~MineAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MineAttribute& MineAttribute::operator=( const MineAttribute& rhs )
{    
    nCurrentNbrDotation_ = rhs.nCurrentNbrDotation_;
    rMiningPercentage_ = rhs.rMiningPercentage_;
    nFullNbrDotation_ = rhs.nFullNbrDotation_;
    dotation_ = rhs.dotation_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void MineAttribute::load( MIL_CheckPointInArchive& ar, const uint )
{
    std::string dotation;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> dotation
       >> nFullNbrDotation_
       >> nCurrentNbrDotation_
       >> rMiningPercentage_;
    dotation_ = PHY_DotationType::FindDotationCategory( dotation );
    if ( !dotation_ )
       throw std::runtime_error( "Unknown dotation category - " + dotation + " - " ); 
}
    
// -----------------------------------------------------------------------------
// Name: MineAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void MineAttribute::save( MIL_CheckPointOutArchive& ar, const uint ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    if ( dotation_ )
       ar << dotation_->GetName();
    else
       ar << "";
    ar << nFullNbrDotation_
       << nCurrentNbrDotation_
       << rMiningPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::WriteODB
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void MineAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "mine" ) 
            << xml::attribute( "completion", rMiningPercentage_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void MineAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeMine( *this ) );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MineAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    if( dotation_ )
    {
        asn.m.minePresent = 1;
        asn.mine.m.dotation_typePresent = 1;
        asn.mine.dotation_type = dotation_->GetMosID();
        asn.mine.m.percentagePresent = 1;
        asn.mine.percentage = uint( rMiningPercentage_ * 100. );
        asn.mine.m.dotation_nbrPresent = 1;
        asn.mine.dotation_nbr = nCurrentNbrDotation_;
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MineAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    if( NeedUpdate() )
    {
        asn.m.minePresent = 1;
        asn.mine.m.percentagePresent = 1;
        asn.mine.percentage = uint( rMiningPercentage_ * 100. );
        asn.mine.m.dotation_nbrPresent = 1;
        asn.mine.dotation_nbr = nCurrentNbrDotation_;
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::OnUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void MineAttribute::OnUpdate( const ASN1T_ObjectAttributes& asn )
{
    if( asn.m.minePresent )
    {
        if ( asn.mine.m.percentagePresent )
            Set( asn.mine.percentage / 100. );
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MineAttribute::Set( MT_Float percentage )
{    
    rMiningPercentage_ = std::max( 0., std::min( 1., percentage ) );
    nCurrentNbrDotation_ = (uint)( rMiningPercentage_ * nFullNbrDotation_ );
    NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MineAttribute::Update( MT_Float rDeltaPercentage )
{
    Set( rMiningPercentage_ + rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationNeededForConstruction
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
uint MineAttribute::GetDotationNeededForConstruction( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rMiningPercentage_ + rDeltaPercentage ) );
    return (uint)( rNewPercentage * nFullNbrDotation_ ) - nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationRecoveredWhenDestroying
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
uint MineAttribute::GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rMiningPercentage_ - rDeltaPercentage ) );
    return nCurrentNbrDotation_ - (uint)( rNewPercentage * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Serialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void MineAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "valorisation",  NeedUpdate( eOnHLAUpdate ), rMiningPercentage_ );
    Reset( eOnHLAUpdate );
}
    
// -----------------------------------------------------------------------------
// Name: MineAttribute::Deserialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void MineAttribute::Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer )
{
    if( attributeID == "valorisation" )
    {
        double percentage;
        deserializer >> percentage;
        Update( percentage );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetPercentage
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
MT_Float MineAttribute::GetState() const
{
    return rMiningPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetCurrentDotations
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
uint MineAttribute::GetCurrentDotations() const
{
    return nCurrentNbrDotation_;
}
