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
#include "Object.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationType.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeMine.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "hla/HLA_UpdateFunctor.h"
#include "protocol/protocol.h"
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MineAttribute )

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute()
    : dotation_           ( 0 )
    , nFullNbrDotation_   ( 0 )
    , nCurrentNbrDotation_( 0 )
    , rMiningPercentage_  ( 0 )
    , nMinesActivityTime_ ( 0 )
    , nDeathTimeStep_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( const PHY_DotationCategory& dotation, unsigned int nDefaultMaxNbrDotation )
    : dotation_           ( &dotation )
    , nFullNbrDotation_   ( nDefaultMaxNbrDotation )
    , nCurrentNbrDotation_( nDefaultMaxNbrDotation )
    , rMiningPercentage_  ( 1. )
    , nMinesActivityTime_ ( 0 )
    , nDeathTimeStep_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: RPD 2009-10-19
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( const Common::MsgMissionParameter_Value& attributes  )
    : dotation_           ( 0 )
    , nFullNbrDotation_   ( 0 )
    , nCurrentNbrDotation_( 0 )
    , rMiningPercentage_  ( 0 )
    , nMinesActivityTime_ ( 0 )
    , nDeathTimeStep_     ( 0 )
{
    dotation_ = PHY_DotationType::FindDotationCategory( attributes.list( 1 ).identifier() );
    if( !dotation_ )
        throw std::runtime_error( "Unknown 'Dotation Type' for mine attribute" );
    nCurrentNbrDotation_ = attributes.list( 2 ).quantity();
    nFullNbrDotation_ = static_cast< unsigned int >( attributes.list( 3 ).areal() );
    rMiningPercentage_ = attributes.list( 4 ).quantity();
    //nMinesActivityTime_;
    //nDeathTimeStep_;
}


// -----------------------------------------------------------------------------
// Name: MineAttribute::Load
// Created: JCR 2008-08-26
// -----------------------------------------------------------------------------
void MineAttribute::Load( xml::xistream& xis )
{
    assert( dotation_ != 0 );
    const double completion = xis.attribute< double >( "completion", 1.f );
    if( completion > 0. && completion <= 1. )
        rMiningPercentage_ = completion;
    nCurrentNbrDotation_ = static_cast< unsigned int >( rMiningPercentage_ * nFullNbrDotation_ );
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
void MineAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::string dotation;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> dotation
       >> nFullNbrDotation_
       >> nCurrentNbrDotation_
       >> rMiningPercentage_;
    dotation_ = PHY_DotationType::FindDotationCategory( dotation );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void MineAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    if( dotation_ )
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
        << xml::end;
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
// Name: MineAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void MineAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< MineAttribute, MineAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MineAttribute::SendFullState( Common::ObjectAttributes& asn ) const
{
    if( dotation_ )
    {
        asn.mutable_mine()->mutable_resource()->set_id( dotation_->GetMosID() );
        asn.mutable_mine()->set_percentage( unsigned int( rMiningPercentage_ * 100. ) );
        asn.mutable_mine()->set_dotation_nbr( nCurrentNbrDotation_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MineAttribute::SendUpdate( Common::ObjectAttributes& asn ) const
{
    if( NeedUpdate() )
    {
        asn.mutable_mine()->set_percentage( unsigned int( rMiningPercentage_ * 100. ) );
        asn.mutable_mine()->set_dotation_nbr( nCurrentNbrDotation_ );
        Reset( eOnUpdate | eOnHLAUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::OnUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void MineAttribute::OnUpdate( const Common::MsgMissionParameter_Value& attribute )
{
    if( attribute.list_size() > 4 )
    {
        Set( attribute.list( 4 ).quantity() / 100. ); // four first parameters not used
        NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MineAttribute::Set( double percentage )
{
    rMiningPercentage_ = std::max( 0., std::min( 1., percentage ) );
    nCurrentNbrDotation_ = (unsigned int)( rMiningPercentage_ * nFullNbrDotation_ );
    NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
void MineAttribute::Update( double rDeltaPercentage )
{
    Set( rMiningPercentage_ + rDeltaPercentage );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationNeededForConstruction
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetDotationNeededForConstruction( double rDeltaPercentage ) const
{
    const double rNewPercentage = std::max( 0., std::min( 1., rMiningPercentage_ + rDeltaPercentage ) );
    return (unsigned int)( rNewPercentage * nFullNbrDotation_ ) - nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationRecoveredWhenDestroying
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetDotationRecoveredWhenDestroying( double rDeltaPercentage ) const
{
    const double rNewPercentage = std::max( 0., std::min( 1., rMiningPercentage_ - rDeltaPercentage ) );
    return nCurrentNbrDotation_ - (unsigned int)( rNewPercentage * nFullNbrDotation_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Serialize
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void MineAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "valorisation", NeedUpdate( eOnHLAUpdate ), rMiningPercentage_ );
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
double MineAttribute::GetState() const
{
    return rMiningPercentage_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetCurrentDotations
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetCurrentDotations() const
{
    return nCurrentNbrDotation_;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::GetDotationType
// Created: JSR 2010-05-27
// -----------------------------------------------------------------------------
unsigned int MineAttribute::GetDotationType() const
{
    return dotation_ ? dotation_->GetMosID() : 0;
}
